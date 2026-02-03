#include <Arduino.h>
#include "modbus.h"

void setup() {
    // Start debug serial
    Serial.begin(9600);

    modbus_init();
    modbus_spindle_run();
}

void loop() {
    // Your main code here
    delay(1000);
}
