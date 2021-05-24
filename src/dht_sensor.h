#pragma once

/**
 *  DHT Sensor (temperature, humidity)
 */

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <iomanip>
#include <string>
#include <sstream>

#define DHTPIN 19     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT22 (ASAIR AM2302)

namespace dht_sensor
{
    // Initialize DHT sensor.
    DHT dht(DHTPIN, DHTTYPE);

    struct Data
    {
        Data(float t, float h, float hi)
        {
            temperature = t;
            humidity = h;
            heatIndex = hi;
        }
        float temperature;
        float humidity;
        float heatIndex;
    };

    void setup()
    {
        dht.begin();
    }

    Data getData()
    {
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();
        float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

        return Data(temperature, humidity, heatIndex);
    }

    void printSerial(Data &d)
    {
        Serial.printf("Temp: %.1f °C , Humidity: %.1f %%, HeatIndex: %.1f °C", d.temperature, d.humidity, d.heatIndex);
    }

} // namespace dht_sensor
