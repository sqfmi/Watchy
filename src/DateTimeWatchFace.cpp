// Self
#include "DateTimeWatchFace.h"

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

void CDateTimeWatchFace::Draw(CWatchyExpanded::ADisplay& display, const tmElements_t& time)
{
	CTimeWatchFace::Draw(display, time);

	m_currentLocalTime.tm_wday = time.Wday - 1;
	m_currentLocalTime.tm_year = time.Year + 1970 - 1900;
	m_currentLocalTime.tm_mon  = time.Month - 1;
	m_currentLocalTime.tm_mday = time.Day;

	char buffer[32];
	strftime(buffer, sizeof(buffer), "%a %b %d, %Y", &m_currentLocalTime);
	display.print(buffer);
	display.println();
};
