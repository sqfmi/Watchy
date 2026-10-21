# Watchy - Fully Open Source E-Paper Watch

![Watchy](https://watchy.sqfmi.com/img/watchy_render.png)

**Buy Watchy from [The Pi Hut](https://thepihut.com/collections/sqfmi), [Crowd Supply](https://www.crowdsupply.com/sqfmi/watchy), and [Mouser](https://www.mouser.com/ProductDetail/SQFMI/SQFMI-WATCHY-10?qs=DRkmTr78QARN9VSJRzqRxw%3D%3D)!**

[**Watchy Case & Accessories**](https://shop.sqfmi.com)

Visit [**https://watchy.sqfmi.com**](https://watchy.sqfmi.com) for documentation, hardware design files, and more!

## Setup
1. In the Arduino IDE Boards Manager, [install support for the ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).
    * Arduino > Preferences > Additional Board Manager URLs
    * ```https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```
2. Install this library (search for **Watchy** in the library manager), and any other dependencies when prompted
3. Check out the examples under ```Examples``` -> ```Watchy```
4. Compile & Upload with these board settings:
    * Board: "Watchy"
    * Partition Scheme: "Huge App"
    * Board Revision: Choose your Watchy version
    * All Other Settings: leave to default

You may also have to install the [CP2104 USB to Serial drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) if the port is not automatically detected.

### Have Fun! :)


### Got Questions?

Join our [Discord](https://discord.gg/ZXDegGV8E7)



## Clock timezone independent of weather

Set the optional final `watchySettings::timeZone` member to a POSIX TZ rule,
for example `.timeZone = "CET-1CEST,M3.5.0,M10.5.0/3"` for Italy.
ESP32's C library expects a POSIX rule, not an IANA name such as `Europe/Rome`.
When omitted or empty, existing fixed-offset/weather timezone behavior is retained.

With this option enabled, the RTC stores UTC. Reads convert UTC to local time,
including daylight saving transitions without a network connection. The Set Time
menu and `init(datetime)` still accept local time. NTP writes UTC directly, and
weather responses cannot replace the selected timezone. NTP is also attempted at
the normal weather update interval when the weather HTTP request fails.

When first enabling this option on an existing watch, synchronize NTP or use Set
Time once: the previous RTC contents were local time. When disabling it, set the
clock again because the RTC still contains UTC. Ambiguous times in the repeated
fall-back hour follow the C library's `mktime` choice.

Host timezone tests: `c++ -std=c++11 tests/timezone.cpp -o /tmp/watchy-tz && /tmp/watchy-tz`.
