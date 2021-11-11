#pragma once
#include "Arduino.h"

namespace mqtt_message_generator
{
    String generateJson(String measurement, String value, String unit)
    {
        return "{\"measurement\":\"" + measurement + "\",\"value\":" + value + ",\"unit\":\"" + unit + "\"}";
    }

    /**
     * @brief Influx Line Protocol
     * https://docs.influxdata.com/influxdb/cloud/reference/syntax/line-protocol/
     */
    String generateInfluxLineProtocol(String measurement, String value, String unit)
    {
        return measurement + ",unit=" + unit + " fieldKey=" + value;
    }

    String generateTemperaturePayload(float value)
    {
        // return generateJson("temperature", String(value, 1), "C");
        return generateInfluxLineProtocol("temperature", String(value, 1), "C");
    }

    String generateHumidityPayload(float value)
    {
        // return generateJson("humidity", String(value, 1), "%");
        return generateInfluxLineProtocol("humidity", String(value, 1), "%");
    }

    String generateCo2Payload(uint32_t value)
    {
        // return generateJson("co2", String(value), "ppm");
        return generateInfluxLineProtocol("co2", String(value), "ppm");
    }
}