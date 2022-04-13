#pragma once

// Expanded
#include "WatchyExpanded.h"

class CWatchyApp
{
	public:
		CWatchyApp(CWatchyExpanded& expanded);
		virtual const char* Name() const = 0;
		virtual void        Work()       = 0;

	protected:
		CWatchyExpanded& m_expanded;
};
