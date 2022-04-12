#pragma once

// Time
#include <TimeLib.h>

// Expanded
#include "WatchFace.h"
#include "WatchyExpanded.h"

class CTimeWatchFace : public CWatchFace
{
	public:
		void Draw(CWatchyExpanded::ADisplay& display, const tmElements_t& time) override;

	protected:
		tm m_currentLocalTime;
};
