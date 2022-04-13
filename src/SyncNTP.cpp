// Self
#include "SyncNTP.h"

// GxEPD2
#include <GxEPD2_BW.h>

// Fonts
#include <Fonts/FreeMonoBold9pt7b.h>

// WiFi
#include <WiFi.h>

// Watchy
#include "WatchyRTC.h"

// NTP
#include <NTPClient.h>

CSyncNTP::CSyncNTP(CWatchyExpanded& expanded) : CWatchyApp(expanded) {}

const char* CSyncNTP::Name() const
{
	return "SyncNTP";
}

void CSyncNTP::Work()
{
	CWatchyExpanded::ADisplay& display = m_expanded.Display();
	display.setFullWindow();
	display.fillScreen(GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	display.setTextColor(GxEPD_WHITE);
	display.setCursor(0, 10);
	display.println("Syncing NTP...");
	display.display(false); //full refresh

	if(m_expanded.ConnectWiFi())
	{
		if (!SyncNTP(3600 * -7, 3600, "pool.ntp.org"))
			display.println("Failed to Sync NTP!");
		else
			display.println("Synced to NTP!");

		WiFi.mode(WIFI_OFF);
		btStop();
	}
	else
	{
		display.println("WiFi Not Configured.");
	}

	display.display(true); //full refresh
	delay(3000);
}

//NTP sync - call after connecting to WiFi and remember to turn it back off
bool CSyncNTP::SyncNTP(long gmt, int dst, String ntpServer)
{ 
	WiFiUDP ntpUDP;
	NTPClient timeClient(ntpUDP, ntpServer.c_str(), gmt);
	timeClient.begin();
	if(!timeClient.forceUpdate())
		return false; //NTP sync failed
	tmElements_t tm;
	breakTime((time_t)timeClient.getEpochTime(), tm);
	m_expanded.RTC().set(tm);
	return true;
}
