#include "Arduino.h"

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }

    Serial.println("Setup complete");
}

void loop()
{
    Serial.println("Loop");
    delay(1000);
}
