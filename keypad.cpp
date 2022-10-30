#include "keypad.h"
#include "arduino.h"

const int C1 = 14;
const int C2 = 15;
const int C3 = 16;
const int R1 = 17;
const int R2 = 18;
const int R3 = 19;
const int R4 = 20;

const int debounceMs = 150;

Keypad::Keypad()
{
    pinMode(C1, INPUT_PULLUP);
    pinMode(C2, INPUT_PULLUP);
    pinMode(C3, INPUT_PULLUP);
    pinMode(R1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(R3, OUTPUT);
    pinMode(R4, OUTPUT);
    digitalWrite(R1, HIGH);
    digitalWrite(R2, HIGH);
    digitalWrite(R3, HIGH);
    digitalWrite(R4, HIGH);

    lastPressMs = millis();

    Serial.begin(115200);
}

int Keypad::sample()
{
    // debounce
    if (millis() - lastPressMs < debounceMs)
    {
        return lastPress;
    }
    lastPressMs = millis();

    int row = -1;

    row = sampleRow(R1);
    if (row != -1)
    {
        lastPress = row;
        return lastPress;
    }
    row = sampleRow(R2);
    if (row != -1)
    {
        lastPress = row + 3;
        return lastPress;
    }
    row = sampleRow(R3);
    if (row != -1)
    {
        lastPress = row + 6;
        return lastPress;
    }
    row = sampleRow(R4);
    if (row != -1)
    {
        lastPress = row + 9;
        return lastPress;
    }
    lastPress = -1;
    return lastPress;
}

int Keypad::sampleRow(int rowPin)
{
    int result = -1;
    digitalWrite(rowPin, LOW);
    if (digitalRead(C1) == LOW)
    {
        result = 0;
    }
    else if (digitalRead(C2) == LOW)
    {
        result = 1;
    }
    else if (digitalRead(C3) == LOW)
    {
        result = 2;
    }
    digitalWrite(rowPin, HIGH);
    return result;
}