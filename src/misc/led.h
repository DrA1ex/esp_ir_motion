#pragma once

#include <Arduino.h>

#include "sys_constants.h"

union Color {
    struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    } components;

    uint32_t value;
};

class Led {
    uint8_t _r_pin, _g_pin, _b_pin;

    Color _color = {.value = 0};

    bool _active = false;

    bool _continuously = false;
    uint8_t _blink_count = 0;
    uint8_t _blink_count_left = 0;
    unsigned long _flash_duration = 0;

    unsigned long _start_time = 0;

    void _refresh_color(bool active);

    bool _initialized = false;

public:
    Led(uint8_t r_pin, uint8_t g_pin, uint8_t b_pin);

    void begin();

    void set_color(const Color &color);
    void set_color(uint8_t r, uint8_t g, uint8_t b);

    void blink(uint8_t count = 1, bool continuously = false);
    void flash(unsigned long duration = 0);

    void turn_off();

    void tick(unsigned long time);

    [[nodiscard]] inline bool initialized() const { return _initialized; }
    [[nodiscard]] inline bool active() const { return _active; }
    [[nodiscard]] inline uint8_t blink_count() const { return _blink_count; }
    [[nodiscard]] inline unsigned long flash_duration() const { return _flash_duration; }
};
