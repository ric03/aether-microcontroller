#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#include <iomanip>
#include <string>
#include <sstream>

#define DHTPIN 19     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT22 (ASAIR AM2302)

/**
 * DHT Sensor (temperature, humidity)
 * DHT22 (ASAIR AM2302)
 * 
 * This sensor reads the temperature and humidity:
 * - Humidity: Good for 0-100% humidity readings with 2-5% accuracy.
 * - Temperature: Good for -40 to 80°C temperature readings with ±0.5°C accuracy.
 */
namespace dht_sensor
{
    // Initialize DHT sensor.
    DHT dht(DHTPIN, DHTTYPE);

    struct Data
    {
        Data(float temperature, float humidity, float heatIndex)
        {
            this->temperature = temperature;
            this->humidity = humidity;
            this->heatIndex = heatIndex;
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
