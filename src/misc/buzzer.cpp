#include "buzzer.h"

Buzzer::Buzzer(uint8_t pin, Note *melody, uint16_t length) : _pin(pin), _melody(melody), _melody_length(length) {}

void Buzzer::begin() {
    _initialized = true;

    pinMode(_pin, OUTPUT);
}

void Buzzer::play() {
    if (!_initialized || _playing) return;

    _start_time = millis();
    _playing = true;
    _current_note = 0;
}

void Buzzer::stop() {
    if (!_initialized || !_playing) return;

    _playing = false;
    noTone(_pin);
}

void Buzzer::tick(unsigned long time) {
    if (!_initialized || !_playing) return;
    if (time - _last_tone_update < BUZZER_TONE_INTERVAL) return;

    _last_tone_update = time;
    auto delta = time - _start_time;
    auto cursor = _melody[_current_note];

    if (delta >= cursor.duration) {
        _start_time = time - (delta - cursor.duration);
        delta = time - _start_time;

        _current_note = (_current_note + 1) % _melody_length;
        cursor = _melody[_current_note];
    }

    auto next_cursor = _melody[(_current_note + 1) % _melody_length];

    auto freq_diff = next_cursor.frequency - cursor.frequency;
    auto new_freq = cursor.frequency + freq_diff * (long) delta / cursor.duration;

    tone(_pin, (int) new_freq);
}
