#include "led.h"

Led::Led(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin) : _r_pin(r_pin), _g_pin(g_pin), _b_pin(b_pin) {}

void Led::begin() {
    pinMode(_r_pin, OUTPUT);
    pinMode(_g_pin, OUTPUT);
    pinMode(_b_pin, OUTPUT);

    _initialized = true;
}

void Led::_refresh_color(bool active) {
    if (!_initialized) return;

    analogWrite(_r_pin, active ? _color.components.red : 0);
    analogWrite(_g_pin, active ? _color.components.green : 0);
    analogWrite(_b_pin, active ? _color.components.blue : 0);
}

void Led::set_color(const Color &color) {
    set_color(color.components.red, color.components.green, color.components.blue);
}

void Led::set_color(uint8_t r, uint8_t g, uint8_t b) {
    _color.components.red = r;
    _color.components.green = g;
    _color.components.blue = b;

    _refresh_color(_active);
}

void Led::flash(unsigned long duration) {
    if (!_initialized || (_active && _blink_count == 0)) return;

    _active = true;
    _start_time = millis();

    _flash_duration = duration;
    _blink_count = 0;
    _blink_count_left = 0;
    _continuously = false;

    _refresh_color(true);
}

void Led::blink(uint8_t count, bool continuously) {
    if (!_initialized) return;

    if (_active && count == 0) {
        turn_off();
    } else if (_active) {
        _continuously = continuously;
        _blink_count = count;
        _blink_count_left = min(_blink_count_left, _blink_count);
    } else {
        _active = true;
        _start_time = millis();

        _flash_duration = 0;
        _blink_count = count;
        _blink_count_left = count;
        _continuously = continuously;

        _refresh_color(true);
    }
}

void Led::turn_off() {
    if (!_initialized || !_active) return;

    _active = false;
    _refresh_color(false);
}

void Led::tick(unsigned long time) {
    if (!_initialized || !_active) return;

    auto delta = time - _start_time;

    if (_blink_count_left > 0) {    // Blink Mode
        if (delta < LED_BLINK_ACTIVE_DURATION) {
            _refresh_color(true);
        } else {
            _refresh_color(false);

            if (delta - LED_BLINK_ACTIVE_DURATION > LED_BLINK_WAIT_DURATION) {
                _start_time = time;
                if (--_blink_count_left == 0 && !_continuously) turn_off();
            }
        }
    } else if (_blink_count > 0 && _continuously) {     // Blink Mode Cool down
        if (delta > LED_BLINK_COOL_DOWN_DURATION) {
            _start_time = time;
            _blink_count_left = _blink_count;
        }
    } else {    // Flash Mode
        if (_flash_duration > 0 && delta < _flash_duration) {
            turn_off();
        }
    }
}
