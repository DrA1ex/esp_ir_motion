#include "application.h"

#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <lib/utils/qr.h>

Application::Application(FS *fs) : _fs(fs), _config_storage(_timer, "config") {}

void Application::begin() {
    _config_storage.begin(_fs);

    _wifi_manager = std::make_unique<WifiManager>(sys_config().wifi_ssid, sys_config().wifi_password);
    _ws_server = std::make_unique<WebSocketServer<PropertyEnum>>();
    _mqtt_server = std::make_unique<MqttServer>();

    _motion_control = std::make_unique<MotionControl>(config());

    _metadata = std::make_unique<ConfigMetadata>(build_metadata(config(), motion_control()));

    NotificationBus::get().subscribe([this](auto sender, auto prop) {
        if (sender != this) update();
    });

    _timer.add_interval([this](auto) {
        _ws_server->send_notification(PropertyEnum::MOTION_STATE);
        _ws_server->send_notification(PropertyEnum::MOTION_SILENCE_TIME_LEFT);
    }, STATE_NOTIFICATION_INTERVAL);

    _motion_control->event().subscribe(this, [this](auto, auto type, auto) {
        _handle_motion_sensor_event(type);
    });

    _metadata->visit([this](AbstractPropertyMeta *meta) {
        auto binary_protocol = (BinaryProtocolMeta<PropertyEnum> *) meta->get_binary_protocol();
        if (binary_protocol->packet_type.has_value()) {
            _ws_server->register_parameter(*binary_protocol->packet_type, meta->get_parameter());
            VERBOSE(D_PRINTF("WebSocket: Register property %s\r\n", __debug_enum_str(*binary_protocol->packet_type)));
        }

        auto mqtt_protocol = meta->get_mqtt_protocol();
        if (mqtt_protocol->topic_in && mqtt_protocol->topic_out) {
            _mqtt_server->register_parameter(mqtt_protocol->topic_in, mqtt_protocol->topic_out, meta->get_parameter());
            VERBOSE(D_PRINTF("MQTT: Register property %s <-> %s\r\n", mqtt_protocol->topic_in, mqtt_protocol->topic_out));
        } else if (mqtt_protocol->topic_out) {
            _mqtt_server->register_notification(mqtt_protocol->topic_out, meta->get_parameter());
            VERBOSE(D_PRINTF("MQTT: Register notification -> %s\r\n", mqtt_protocol->topic_out));
        }
    });

    _ws_server->register_data_request(PropertyEnum::GET_STATE, _metadata->generated.app_state);

    _ws_server->register_notification(PropertyEnum::MOTION_STATE, _metadata->generated.sensor_state);
    _ws_server->register_notification(PropertyEnum::MOTION_SILENCE_TIME_LEFT, _metadata->generated.sensor_silence_time_left);

    _ws_server->register_command(PropertyEnum::SILENCE, std::bind(&MotionControl::silence_add, &motion_control()));
    _ws_server->register_command(PropertyEnum::SILENCE_RESET, std::bind(&MotionControl::silence_reset, &motion_control()));
    _ws_server->register_command(PropertyEnum::TEST, std::bind(&MotionControl::alarm_test, &motion_control()));

    _ws_server->register_data_request((PropertyEnum) SystemPacketTypeEnum::GET_CONFIG, _metadata->data.config);
    _ws_server->register_command((PropertyEnum) SystemPacketTypeEnum::RESTART, std::bind(&Application::restart, this));
}

void Application::update() {
    _config_storage.save();
}

void Application::event_loop() {
    switch (_state) {
        case ApplicationState::UNINITIALIZED:
            _wifi_manager->connect(sys_config().wifi_mode, sys_config().wifi_connection_timeout);
            _state = ApplicationState::WIFI_CONNECT;

            break;

        case ApplicationState::WIFI_CONNECT:
            _wifi_manager->handle_connection();
            if (_wifi_manager->state() == WifiManagerState::CONNECTED) {
                _state = ApplicationState::INITIALIZING;
            }

            break;

        case ApplicationState::INITIALIZING:
            ArduinoOTA.setHostname(sys_config().mdns_name);
            ArduinoOTA.begin();

            _ws_server->begin(_web_server);
            _web_server.begin(_fs);

            if (sys_config().mqtt_enabled) {
                _mqtt_server->begin(sys_config().mqtt_host, sys_config().mqtt_port,
                                    sys_config().mqtt_user, sys_config().mqtt_password);
            }

            _motion_control->begin();

            D_PRINT("ESP Ready");
            _after_init();

            _state = ApplicationState::READY;
            break;

        case ApplicationState::READY:
            _wifi_manager->handle_connection();
            ArduinoOTA.handle();

            if (_dns_server) _dns_server->processNextRequest();

            _timer.handle_timers();

            _ws_server->handle_connection();
            _mqtt_server->handle_connection();

            _motion_control->tick();
            break;
    }
}

void Application::_after_init() {
    if (_wifi_manager->mode() == WifiMode::AP) {
        _dns_server = std::make_unique<DNSServer>();
        _dns_server->start(53, "*", WiFi.softAPIP());

        D_PRINT("Connect to WiFi:");
        qr_print_wifi_connection(_wifi_manager->ssid(), _wifi_manager->password());
    } else {
        String url = "http://";
        url += sys_config().mdns_name;
        url += ".local";

        if (_web_server.port() != 80) {
            url += ":";
            url += _web_server.port();
        }

        D_PRINT("Open WebUI:");
        qr_print_string(url.c_str());
    }
}

void Application::restart() {
    D_PRINTF("Received restart signal. Restarting after %u ms.\r\n", RESTART_DELAY);

    if (_config_storage.is_pending_commit()) _config_storage.force_save();

    _timer.add_timeout([](auto) { ESP.restart(); }, RESTART_DELAY);
}

void Application::_handle_motion_sensor_event(MotionEventType type) {
    switch (type) {
        case MotionEventType::STATE_CHANGED:
            _ws_server->send_notification(PropertyEnum::MOTION_STATE);
            _mqtt_server->send_notification(MQTT_OUT_TOPIC_ALARM);
            break;

        case MotionEventType::SILENCE_TIME_LEFT_CHANGED:
            _ws_server->send_notification(PropertyEnum::MOTION_SILENCE_TIME_LEFT);
            break;
    }
}
