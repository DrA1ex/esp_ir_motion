#pragma once

#include <LittleFS.h>

#include "lib/bootstrap.h"

#include "metadata.h"
#include "misc/motion.h"

class Application {
    Bootstrap<Config, PropertyEnum> *_bootstrap = nullptr;
    MotionControl *_motion_control = nullptr;
    std::unique_ptr<ConfigMetadata> _metadata = nullptr;

    bool _initialized = false;

public:
    void begin();
    void event_loop();

private:
    void _setup();
    void _handle_motion_sensor_event(MotionEventType type);
};
