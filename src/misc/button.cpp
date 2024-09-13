#include "button.h"

Button::Button(uint8_t pin) : _pin(pin) {}

void Button::begin() {
    pinMode(_pin, INPUT);

    attachInterruptArg(digitalPinToInterrupt(_pin), _handle_interrupt, this, CHANGE);
}

void Button::_handle_interrupt(void *arg) {
    auto self = (Button *) arg;
    self->_handle_interrupt();
}

void Button::_handle_interrupt() {
    auto silence_time = millis() - _last_impulse_time;
    _last_impulse_time = millis();

    if (silence_time < BTN_SILENCE_INTERVAL) return;

    if (digitalRead(_pin)) {
        _handle_rising_interrupt();
    } else {
        _handle_falling_interrupt();
    }
}


void Button::_handle_rising_interrupt() {
    const auto delta = millis() - _last_impulse_time;
    if (delta > BTN_RESET_INTERVAL) {
        _click_count = 0;
    }
}


void Button::_handle_falling_interrupt() {
    if (_hold) {
        _hold = false;
        _click_count = 0;
    } else {
        _click_count++;
    }
}


void Button::handle() {
    unsigned long delta = millis() - _last_impulse_time;

    const bool state = digitalRead(_pin);
    if (!_hold && state && delta >= BTN_HOLD_INTERVAL) {
        _hold = true;
        _click_count++;
    } else if (_click_count && !_hold && delta >= BTN_RESET_INTERVAL) {
        _click_count = 0;
    } else if (_hold && !state) {
        _hold = false;
        _click_count = 0;
    }

    if (_hold) {

        if (_hold_handler != nullptr) {
            _hold_handler(_click_count);
        }
    } else if (_click_count && delta > BTN_PRESS_WAIT_INTERVAL) {

        if (_click_handler != nullptr) {
            _click_handler(_click_count);
        }

        _click_count = 0;
    }
}
