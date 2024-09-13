#include "motion.h"

#include "lib/debug.h"

Note Melody[] = {
        {650,  250},
        {1350, 250},
};

MotionControl::MotionControl(const Config &config) : _config(config) {}

void MotionControl::begin() {
    auto motion_cfg = _config.motion_config;

    _buzzer = std::make_unique<Buzzer>(
            motion_cfg.buzzer_pin, Melody, sizeof(Melody) / sizeof(Melody[0]));

    _led = std::make_unique<Led>(motion_cfg.led_r_pin,
                                 motion_cfg.led_g_pin,
                                 motion_cfg.led_b_pin);

    _btn = std::make_unique<Button>(motion_cfg.button_pin);

    pinMode(motion_cfg.motion_pin, INPUT);

    if (motion_cfg.led_enabled) _led->begin();
    if (motion_cfg.buzzer_enabled) _buzzer->begin();
    if (motion_cfg.button_enabled) {
        _btn->begin();
        _btn->set_on_click(std::bind(&MotionControl::_button_clicked, this, std::placeholders::_1));
        _btn->set_on_hold(std::bind(&MotionControl::_button_hold, this, std::placeholders::_1));
    }

    D_PRINT("Motion Control: Initialized");
}

void MotionControl::_button_clicked(uint8_t count) {
    D_PRINTF("Motion Control: Button clicked: %i\r\n", count);

    if (count <= _silence_level) return;

    _silence_time = count * _config.sys_config.silent_time;
    _silence_level = count;

    _change_state(State::SILENT);
}

void MotionControl::_button_hold(uint8_t) {
    if (_state != State::SILENT) return;

    D_PRINT("Motion Control: Button reset");
    _change_state(State::IDLE);
}
void MotionControl::_change_state(State next_state) {
    if (next_state == _state) return;

    if (_state == State::PANIC) {
        _buzzer->stop();
    } else if (_state == State::SILENT) {
        _silence_time = 0;
        _silence_level = 0;
    }

    if (next_state == State::PANIC) {
        _buzzer->play();
        _led->flash();
    } else if (next_state == State::SILENT) {
        _led->set_color(0, 3, 0);
        _led->flash(LED_BLINK_COOL_DOWN_DURATION);
    } else {
        _led->turn_off();
    }

    D_PRINTF("Motion Control: change state %s -> %s\r\n", __debug_enum_str(_state), __debug_enum_str(next_state));

    _state = next_state;
    _last_time = millis();
}
void MotionControl::_state_machine(unsigned long time) {
    auto delta = time - _last_time;

    switch (_state) {
        case State::IDLE:
            if (digitalRead(_config.motion_config.motion_pin) == HIGH) {
                _change_state(State::PANIC);
            }
            break;

        case State::PANIC:
            if (delta >= _config.sys_config.buzz_time && digitalRead(_config.motion_config.motion_pin) == LOW) {
                _change_state(State::IDLE);
            }
            break;

        case State::SILENT:
            if (delta >= _silence_time) {
                _change_state(State::IDLE);
            }
            break;
    }
}

void MotionControl::tick() {
    auto time = millis();

    if (!_config.power) {
        _change_state(State::IDLE);
        return;
    }

    if (_config.motion_config.button_enabled) _btn->handle();
    _state_machine(time);

    auto delta = time - _last_time;
    switch (_state) {
        case State::IDLE:
            if (delta >= _config.sys_config.idle_flash_time) {
                _last_time = time;

                _led->set_color(64, 0, 0);
                _led->blink();
            }
            break;

        case State::PANIC:
            if ((delta / _config.sys_config.panic_color_time) % 2) {
                _led->set_color(255, 0, 0);
            } else {
                _led->set_color(0, 0, 255);
            }
            break;

        case State::SILENT: {
            auto level = (int) ((_silence_time - delta - 1) / _config.sys_config.silent_time) + 1;

            if (!_led->active() || _led->blink_count() != level) {
                D_PRINTF("Motion Control: time left (sec) %lu\r\n", (_silence_time - delta) / 1000);

                _led->blink(level, true);
            }
        }
            break;
    }

    if (_config.motion_config.buzzer_enabled) _buzzer->tick(time);
    if (_config.motion_config.led_enabled) _led->tick(time);
}

void MotionControl::silence_add() {
    D_PRINT("Motion Control: Add Silence");
    _change_state(State::SILENT);

    _silence_level += 1;
    _silence_time = _silence_level * _config.sys_config.silent_time;
}

void MotionControl::silence_reset() {
    D_PRINT("Motion Control: Reset Silence");
    _change_state(State::IDLE);
}
