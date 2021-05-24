#pragma once

/**
 * Senseair Sensor
 * 
 * Product No. 004-0-0053
 * https://senseair.com/products/size-counts/s8-lp/
 */
#include <Arduino.h>

//Senseair Sensor UART pins
#define RXD2 13
#define TXD2 12

namespace co2_sensor
{
    // Example Implementation:
    // https://github.com/airgradienthq/arduino/blob/master/AirGradient.cpp#L609

    byte CO2_request_sequence[] = {0xFE, 0x04, 0x00, 0x03, 0x00, 0x01, 0xD5, 0xC5};

    auto &SerialCO2 = Serial1; // TODO create custom HardwareSerial serialCO2(3) ?

    void setup()
    {
        SerialCO2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    }

    inline int isSerialEmptyOrUnavailable() { return SerialCO2.available() == 0; }
    inline int getMessageCount() { return SerialCO2.available(); }

    uint32_t readCo2()
    {
        int timeout = 10;
        while (isSerialEmptyOrUnavailable())
        {
            timeout--;
            if (timeout == 0)
            {
                return -1;
            }

            SerialCO2.write(CO2_request_sequence, 8);
            delay(50);
        }

        int timeout = 10;
        while (getMessageCount() < 7)
        {
            timeout--;
            if (timeout == 0)
            {
                while (SerialCO2.available())
                {
                    SerialCO2.read();
                }
                break;
            }
            delay(50);
        }

        byte response[7] = {};
        for (int i = 0; i < 7; i++)
        {
            response[i] = SerialCO2.read();
        }

        uint32_t high = response[3];
        uint32_t low = response[4];
        return (high << 8) + low;
    }

} // namespace co2_sensor