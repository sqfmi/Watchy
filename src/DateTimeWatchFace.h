#pragma once

// Time
#include <TimeLib.h>

// Expanded
#include "WatchFace.h"
#include "WatchyExpanded.h"

// Faces
#include "TimeWatchFace.h"

class CDateTimeWatchFace : public CTimeWatchFace
{
	public:
		void Draw(CWatchyExpanded::ADisplay& display, const tmElements_t& time) override;
};
