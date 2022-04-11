#include "WatchyExpanded.h"

// ESP
#include <esp_sleep.h>

// Wire
#include <Wire.h>

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

// Expanded
#include "WatchFace.h"

RTC_DATA_ATTR bool g_displayFullInit = true;

class CBasicWatch : public CWatchFace
{
	public:
		void Draw(CWatchyExpanded::ADisplay& display, const tmElements_t& time) override
		{
			display.setFullWindow();
			display.fillScreen(GxEPD_BLACK);
			display.setFont(&FreeMonoBold9pt7b);
			display.setCursor(0, 15);

			tm currentLocalTime;
			currentLocalTime.tm_wday = time.Wday - 1;
			currentLocalTime.tm_year = time.Year + 1970 - 1900;
			currentLocalTime.tm_mon  = time.Month - 1;
			currentLocalTime.tm_mday = time.Day;
			currentLocalTime.tm_hour = time.Hour;
			currentLocalTime.tm_min  = time.Minute;
			currentLocalTime.tm_sec  = time.Second;

			display.print("Time: ");
			char buffer[20];
			strftime(buffer, sizeof(buffer), " %I:%M %p", &currentLocalTime);
			display.println(buffer);

			strftime(buffer, sizeof(buffer), "%a %b %d, %Y", &currentLocalTime);
			display.print(buffer);
		};
};

CWatchyExpanded::CWatchyExpanded() : m_display(GxEPD2_154_D67(wcd::cs, wcd::dc, wcd::reset, wcd::busy))
{
	AddWatchFace(new CBasicWatch);
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
	m_display.init(0, g_displayFullInit, 10, true); // 10ms by spec, and fast pulldown reset
	m_display.epd2.setBusyCallback(DisplayBusyCallback);

	switch (wakeup_reason)
	{
		case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
			if(m_guiState == wc::kWatchFace_State)
			{
				m_rtc.read(m_currentTime);
				UpdateScreen(false); //partial updates on tick
			}
			break;
	//	case ESP_SLEEP_WAKEUP_EXT1: //button Press
	//		handleButtonPress();
	//	break;
		default: //reset
			m_rtc.config("");
			//_bmaConfig();
			m_rtc.read(m_currentTime);
			UpdateScreen(true); //full update on reset
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

void CWatchyExpanded::UpdateScreen(const bool fullUpdate)
{
	m_display.setFullWindow();
	m_faces[m_face]->Draw(m_display, m_currentTime);
	m_display.display(fullUpdate); //partial refresh
	m_guiState = wc::kWatchFace_State;
}

void CWatchyExpanded::DeepSleep()
{
	m_display.hibernate();
	g_displayFullInit = false;
	m_rtc.clearAlarm(); //resets the alarm flag in the RTC

	for(int i=0; i<40; i++) // Set pins 0-39 to input to avoid power leaking out
		pinMode(i, INPUT);

	esp_sleep_enable_ext0_wakeup(wc::rtc_pin, 0); //enable deep sleep wake on RTC interrupt
	esp_sleep_enable_ext1_wakeup(wc::btn_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
	esp_deep_sleep_start();
}
