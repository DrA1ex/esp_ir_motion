#include "application.h"

void Application::begin() {
    D_PRINT("Starting application...");

    if (!LittleFS.begin()) {
        D_PRINT("Unable to initialize FS");
    }

    _bootstrap = new Bootstrap<Config, PropertyEnum>(&LittleFS);

    auto &sys_config = _bootstrap->config().sys_config;
    _bootstrap->begin({
                              .mdns_name = sys_config.mdns_name,
                              .wifi_mode = sys_config.wifi_mode,
                              .wifi_ssid = sys_config.wifi_ssid,
                              .wifi_password = sys_config.wifi_password,
                              .wifi_connection_timeout = sys_config.wifi_connection_timeout,
                              .mqtt_enabled = sys_config.mqtt_enabled,
                              .mqtt_host = sys_config.mqtt_host,
                              .mqtt_port = sys_config.mqtt_port,
                              .mqtt_user = sys_config.mqtt_user,
                              .mqtt_password = sys_config.mqtt_password,
                      });

    _motion_control = new MotionControl(_bootstrap->config());
    _motion_control->begin();

    _setup();
    _initialized = true;
}

void Application::event_loop() {
    if (!_initialized) return;

    auto state = _bootstrap->event_loop();

    if (state == BootstrapState::READY) {
        _motion_control->tick();
    }
}

void Application::_setup() {
    NotificationBus::get().subscribe([this](auto sender, auto prop) {
        if (sender != this) _bootstrap->save_changes();
    });

    _motion_control->event().subscribe(this, [this](auto, auto type, auto) {
        _handle_motion_sensor_event(type);
    });

    auto &ws_server = _bootstrap->ws_server();
    auto &mqtt_server = _bootstrap->mqtt_server();

    _bootstrap->timer().add_interval([&ws_server](auto) {
        ws_server->send_notification(PropertyEnum::MOTION_STATE);
        ws_server->send_notification(PropertyEnum::MOTION_SILENCE_TIME_LEFT);
    }, STATE_NOTIFICATION_INTERVAL);

    _metadata = std::make_unique<ConfigMetadata>(build_metadata(_bootstrap->config(), *_motion_control));
    _metadata->visit([&ws_server, &mqtt_server](AbstractPropertyMeta *meta) {
        auto binary_protocol = (BinaryProtocolMeta<PropertyEnum> *) meta->get_binary_protocol();
        if (binary_protocol->packet_type.has_value()) {
            ws_server->register_parameter(*binary_protocol->packet_type, meta->get_parameter());
            VERBOSE(D_PRINTF("WebSocket: Register property %s\r\n", __debug_enum_str(*binary_protocol->packet_type)));
        }

        auto mqtt_protocol = meta->get_mqtt_protocol();
        if (mqtt_protocol->topic_in && mqtt_protocol->topic_out) {
            mqtt_server->register_parameter(mqtt_protocol->topic_in, mqtt_protocol->topic_out, meta->get_parameter());
            VERBOSE(D_PRINTF("MQTT: Register property %s <-> %s\r\n", mqtt_protocol->topic_in, mqtt_protocol->topic_out));
        } else if (mqtt_protocol->topic_out) {
            mqtt_server->register_notification(mqtt_protocol->topic_out, meta->get_parameter());
            VERBOSE(D_PRINTF("MQTT: Register notification -> %s\r\n", mqtt_protocol->topic_out));
        }
    });

    ws_server->register_data_request(PropertyEnum::GET_STATE, _metadata->generated.app_state);

    ws_server->register_notification(PropertyEnum::MOTION_STATE, _metadata->generated.sensor_state);
    ws_server->register_notification(PropertyEnum::MOTION_SILENCE_TIME_LEFT, _metadata->generated.sensor_silence_time_left);

    ws_server->register_command(PropertyEnum::SILENCE, [this] { _motion_control->silence_add(); });
    ws_server->register_command(PropertyEnum::SILENCE_RESET, [this] { _motion_control->silence_reset(); });
    ws_server->register_command(PropertyEnum::TEST, [this] { _motion_control->alarm_test(); });

    ws_server->register_data_request((PropertyEnum) SystemPacketTypeEnum::GET_CONFIG, _metadata->data.config);
    ws_server->register_command((PropertyEnum) SystemPacketTypeEnum::RESTART, [this] { _bootstrap->restart(); });

    mqtt_server->register_command(MQTT_TOPIC_PAUSE, [this] {
        if (_motion_control->state() != MotionState::SILENT) {
            _motion_control->silence_add();
        } else {
            _motion_control->silence_reset();
        }
    });

    mqtt_server->register_command(MQTT_TOPIC_SILENT, [this](const String &payload) {
        auto value = payload.toInt();
        _motion_control->silence_set(value > 0 ? value : _bootstrap->config().sys_config.silent_time / 1000);
    });

    mqtt_server->register_notification(MQTT_OUT_TOPIC_PAUSE, _metadata->generated.sensor_state_pause);
}

void Application::_handle_motion_sensor_event(MotionEventType type) {
    switch (type) {
        case MotionEventType::STATE_CHANGED:
            _bootstrap->ws_server()->send_notification(PropertyEnum::MOTION_STATE);
            _bootstrap->mqtt_server()->send_notification(MQTT_OUT_TOPIC_ALARM);
            _bootstrap->mqtt_server()->send_notification(MQTT_OUT_TOPIC_PAUSE);
            break;

        case MotionEventType::SILENCE_TIME_LEFT_CHANGED:
            _bootstrap->ws_server()->send_notification(PropertyEnum::MOTION_SILENCE_TIME_LEFT);
            break;
    }
}