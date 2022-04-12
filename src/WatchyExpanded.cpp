#include "WatchyExpanded.h"

// ESP
#include <esp_sleep.h>

// Wire
#include <Wire.h>

// Faces
#include "TimeWatchFace.h"
#include "DateTimeWatchFace.h"

RTC_DATA_ATTR SExpandedData g_data;

CWatchyExpanded::CWatchyExpanded() : m_display(GxEPD2_154_D67(wcd::cs, wcd::dc, wcd::reset, wcd::busy)), m_data(g_data)
{
	AddWatchFace(new CTimeWatchFace);
	AddWatchFace(new CDateTimeWatchFace);
}

void CWatchyExpanded::AddWatchFace(CWatchFace* pFace)
{
	m_faces.push_back(pFace);
}

void CWatchyExpanded::Run()
{
	const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause(); //get wake up reason
	Wire.begin(SDA, SCL); //init i2c
	m_rtc.init();

	// Init the display here for all cases, if unused, it will do nothing
	m_display.init(0, m_data.m_init, 10, true); // 10ms by spec, and fast pulldown reset
	//Init();
	m_display.epd2.setBusyCallback(DisplayBusyCallback);

	switch (wakeup_reason)
	{
		case ESP_SLEEP_WAKEUP_EXT0: //RTC Alarm
			if(m_guiState == wc::kWatchFace_State)
				m_UpdateWatchFace = true;
		break;
		case ESP_SLEEP_WAKEUP_EXT1: //button Press
			HandleButtonPress();
		break;
		default: //reset
			m_rtc.config("");
		break;
	}
	if (m_UpdateWatchFace)
	{
		m_rtc.read(m_currentTime);
		UpdateScreen(false); // full update
	}
	DeepSleep();
}

void CWatchyExpanded::Init()
{
	if (!m_data.m_init)
		return;

	m_data.m_init = false;
}

void CWatchyExpanded::DisplayBusyCallback(const void*)
{
	gpio_wakeup_enable(static_cast<gpio_num_t>(wcd::busy), GPIO_INTR_LOW_LEVEL);
	esp_sleep_enable_gpio_wakeup();
	esp_light_sleep_start();
}

void CWatchyExpanded::UpdateScreen(const bool partial_update)
{
	m_display.setFullWindow();
	m_currentTime.Month = 4;
	m_faces[m_data.m_face % m_faces.size()]->Draw(m_display, m_currentTime);
	m_display.display(partial_update); //partial refresh
	m_guiState = wc::kWatchFace_State;
}

void CWatchyExpanded::DeepSleep()
{
	m_display.hibernate();
	m_data.m_init = false;
	m_rtc.clearAlarm(); //resets the alarm flag in the RTC

	for(int i = 0; i < 40; ++i) // Set pins 0-39 to input to avoid power leaking out
		pinMode(i, INPUT);

	esp_sleep_enable_ext0_wakeup(wc::rtc_pin, 0); //enable deep sleep wake on RTC interrupt
	esp_sleep_enable_ext1_wakeup(wc::btn_pin_mask, ESP_EXT1_WAKEUP_ANY_HIGH); //enable deep sleep wake on button press
	esp_deep_sleep_start();
}

void CWatchyExpanded::HandleButtonPress()
{
	const uint64_t kWakeupBit = esp_sleep_get_ext1_wakeup_status();
	if (kWakeupBit & wc::up_btn_mask)
		BackFace();
	else if (kWakeupBit & wc::down_btn_mask)
		ForwardFace();
}

void CWatchyExpanded::BackFace()
{
	--m_data.m_face;
	m_UpdateWatchFace = true;
}

void CWatchyExpanded::ForwardFace()
{
	++m_data.m_face;
	m_UpdateWatchFace = true;
}

void CWatchyExpanded::_bmaConfig()
{
	//if (m_sensor.begin(_readRegister, _writeRegister, delay) == false)
	//	return; //fail to init BMA
}

uint16_t CWatchyExpanded::_readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom((uint8_t)address, (uint8_t)len);
	uint8_t i = 0;
	while (Wire.available())
		data[++i] = Wire.read();
	return 0;
}
