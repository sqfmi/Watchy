// Self
#include "TimeWatchFace.h"

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

void CTimeWatchFace::Draw(CWatchyExpanded& expanded)
{
	CWatchyExpanded::ADisplay& display = expanded.Display();
	StartDraw(display);
	DrawInternal(expanded);
};

void CTimeWatchFace::StartDraw(CWatchyExpanded::ADisplay& display)
{
	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(0, 15);
};

void CTimeWatchFace::DrawInternal(CWatchyExpanded& expanded)
{
	CWatchyExpanded::ADisplay& display = expanded.Display();
	tmElements_t& time = expanded.Time();
	m_currentLocalTime.tm_hour = time.Hour;
	m_currentLocalTime.tm_min  = time.Minute;
	m_currentLocalTime.tm_sec  = time.Second;

	display.print("Time: ");
	char buffer[20];
	strftime(buffer, sizeof(buffer), " %I:%M %p", &m_currentLocalTime);
	display.println(buffer);
};
