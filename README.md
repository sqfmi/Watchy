# Watchy - WIP

As I need to edit the main lib to do what I want/need to do. I had to fork the main repo to create my own version of the Watchy.

It's this a work in progress and be considere as experimental !

## How to

### Set seetings

First you need to edit the [parameters file](7_SEG/parameters.h.example) with your informations and rename it to `parameters.h`.

### Compile & Upload

In order to build and upload the Watchy from cmd line :

#### Setup some vars

```bash
export SKETCH="7_SEG/7_SEG.ino"
export FQBN="esp32:esp32:watchy:Revision=v20,PartitionScheme=huge_app,UploadSpeed=921600,DebugLevel=none"
export PORT="/dev/cu.wchusbserial56230332171"
```

You may need to replace the `--port` with your own.

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
