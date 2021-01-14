#pragma once

#include <Arduino.h>

#define BTN_1 34
#define BTN_2 18

namespace button
{
    void setup()
    {
        pinMode(BTN_1, INPUT_PULLUP);
        pinMode(BTN_2, INPUT_PULLUP);
    }

    int isBtn1Pressed()
    {
        return analogRead(BTN_1);
    }

    bool isBtn2Pressed()
    {
        return digitalRead(BTN_2) == LOW;
    }

} // namespace button