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

class CWatchyExpanded
{
	public:
		CWatchyExpanded();

		void AddWatchFace(CWatchFace* pFace);
		void Init();

		using ADisplay = GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT>;

	private:
		static void DisplayBusyCallback(const void*);

		void UpdateScreen(const bool fullUpdate);
		void DeepSleep();

		std::vector<CWatchFace*> m_faces;
		std::int8_t m_face = 0;

		ADisplay m_display;
		tmElements_t m_currentTime;
		std::int8_t m_guiState = wc::kWatchFace_State;
		WatchyRTC m_rtc;
};
