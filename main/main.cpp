#include "Arduino.h"

#define LED 12

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
    }

    pinMode(LED, OUTPUT);

    Serial.println("Setup complete");
}

void loop()
{
    Serial.println("Loop");

    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
    delay(900);
}
