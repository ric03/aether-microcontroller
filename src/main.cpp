#include "Arduino.h"
// #include <driver/adc.h> //  ADC configuration

#include "dht_sensor.h"
#include "oled_display.h"
#include "button.h"

// https://senseair.com/products/size-counts/s8-lp/ Product No. 004-0-0053
// Senseair Sensor UART pins
#define RXD2 13
#define TXD2 12

void setup()
{
  Serial.begin(115200);

  button::setup();
  dht_sensor::setup();
  display::setup();
}

void loop()
{
  delay(2000);

  auto data = dht_sensor::getData();
  dht_sensor::serialPrint(data);

  Serial.printf("btn1 = %d | btn2 = %d \n", button::isBtn1Pressed(), button::isBtn2Pressed());
}
