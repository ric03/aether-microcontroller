#include "Arduino.h"
// #include <driver/adc.h> //  ADC configuration

#include "button.h"
#include "co2_sensor.h"
#include "dht_sensor.h"
#include "oled_display.h"
#include "mqtt.h"

#define DEGREE_SYMBOL 248

void setup()
{
  Serial.begin(115200);

  button::setup();
  co2_sensor::setup();
  dht_sensor::setup();
  display::setup();
  mqtt::setup();
}

void loop()
{
  delay(2000);

  auto dht_data = dht_sensor::getData();
  uint32_t co2 = co2_sensor::readCo2();

#if ENABLE_SERIAL_LOGGING == true
  Serial.printf("Temp: %.1f °C, Humidity: %.1f %%, HeatIndex: %.1f °C -- CO2: %d ppm\n",
                dht_data.temperature,
                dht_data.humidity,
                dht_data.heatIndex,
                co2);
#endif

#if ENABLE_MQTT == true
  mqtt::mqttClient.publish("sensors/home/livingroom/temperature", 0, true, String(dht_data.temperature, 1).c_str());
  mqtt::mqttClient.publish("sensors/home/livingroom/humidity", 0, true, String(dht_data.humidity, 1).c_str());
  mqtt::mqttClient.publish("sensors/home/livingroom/co2", 0, true, String(co2).c_str());
#endif

#if ENABLE_DISPLAY == true
  display::display.clearDisplay();
  display::display.setCursor(0, 0);
  display::display.cp437(true);
  display::display.printf("Temperature: %.1f %cC\n", dht_data.temperature, DEGREE_SYMBOL);
  display::display.printf("Humidity: %.1f %%\n", dht_data.humidity);
  display::display.printf("CO2: %d ppm", co2);
  display::display.display();
#endif
  // Serial.printf("btn1 = %d | btn2 = %d \n", button::isBtn1Pressed(), button::isBtn2Pressed());
}
