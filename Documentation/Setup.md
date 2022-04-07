
<h1 align = 'center'>Setup</h1>

<br>
<br>

## Board

In the **Arduino IDE Boards Manager** you will <br>
need to **[Install Support][ESP32]** for the **ESP32** board.

<br>

##### Navigate To

<kbd>**Arduino**</kbd> **›** <kbd>**Preferences**</kbd> **›** <kbd>**Additional Board Manager URLs**</kbd>

<br>

##### Insert

```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

<br>
<br>

---

<br>
<br>

## Library

In the **Library Manager**, search for `Watchy` <br>
and install it and any of its dependencies.

<br>
<br>

---

<br>
<br>

## Example

Check out the examples under

<kbd>**Examples**</kbd> **›** <kbd>**Watchy**</kbd>

<br>
<br>

---

<br>
<br>

## Compile / Upload

Use the following settings when building <br>
and leave any other ones at their default.

<br>

Setting |        |
:------:|:------:|
Board            | `ESP Dev Module`
Partition Scheme | `Minimal SPIFFS`

<br>
<br>

---

<br>
<br>

## Port

In case your port isn't automatically detected, you may <br>
also need to install the [CP2104 USB To Serial Drivers][Drivers].

<br>

<!----------------------------------------------------------------------------->

[Drivers]: https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
[ESP32]: https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html
