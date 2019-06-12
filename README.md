# grbl-cnc32

Simple controller for my RS-CNC32, this Grbl controller runs on a ESP32.

Work based on https://github.com/mstrens/grbl_controller_esp32.

Thanks to https://github.com/mstrens for his initial work.

# features and todos

## feature

* move your tools on each axis with TFT or a nunchuk
* order homing, set coordinate ...
* get usefull statistics (logs, GRBL output)
* mount a SDCARD and print your GCODE files
* handle GRBL admin order (reset, alarm ...)
* wifi and rest server support
    * all configuration handle by api rest (read and write)
* ntp connect to obtain local time

## todo

* design telnet feature
* slack alerting, or any other kind of tools

# api

This firmware is API REST based.

## config.json resource (see bellow in setup bloc)

```console
GET|PUT <localhost:8080>/api/v1/config/config.json to handle config object.
```

## system commands resource

```console
GET <localhost:8080>/api/v1/reboot to reboot TFT.
```

# setup and more

## setup

Simply add a first i18n_enUS.json on your root SDCARD (it will be updated by API after)

Simply add a first config.json on your root SDCARD (it will be updated by API after)

```json
{
    "sys": {
        "lang": "/i18n_enUS.json"
    },
    "wifi": [
        {
            "ssid": "<your SSID>",
            "pass": "<your password>"
        }
    ],
    "tft": {
        "calibrate": [386, 3530, 220, 3627, 7]
    },
    "grbl": {
        "emulate": false
    },
    "nunchuk": {
        "calibrate": {
            "x": 100,
            "y": 100
        }
    },
    "commands": {
        "cmd01": [
            "Line 1",
            "Line 2",
            "..."
        ]
    }
}
```

## wifi bloc

setup your wifi access :
* an array of SSID/PASS to scan

## tft bloc

setup your tft :
* with a 5 values array of tft calibration

## grbl bloc

setup your grbl :
* emulate is a boolean to setup a mock behaviour

## nunchuk bloc

setup your nunchuk :
* calibrate contain 2 fields x and y to setup nunchuk calibration

## commands bloc

setup your commands :
* a lines array for the command

## fingerprint bloc

timestamp
* uTime le last update time in unix second
* fTime le last update time

# Hardware and software

## Hardware

To implement this project you need:
- an ESP32 development board and
- a display module combining 3 components:
  - a touch screen 320 X 240 a ILI9341 (display controller),
  - a XPT2046 (control the touch panel) and
  - a SD card support
  
Note: this configuration uses the ILI9341 with 4 wires (CLK, MOSI, MISO and CD).

Currently, this project works only with a TFT having a ILI9341 chip, a resolution of 320X240 and XPT2046 chip for the touch screen. If you are using another display, you should change the code yourself.

## Software

This project compiles in Arduino IDE but it requires:
- to add in Arduino IDE the software that support ESP32. The process is explained e.g. in this link
	https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
 
Take care that if you do not use my configuration, you can have:
-  to edit the file User_setup.h included in the TFT_eSPI_ms folder.
   This file specify the type of display controller and the pins being used by the SPI, the display and the touch screen chip select.
-  to edit the file config.h from this project in order to specify some pins being used (e.g. the chip select for the SD card reader)
  
Note: the pins used for the SPI signals (MOSI, MISO, SCLK) are currently hardcoded.
Please note that many ESP32 pins are reserved and can't ne used (reserved for bootup, for internal flash, input only,...).
See doc on ESP for more details.

