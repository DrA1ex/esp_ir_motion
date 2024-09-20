#pragma once

#include <Arduino.h>

#include "sys_constants.h"

struct Note {
    int frequency;
    uint16_t duration;
};

class Buzzer {
    uint8_t _pin;
    Note *_melody;
    uint16_t _melody_length;

    bool _initialized = false;
    bool _playing = false;
    unsigned long _start_time = 0;
    size_t _current_note = 0;

    unsigned long _last_tone_update = 0;

public:
    explicit Buzzer(uint8_t pin, Note *melody, uint16_t length);

    void begin();
    void tick(unsigned long time);

    void play();
    void stop();

    [[nodiscard]] inline bool initialized() const { return _initialized; }
};
