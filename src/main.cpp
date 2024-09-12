#include <LittleFS.h>

#include "application.h"

Application *app;

void setup() {
#ifdef DEBUG
    Serial.begin(115200);
    while (!Serial) delay(1);

    delay(2000);
#endif

    D_PRINT("Starting application...");

    if (!LittleFS.begin()) {
        D_PRINT("Unable to initialize FS");
    }

    app = new Application(&LittleFS);
    app->begin();
}

void loop() {
    app->event_loop();
}
