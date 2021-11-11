#include <WiFi.h>
#include "config.h"

namespace wifi
{
    void setup()
    {
        Serial.println("Connecting to Wi-Fi...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}