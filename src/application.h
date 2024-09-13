#pragma once

#include <DNSServer.h>
#include <lib/base/application.h>
#include <lib/misc/storage.h>
#include <lib/network/wifi.h>
#include <lib/network/web.h>
#include <lib/network/protocol/packet_handler.h>
#include <lib/network/server/ws.h>
#include <lib/network/server/mqtt.h>

#include "type.h"
#include "metadata.h"
#include "constants.h"
#include "misc/motion.h"

namespace __int {
    using AppType = AbstractApplication<Config, AppPropertyMeta>;
}

class Application : public __int::AppType {
    enum class ApplicationState {
        UNINITIALIZED,
        WIFI_CONNECT,
        INITIALIZING,
        READY,
    };

    std::unique_ptr<WifiManager> _wifi_manager = nullptr;

    std::unique_ptr<PacketHandler<__int::AppType>> _packet_handler = nullptr;
    std::unique_ptr<WebSocketServer<__int::AppType>> _ws_server = nullptr;
    std::unique_ptr<MqttServer<__int::AppType>> _mqtt_server = nullptr;

    FS *_fs;
    Timer _timer;
    Storage<Config> _config_storage;
    WebServer _web_server{};

    ApplicationState _state = ApplicationState::UNINITIALIZED;
    std::unique_ptr<DNSServer> _dns_server = nullptr;

    std::unique_ptr<MotionControl> _motion_control = nullptr;

public:
    explicit Application(FS *fs);

    inline Config &config() override { return _config_storage.get(); }
    inline SysConfig &sys_config() { return config().sys_config; }

    void begin();
    void event_loop();

    void update();

    void restart() override;

private:
    void _after_init();
    void _handle_property_change(PropertyEnum type);
};
