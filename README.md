# Watchy - Fully Open Source E-Paper Watch

![Watchy](https://watchy.sqfmi.com/img/watchy_render.png)

**Buy Watchy at [Crowd Supply](https://www.crowdsupply.com/sqfmi/watchy) and [Mouser](https://www.mouser.com/ProductDetail/SQFMI/SQFMI-WATCHY-10?qs=DRkmTr78QARN9VSJRzqRxw%3D%3D)!**

[**Watchy Case & Accessories**](https://shop.sqfmi.com)

Visit [**https://watchy.sqfmi.com**](https://watchy.sqfmi.com) for documentation, hardware design files, and more!

## Setup
1. In the Arduino IDE Boards Manager, [install support for the ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).
    * Arduino > Preferences > Additional Board Manager URLs
    * ```https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```
2. Install this library (search for **Watchy** in the library manager), and any other dependencies when prompted
3. Check out the examples under ```Examples``` -> ```Watchy```
4. Compile & Upload with these board settings:
    * Board: "ESP32 Dev Module"
    * Partition Scheme: "Huge App"
    * All Other Settings: leave to default

You may also have to install the [CP2104 USB to Serial drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) if the port is not automatically detected.

## User Manual

### Wifi Setup

1. Click 'Setup Wifi' from main menu on the watch.
2. Connect to 'Watchy Ap' wifi from another device such as phone or computer / laptop.
3. Open 192.168.4.1 in the browser. On iphone this page open itself when connecting to wifi.
4. Click 'Configure WiFi'
5. Enter SSID and password. (Note: SSID can be prefilled by listed networks on the top of this screen)
6. Hit 'Save' button and wait for Watchy.
7. If connection failed, the watchy will display 'Setup failed & timed out!', otherwise it will display the local ip address and SSID of the connected network with confirmation.

#### Troubleshoot Wifi Setup:
1. Click 'Info' from 192.168.4.1 page.
2. Check your router setting to make sure the listed mac address is allowed.
3. If above didn't fix, then try clicking 'Erase Wifi Config' from 'Info' page and wait for Watchy to restart itself. Once it happen, try the wifi setup again and it should hopefully work.

### Have Fun! :)


