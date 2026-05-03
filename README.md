# Watchy v2 - WIP

[![cpp-linter](https://github.com/cpp-linter/cpp-linter-action/actions/workflows/cpp-linter.yml/badge.svg)](https://github.com/mickahell/Watchy/actions/workflows/lint.yml)

As I need to edit the main lib to do what I want/need to do. I had to fork the main repo to create my own version of the Watchy.

The actual face is the `Watchy7SEG` from the main repo.

It's this a work in progress and be considere as experimental !

All the features details are available in [features file](features.md).

## How to

### Set settings

First you need to edit the [parameters file](parameters.h.example) with your informations and rename it to `parameters.h`.

### Compile & Upload

In order to build and upload the Watchy from cmd line :

#### Setup some vars

```bash
export SKETCH="electrowatch.ino"

# v2
export FQBN="esp32:esp32:watchy:Revision=v20,PartitionScheme=huge_app,UploadSpeed=921600,DebugLevel=none"
export PORT="/dev/cu.wchusbserial56230332171"

# v3
export FQBN="esp32:esp32:esp32s3:FlashSize=8M,PartitionScheme=default_8MB,UploadSpeed=921600,DebugLevel=none,EraseFlash=none"
export PORT="/dev/cu.usbmodem14101"
```

You may need to replace the `--port` with your own.

#### Deps

```bash
arduino-cli core update-index --additional-urls https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
arduino-cli core install esp32:esp32@2.0.17
arduino-cli lib install Watchy@1.4.14
```

#### Compile

```bash
arduino-cli compile ${SKETCH} --fqbn ${FQBN} --log-level error
```

#### Upload

```bash
arduino-cli upload ${SKETCH} --port ${PORT} --fqbn ${FQBN}
```

#### Both

```bash
arduino-cli compile --upload ${SKETCH} --port ${PORT} --fqbn ${FQBN}
```

### Debug

```bash
screen ${PORT} 115200
```

to exit `ctrl a + k`

### Reset

Install [esptool.py](https://docs.espressif.com/projects/esptool/en/latest/esp32/installation.html)

```bash
esptool.py --port ${PORT} erase_flash
```
