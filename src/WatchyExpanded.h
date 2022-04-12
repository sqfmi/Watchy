#pragma once

// STL
#include <vector>

// GxEPD2
#include <GxEPD2_BW.h>

// Time
#include <TimeLib.h>

// Watchy
#include "WatchyRTC.h"

// Expanded
#include "watchy_config.h"

// Defs
class CWatchFace;
class CWatchyApp;

struct SExpandedData
{
	bool m_init = true;
	std::uint8_t m_face = 0;
};

class CWatchyExpanded
{
	public:
		CWatchyExpanded();

		void AddWatchFace(CWatchFace* pFace);
		void Init();
		void Run();

		using ADisplay = GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>;

	private:
		static void DisplayBusyCallback(const void*);

		void UpdateScreen(const bool fullUpdate);
		void DeepSleep();
		void HandleButtonPress();
		void BackFace();
		void ForwardFace();

		void _bmaConfig();
		static uint16_t _readRegister(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len);

		std::vector<CWatchFace*> m_faces;
		std::vector<CWatchyApp*> m_apps;

		ADisplay m_display;
		tmElements_t m_currentTime;
		std::int8_t m_guiState = 0;
		WatchyRTC m_rtc;
		SExpandedData& m_data;
		bool m_UpdateWatchFace = false;
		//BMA423 m_sensor;
};
