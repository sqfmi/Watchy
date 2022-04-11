#pragma once

// STL
#include <vector>

// GxEPD2
#include <GxEPD2_BW.h>

// Time
#include <TimeLib.h>

// Watchy
#include "config.h"

// Expanded
#include "WatchFace.h"

// Defs
class CWatchFace;

class CWatchyExpanded
{
	public:
		CWatchyExpanded();

		void AddWatchFace(CWatchFace* pFace);

		void Init();

	private:
		static void DisplayBusyCallback(const void*);

		void UpdateScreen();
		void DeepSleep();

		void DrawBasicClock();

		std::vector<CWatchFace*> m_faces;
		std::int8_t m_face = 0;

		GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> m_display;
		tmElements_t m_currentTime;
		std::int8_t m_guiState = kWatchFace_State;
};
