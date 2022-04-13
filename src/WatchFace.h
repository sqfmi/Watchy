#pragma once

// Time
#include <TimeLib.h>

// Expanded
#include "WatchyExpanded.h"

class CWatchFace
{
	public:
		CWatchFace() = default;
		virtual void Draw(CWatchyExpanded& expanded) = 0; //override this method for different watch faces
};
