#include "Arduino.h"
#include <driver/adc.h> //  ADC configuration
#include <array>

//Senseair Sensor UART pins
#define RXD2 13
#define TXD2 12

// SSD1306 Display i2c pins
#define DPORT 0x3c
#define DSDA 4
#define DSCL 15
#define DRST 16

byte CO2req[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};
byte CO2out[] = {0, 0, 0, 0, 0, 0, 0};

// Code from https://github.com/liutyi/esp32-oled-senseair/blob/master/ttgo_simple_co2meter.ino
void RequestCO2()
{
    while (!Serial1.available())
    {
        Serial1.write(CO2req, 7);
        delay(50);
    }

    int timeout = 0;
    while (Serial1.available() < 7)
    {
        timeout++;
        if (timeout > 10)
        {
            while (Serial1.available())
                Serial1.read();
            break;
        }
        delay(50);
    }

    for (int i = 0; i < 7; i++)
    {
        CO2out[i] = Serial1.read();
    }
}

unsigned long CO2count()
{
    int high = CO2out[3];
    int low = CO2out[4];
    unsigned long val = high * 256 + low;
    return val * 1; // S8 = 1. K-30 3% = 3, K-33 ICB = 10
}

void setup()
{
    Serial.begin(115200);
    Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
    // Reset and setup display
    pinMode(DRST, OUTPUT);
    digitalWrite(DRST, 1);
}

unsigned long nextTime = millis();
void loop()
{
    initializer.tick();

    if (nextTime < millis())
    {
        nextTime = millis() + 1000;
        Serial.println("Collection measurements...");
        RequestCO2();
        unsigned long CO2 = CO2count();
        if (CO2 > 5)
        {
            float temperature = temp::readTemperature();
            float humidity = temp::getHumidity();

            Serial.println("CO2: ", CO2, ", temperature: ", temperature, ", humidity: ", humidity);

            //initializer.publish("airsensor/co2", CO2);
            //initializer.publish("airsensor/temperature", temperature);
            //initializer.publish("airsensor/humidity", humidity);
        }
    }
}