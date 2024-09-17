#include <LittleFS.h>

#include "application.h"

Application *App;

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

    D_PRINT("Starting application...");

    if (!LittleFS.begin()) {
        D_PRINT("Unable to initialize FS");
    }

    App = new Application(&LittleFS);
    App->begin();
}

void loop() {
    App->event_loop();
}
