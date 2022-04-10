#include "WatchyExpanded.h"

// ESP
#include <esp_sleep.h>

CWatchyExpanded::CWatchyExpanded() : m_display(GxEPD2_154_D67(DISPLAY_CS, DISPLAY_DC, DISPLAY_RES, DISPLAY_BUSY))
{
}

void CWatchyExpanded::AddWatchFace(Watchy* pFace)
{
	m_faces.push_back(pFace);
}

void CWatchyExpanded::Init()
{
	const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
	Wire.begin(SDA, SCL); //init i2c
	RTC.init();

	// Init the display here for all cases, if unused, it will do nothing
	m_display.init(0, true, 10, true); // 10ms by spec, and fast pulldown reset
	display.epd2.setBusyCallback(displayBusyCallback);

	switch (wakeup_reason)
	{
		case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
			if(guiState == kWatchFace_State){
				RTC.read(m_currentTime);
				showWatchFace(true); //partial updates on tick
			}
			break;
	//	case ESP_SLEEP_WAKEUP_EXT1: //button Press
	//		handleButtonPress();
	//	break;
	//	default: //reset
	//		RTC.config(datetime);
	//		_bmaConfig();
	//		RTC.read(currentTime);
	//		//showWatchFace(false); //full update on reset
		break;
	}
	//deepSleep();
}

void Watchy::DisplayBusyCallback(const void*)
{
	gpio_wakeup_enable((gpio_num_t)DISPLAY_BUSY, GPIO_INTR_LOW_LEVEL);
	esp_sleep_enable_gpio_wakeup();
	esp_light_sleep_start();
}

void UpdateScreen()
{
	m_display.setFullWindow();
	//drawWatchFace();
	DrawBasicClock(); // Temp

	m_display.display(true); //partial refresh
	guiState = kWatchFace_State;
}

void CWatchyExpanded::DrawBasicClock()
{
	m_display.setFullWindow();
	m_display.fillScreen(GxEPD_BLACK);
	m_display.setFont(&FreeMonoBold9pt7b);

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
	display.hibernate();
	RTC.clearAlarm(); //resets the alarm flag in the RTC

	for(int i=0; i<40; i++) // Set pins 0-39 to input to avoid power leaking out
		pinMode(i, INPUT);

	esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN, 0); //enable deep sleep wake on RTC interrupt
	esp_sleep_enable_ext1_wakeup(BTN_PIN_MASK, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
	esp_deep_sleep_start();
}
