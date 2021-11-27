# Watchy - Fully Open Source E-Paper Watch

![Watchy](https://watchy.sqfmi.com/img/watchy_render.png)

[**Pre-order Watchy!**](https://www.crowdsupply.com/sqfmi/watchy)
Now with injection molded or CNC'd aluminum case

Visit [**https://watchy.sqfmi.com**](https://watchy.sqfmi.com) for documentation, hardware design files, and more!

## Setup
1. In the Arduino IDE Boards Manager, install support for the ESP32. You can find instructions here: https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md
2. Install this library (search for **Watchy** in the library manager), and any other depdencies when prompted
3. Check out the examples under ```Examples``` -> ```Watchy```
4. Compile & Upload with these board settings:
    * Board: "ESP32 Dev Module"
    * Partition Scheme: "Minimal SPIFFS"
    * All Other Settings: leave to default

You may also have to install the [CP2104 USB to Serial drivers](https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers) if the port is not automatically detected

### Have Fun! :)
    
    
# New Watchy owners READ ME
As of late 2021 there is a new Real-Time Clock chip in the lastest batch of Watchys.
This chip is *not* compatible with the older chip and the code has changed to accomdate this. This repository has a branch 'pfc8563'. Use that branch as the basis for *all* watch updates like clock faces. Current clock faces (other than the default face in the branch) needs to be updated to use the new RTC code if used in a new watchy. If you flash a face and all you see is 0's you know you flashed code based on the OLD chip. Switching chips was needed because of issues with the old chip.

