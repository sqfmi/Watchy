#pragma once

// Expanded
#include "WatchFace.h"
#include "WatchyExpanded.h"

// Faces
#include "TimeWatchFace.h"

class CDateWatchFace : public CTimeWatchFace
{
	public:
		void Draw(CWatchyExpanded& expanded) override;

		void StartDraw(CWatchyExpanded::ADisplay& display);
		void DrawInternal(CWatchyExpanded& expanded);
};
