#pragma once

// STL
#include <vector>

// Defs
class CWatchFace;

class CWatchyExpanded
{
	public:
		CWatchyExpanded();

		void AddWatchFace(CWatchFace* pFace);

		void Init();

	private:
		void DisplayBusyCallback(const void*);
		void UpdateScreen();
		void DeepSleep();

		void DrawBasicClock();

		std::vector<CWatchFace*> m_faces;
		std::int8_t m_face = 0;

		GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> m_display;
		tmElements_t m_currentTime;
};
