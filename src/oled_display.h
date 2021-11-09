#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "bitmap_ms_paint.h"
#include "bitmap_dog_bone.h"

#define SCREEN_WIDTH 128         // OLED display width, in pixels
#define SCREEN_HEIGHT 32         // OLED display height, in pixels
#define OLED_RESET -1            // Reset pin # (or -1 if sharing Arduino reset pin)
#define TEXT_COLOR SSD1306_WHITE // Draw white text (default is black), because the display is black

namespace display
{
    void println(const String &s);
    void print_bitmap_128x32(const uint8_t bitmap[]);

    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    void setup()
    {
        if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Address 0x3C for 128x32
        {
            Serial.println(F("SSD1306 allocation failed"));
            for (;;)
                ; // Don't proceed, loop forever
        }

        display.setTextColor(TEXT_COLOR);
        display.clearDisplay();
        display.display();
    }

    void show_sample_images()
    {
        println("It works :D");
        delay(2000);

        display.clearDisplay();
        print_bitmap_128x32(bitmap_ms_paint);
        delay(2000);

        display.clearDisplay();
        print_bitmap_128x32(bitmap_dog_bone);
        delay(2000);

        display.clearDisplay();
        display.display();
        delay(2000);
    }

    void println(const String &s)
    {
        display.setTextSize(1);  // Normal 1:1 pixel scale
        display.setCursor(0, 0); // Start at top-left corner
        display.println(s);
        display.display();
    }

    void print_bitmap_128x32(const uint8_t bitmap[])
    {
        display.drawBitmap(0, 0, bitmap, 128, 32, SSD1306_WHITE);
        display.display();
    }

} // namespace display
