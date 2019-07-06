#ifndef _config_h
#define _config_h

// Note: SPI is currently hardcoded for using following pins
// MOSI=13, MISO=12, SCK=14
// Those are used for the display, the touch panel and the SD card reader.

#define TFT_LED_PIN 25 // pin connected to led of lcd; pin has to be high to set led ON

// other TFT pins are defined in tft_espi User_Setup.h file

#define SD_CHIPSELECT_PIN 26 //5  // pin for SD card selection // to change probably

// pins for Serial to GRBL (it uses Serial2 UART)
#define SERIAL2_RXPIN 16
#define SERIAL2_TXPIN 17

// select color between (or define your own)
// TFT_BLACK       0x0000      /*   0,   0,   0 */
// TFT_NAVY        0x000F      /*   0,   0, 128 */
// TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
// TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
// TFT_MAROON      0x7800      /* 128,   0,   0 */
// TFT_PURPLE      0x780F      /* 128,   0, 128 */
// TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
// TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
// TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
// TFT_BLUE        0x001F      /*   0,   0, 255 */
// TFT_GREEN       0x07E0      /*   0, 255,   0 */
// TFT_CYAN        0x07FF      /*   0, 255, 255 */
// TFT_RED         0xF800      /* 255,   0,   0 */
// TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
// TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
// TFT_WHITE       0xFFFF      /* 255, 255, 255 */
// TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
// TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
// TFT_PINK        0xFC9F

#define JSON_CONFIG_SIZE 8192
#define JSON_I18N_SIZE 8192

#define SEMVER "v0.81"
#endif
