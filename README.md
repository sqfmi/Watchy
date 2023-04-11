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


