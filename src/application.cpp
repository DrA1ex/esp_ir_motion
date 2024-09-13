#include "application.h"

#include <ArduinoOTA.h>
#include <LittleFS.h>
#include <lib/utils/qr.h>

Application::Application(FS *fs) : __int::AppType(PropertyMetaMap),
                                   _fs(fs), _config_storage(_timer, "config") {}

void Application::begin() {
    _config_storage.begin(_fs);

    _wifi_manager = std::make_unique<WifiManager>(sys_config().wifi_ssid, sys_config().wifi_password);
    _packet_handler = std::make_unique<AppPacketHandler>(*this);
    _ws_server = std::make_unique<WebSocketServer<__int::AppType>>(*this, *_packet_handler);
    _mqtt_server = std::make_unique<MqttServer<__int::AppType>>(*this);

    _motion_control = std::make_unique<MotionControl>(config());

    event_property_changed().subscribe(this, [this](auto sender, auto type, auto) {
        if (sender != this) _handle_property_change(type);
    });

    _timer.add_interval([this](auto) {
        _ws_server->notify_clients(-1, PropertyEnum::MOTION_STATE, motion_control().state());
        _ws_server->notify_clients(-1, PropertyEnum::MOTION_SILENCE_TIME_LEFT,
                                   motion_control().silence_time_left());
    }, 10000);

    _motion_control->event().subscribe(this, [this](auto, auto type, auto) {
        switch (type) {
            case MotionEventType::STATE_CHANGED:
                return _ws_server->notify_clients(-1, PropertyEnum::MOTION_STATE, motion_control().state());
            case MotionEventType::SILENCE_TIME_LEFT_CHANGED:
                return _ws_server->notify_clients(-1, PropertyEnum::MOTION_SILENCE_TIME_LEFT, motion_control().silence_time_left());
        }
    });
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

            _ws_server->handle_incoming_data();
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

void Application::_handle_property_change(PropertyEnum type) {
    update();
}


Response AppPacketHandler::handle_packet_data(uint32_t client_id, const Packet<PacketHandler<Application>::PacketEnumT> &packet) {
    auto &app = (Application &) this->app();

    if (packet.header->type == PropertyEnum::SILENCE) {
        app.motion_control().silence_add();
        return Response::ok();
    } else if (packet.header->type == PropertyEnum::SILENCE_RESET) {
        app.motion_control().silence_reset();
        return Response::ok();
    } else if (packet.header->type == PropertyEnum::GET_STATE) {
        static AppState state_obj;
        state_obj = {
                app.motion_control().state(),
                app.motion_control().silence_time_left(),
        };

        return protocol().serialize(state_obj);
    }

    return PacketHandler::handle_packet_data(client_id, packet);
}
