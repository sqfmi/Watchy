// Self
#include "TimeWatchFace.h"

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

void CTimeWatchFace::Draw(CWatchyExpanded::ADisplay& display, const tmElements_t& time)
{
	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(0, 15);

	m_currentLocalTime.tm_hour = time.Hour;
	m_currentLocalTime.tm_min  = time.Minute;
	m_currentLocalTime.tm_sec  = time.Second;

	display.print("Time: ");
	char buffer[20];
	strftime(buffer, sizeof(buffer), " %I:%M %p", &m_currentLocalTime);
	display.println(buffer);
};
