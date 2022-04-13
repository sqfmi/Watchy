// Self
#include "DateWatchFace.h"

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

// Time
#include <TimeLib.h>

void CDateWatchFace::Draw(CWatchyExpanded& expanded)
{
	StartDraw(expanded.Display());
	CTimeWatchFace::DrawInternal(expanded);
	DrawInternal(expanded);
};

void CDateWatchFace::StartDraw(CWatchyExpanded::ADisplay& display)
{
	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(0, 15);
};

void CDateWatchFace::DrawInternal(CWatchyExpanded& expanded)
{
	CWatchyExpanded::ADisplay& display = expanded.Display();
	tmElements_t& time                 = expanded.Time();

	m_currentLocalTime.tm_wday = time.Wday - 1;
	m_currentLocalTime.tm_year = time.Year + 1970 - 1900;
	m_currentLocalTime.tm_mon  = time.Month - 1;
	m_currentLocalTime.tm_mday = time.Day;

	char buffer[32];
	strftime(buffer, sizeof(buffer), "%a %b %d, %Y", &m_currentLocalTime);
	display.print(buffer);
	display.println();
};
