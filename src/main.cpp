#include "Arduino.h"
#include <driver/adc.h> //  ADC configuration
#include <array>

#include "dht_sensor.h"

// https://senseair.com/products/size-counts/s8-lp/ Product No. 004-0-0053
// Senseair Sensor UART pins
#define RXD2 13
#define TXD2 12

// SSD1306 Display i2c pins
#define DPORT 0x3c
#define DSDA 4
#define DSCL 15
#define DRST 16

void setup()
{
  Serial.begin(115200);
  dht_sensor::setup();
}

unsigned long nextTime = millis();
void loop()
{
  delay(2000);

  dht_sensor::Data data = dht_sensor::getData();
  dht_sensor::serialPrint(data);
}
