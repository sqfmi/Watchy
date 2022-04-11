#include "WatchyExpanded.h"

// ESP
#include <esp_sleep.h>

// Wire
#include <Wire.h>

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

CWatchyExpanded::CWatchyExpanded() : m_display(GxEPD2_154_D67(wcd::cs, wcd::dc, wcd::reset, wcd::busy))
{
}

void CWatchyExpanded::AddWatchFace(CWatchFace* pFace)
{
	m_faces.push_back(pFace);
}

void CWatchyExpanded::Init()
{
	const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
	Wire.begin(SDA, SCL); //init i2c
	m_rtc.init();

	// Init the display here for all cases, if unused, it will do nothing
	m_display.init(0, true, 10, true); // 10ms by spec, and fast pulldown reset
	m_display.epd2.setBusyCallback(DisplayBusyCallback);

	switch (wakeup_reason)
	{
		case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
			if(m_guiState == wc::kWatchFace_State)
			{
				m_rtc.read(m_currentTime);
				UpdateScreen(); //partial updates on tick
			}
			break;
	//	case ESP_SLEEP_WAKEUP_EXT1: //button Press
	//		handleButtonPress();
	//	break;
		default: //reset
			//RTC.config(datetime);
			//_bmaConfig();
			//RTC.read(currentTime);
			//showWatchFace(false); //full update on reset
		break;
	}
	DeepSleep();
}

void CWatchyExpanded::DisplayBusyCallback(const void*)
{
	gpio_wakeup_enable(static_cast<gpio_num_t>(wcd::busy), GPIO_INTR_LOW_LEVEL);
	esp_sleep_enable_gpio_wakeup();
	esp_light_sleep_start();
}

void CWatchyExpanded::UpdateScreen()
{
	m_display.setFullWindow();
	//drawWatchFace();
	DrawBasicClock(); // Temp

	m_display.display(true); //partial refresh
	m_guiState = wc::kWatchFace_State;
}

void CWatchyExpanded::DrawBasicClock()
{
	m_display.setFullWindow();
	m_display.fillScreen(GxEPD_BLACK);
	m_display.setFont(&FreeMonoBold9pt7b);
	m_display.setCursor(0, 17);

	tm currentLocalTime;
	currentLocalTime.tm_wday = m_currentTime.Wday - 1;
	currentLocalTime.tm_year = m_currentTime.Year + 1970 - 1900;
	currentLocalTime.tm_mon  = m_currentTime.Month - 1;
	currentLocalTime.tm_mday = m_currentTime.Day;
	currentLocalTime.tm_hour = m_currentTime.Hour;
	currentLocalTime.tm_min  = m_currentTime.Minute;
	currentLocalTime.tm_sec  = m_currentTime.Second;

	m_display.print("Time: ");
	char buffer[20];
	strftime(buffer, sizeof(buffer), " %I:%M %p", &currentLocalTime);
	m_display.println(buffer);

	strftime(buffer, sizeof(buffer), "%a %b %d, %Y", &currentLocalTime);
	m_display.print(buffer);
}

void CWatchyExpanded::DeepSleep()
{
	m_display.hibernate();
	m_rtc.clearAlarm(); //resets the alarm flag in the RTC

	for(int i=0; i<40; i++) // Set pins 0-39 to input to avoid power leaking out
		pinMode(i, INPUT);

	esp_sleep_enable_ext0_wakeup(wc::rtc_pin, 0); //enable deep sleep wake on RTC interrupt
	esp_sleep_enable_ext1_wakeup(wc::btn_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
	esp_deep_sleep_start();
}
