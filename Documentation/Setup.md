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