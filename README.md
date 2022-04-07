# Watchy [![Badge License]][License] [![Badge Discord]][Discord] [![Badge CrowdSupply]][CrowdSupply] [![Badge Mouser]][Mouser]

***A Fully Open Source E-Paper Watch***

---

 **⸢ [Website] ⸥**
 **⸢ [Shop] ⸥**

---

![Preview]

## Setup
1. In the Arduino IDE Boards Manager, [install support for the ESP32][ESP32].
    * Arduino > Preferences > Additional Board Manager URLs
    * ```https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json```
2. Install this library (search for **Watchy** in the library manager), and any other dependencies when prompted
3. Check out the examples under ```Examples``` -> ```Watchy```
4. Compile & Upload with these board settings:
    * Board: "ESP32 Dev Module"
    * Partition Scheme: "Minimal SPIFFS"
    * All Other Settings: leave to default

You may also have to install the [CP2104 USB to Serial drivers][Drivers] if the port is not automatically detected.

### Have Fun! :)

<!----------------------------------------------------------------------------->

[Badge License]: https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge
[Badge Discord]: https://img.shields.io/badge/Discord-7289DA?style=for-the-badge&logo=discord&logoColor=white
[Badge CrowdSupply]: https://img.shields.io/badge/CrowdSupply-00b2ad?style=for-the-badge&logo=shopify&logoColor=white 'Buy Watchy at Crowd Supply'
[Badge Mouser]: https://img.shields.io/badge/Mouser-0070bb?style=for-the-badge&logo=shopify&logoColor=white 'Buy Watchy at Mouser'

[CrowdSupply]: https://www.crowdsupply.com/sqfmi/watchy
[Discord]: https://discord.gg/ZXDegGV8E7
[License]: LICENSE
[Mouser]: https://www.mouser.com/ProductDetail/SQFMI/SQFMI-WATCHY-10?qs=DRkmTr78QARN9VSJRzqRxw%3D%3D

[Preview]: https://watchy.sqfmi.com/img/watchy_render.png


[Website]: https://watchy.sqfmi.com
[Shop]: https://shop.sqfmi.com

[Drivers]: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
[ESP32]: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html

