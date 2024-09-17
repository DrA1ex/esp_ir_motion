#pragma once

#include <Arduino.h>
#include <memory>

#include <lib/utils/enum.h>

#include "type.h"
#include "misc/button.h"
#include "misc/led.h"
#include "misc/buzzer.h"
#include "sys_constants.h"

enum class MotionEventType {
    STATE_CHANGED,
    SILENCE_TIME_LEFT_CHANGED,
};

class MotionControl {
    EventTopic<MotionEventType> _event;

    std::unique_ptr<Button> _btn = nullptr;
    std::unique_ptr<Buzzer> _buzzer = nullptr;
    std::unique_ptr<Led> _led = nullptr;

    const Config &_config;
public:
    explicit MotionControl(const Config &config);

    void begin();
    void tick();

    void silence_add();
    void silence_reset();
    void alarm_test();

    inline MotionState state() { return _state; }
    inline unsigned long silence_time_left() { return _silence_time; }

    inline EventTopic<MotionEventType> &event() { return _event; };

private:
    MotionState _state = MotionState::PAUSED;
    unsigned long _last_time = 0;

    unsigned long _silence_time = 0;
    uint8_t _silence_level = 0;

    void _button_clicked(uint8_t count);
    void _button_hold(uint8_t);

    void _change_state(MotionState next_state);

    void _state_machine(unsigned long time);
};
