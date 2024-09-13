#pragma once

#include <Arduino.h>
#include <memory>

#include <lib/utils/enum.h>

#include "type.h"
#include "misc/button.h"
#include "misc/led.h"
#include "misc/buzzer.h"
#include "sys_constants.h"

MAKE_ENUM(State, uint8_t,
          IDLE, 0,
          PANIC, 1,
          SILENT, 2
)

class MotionControl {
    std::unique_ptr<Button> _btn = nullptr;
    std::unique_ptr<Buzzer> _buzzer = nullptr;
    std::unique_ptr<Led> _led = nullptr;

    const Config &_config;

public:
    explicit MotionControl(const Config &config);

    void begin();
    void tick();

private:
    State _state = State::IDLE;
    unsigned long _last_time = 0;

    unsigned long _silence_time = 0;
    uint8_t _silence_level = 0;

    void _button_clicked(uint8_t count);
    void _button_hold(uint8_t);

    void _change_state(State next_state);

    void _state_machine(unsigned long time);
};
