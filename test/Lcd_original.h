#pragma once

#include "SSD1306.h"

namespace lcd
{
#define LITTLE_LCD

#ifdef LITTLE_LCD
	int g_MaxWidth = 128;
	int g_MaxHeight = 32;
#endif

	SSD1306 *g_Display;
	bool g_LcdInitialized = false;

	bool initialized()
	{
		bool success = true;
		if (g_Display == nullptr)
		{
			AnnounceLine("Display is null!");
			success = false;
		}

		if (!g_LcdInitialized)
		{
			AnnounceLine("Display is not initialized!");
			success = false;
		}

		return success;
	}

	void init(uint8_t a_Sda, uint8_t a_Scl, bool a_FlipScreen = true, uint8_t a_Address = 0x3c)
	{
		SSD1306 *l_Display = new SSD1306(a_Address, a_Sda, a_Scl);
		l_Display->init();
		if (a_FlipScreen)
			l_Display->flipScreenVertically();
		l_Display->setFont(ArialMT_Plain_24);

		l_Display->drawStringMaxWidth(0, 1, 128, "Initialised");
		l_Display->display();
		delay(100);
		l_Display->displayOff();
		g_Display = l_Display;
		g_LcdInitialized = true;
	}

	void displayText(String a_Text, uint8_t duration = 0 /*s*/, int16_t x = 0, int16_t y = 1, uint16_t maxLineWidth = 128)
	{
		if (!initialized())
			return;

		g_Display->displayOn();
		g_Display->clear();
		if (a_Text.length() > 0)
		{
			g_Display->drawStringMaxWidth(x, y, maxLineWidth, a_Text);
		}
		g_Display->display();
		if (duration > 0)
		{
			delay(duration);
			g_Display->displayOff();
		}
	}

	bool radar = false;
	bool pir = false;

	void DisplayLines()
	{
		g_Display->displayOn();
		g_Display->clear();
		if (pir)
			g_Display->drawVerticalLine(1, 1, g_MaxHeight - 1);

		if (radar)
			g_Display->drawVerticalLine(g_MaxWidth - 1, 1, g_MaxHeight - 1);

		g_Display->display();
	}

	void DisplayLineRadar(bool movement)
	{
		radar = movement;
		DisplayLines();
	}

	void DisplayLinePir(bool movement)
	{
		pir = movement;
		DisplayLines();
	}

	void switchOff()
	{
		if (!initialized())
		{
			Serial.println("LcdScreen is not initialzed or null!");
			return;
		}

		g_Display->displayOff();
	}

	void runLcdScreenTest()
	{
		g_Display->displayOn();
		g_Display->clear();
		displayText("Test");
	}

} // namespace lcd