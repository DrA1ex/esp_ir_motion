#include <LittleFS.h>

#include "application.h"

Application *App;

void setup() {
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) delay(1);

    delay(2000);
#endif

    analogWriteResolution(8);
    analogWriteFrequency(22000);

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
