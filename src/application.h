#pragma once

#include <DNSServer.h>
#include <lib/misc/storage.h>
#include <lib/network/wifi.h>
#include <lib/network/web.h>
#include <lib/network/server/ws.h>
#include <lib/network/server/mqtt.h>

#include "type.h"
#include "constants.h"
#include "metadata.h"
#include "misc/motion.h"

enum class ApplicationState {
    UNINITIALIZED,
    WIFI_CONNECT,
    INITIALIZING,
    READY,
};

class Application {
    std::unique_ptr<WifiManager> _wifi_manager = nullptr;

    std::unique_ptr<WebSocketServer<PropertyEnum>> _ws_server = nullptr;
    std::unique_ptr<MqttServer> _mqtt_server = nullptr;

    FS *_fs;
    Timer _timer;
    Storage<Config> _config_storage;
    WebServer _web_server{};

    ApplicationState _state = ApplicationState::UNINITIALIZED;
    std::unique_ptr<DNSServer> _dns_server = nullptr;

    std::unique_ptr<MotionControl> _motion_control = nullptr;

    std::unique_ptr<ConfigMetadata> _metadata;

public:
    explicit Application(FS *fs);

    inline Config &config() { return _config_storage.get(); }
    inline SysConfig &sys_config() { return config().sys_config; }

    inline MotionControl &motion_control() { return *_motion_control; }

    void begin();
    void event_loop();

    void update();
    void restart();

private:
    void _after_init();
    void _handle_motion_sensor_event(MotionEventType type);
};
