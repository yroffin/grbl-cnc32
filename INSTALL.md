# grbl-cnc32

## Setup arduino

Simply compil and deploy arduino part (vscode plugin is very cool for that)

## Setup ui

In ui folder, simply run npm build (pre-requisite node)

```shell
npm run build
```

And copy dist folder on sdcard in /static

## Setup config

Config sample files are stored in config folder

Simply add a first i18n_enUS.json, i18n_frFR.json on your root SDCARD (it can be updated by API after)

Simply add a first config.json (*) on your root SDCARD (it can be updated by API after)

* remove key tft to force re-calibration

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

