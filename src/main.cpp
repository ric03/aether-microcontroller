#include "Arduino.h"
#include <driver/adc.h> //  ADC configuration
#include <array>

// DHT Sensor (temperature, humidity)
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// DHT Sensor (temperature, humidity)
#define DHTPIN 19     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22 // DHT22 (AM2302)

// https://senseair.com/products/size-counts/s8-lp/ Product No. 004-0-0053
// Senseair Sensor UART pins
#define RXD2 13
#define TXD2 12

// SSD1306 Display i2c pins
#define DPORT 0x3c
#define DSDA 4
#define DSCL 15
#define DRST 16

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  dht.begin();
}

unsigned long nextTime = millis();
void loop()
{
  delay(2000);

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  Serial.printf("Temp: %f °C, Humidity: %f %%, HeatIndex: %f °C", temperature, humidity, heatIndex);
  Serial.println();
}
