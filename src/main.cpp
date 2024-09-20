#include <LittleFS.h>

#include "application.h"

Application ApplicationInstance;

void setup() {
#ifdef DEBUG
    Serial.begin(115200);

    {
        auto start_t = millis();
        while (!Serial && (millis() - start_t) < 15000ul) delay(1);
    }

    delay(2000);
#endif

    analogWriteResolution(PWM_RESOLUTION);

#ifdef ARDUINO_ARCH_ESP8266
    analogWriteFreq(PWM_FREQUENCY);
#else
    analogWriteFrequency(PWM_FREQUENCY);
#endif

    ApplicationInstance.begin();
}

void loop() {
    ApplicationInstance.event_loop();
}
