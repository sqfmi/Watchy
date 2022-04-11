#pragma once

// Time
#include <TimeLib.h>

// Expanded
#include "WatchyExpanded.h"

class CWatchFace
{
	public:
		CWatchFace() = default;
		//override this method for different watch faces
		virtual void Draw(CWatchyExpanded::ADisplay& display, const tmElements_t& time) = 0;
};
