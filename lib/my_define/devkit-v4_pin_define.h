/*
devkit-v4_pin_define.h
*/
#ifndef DEVKIT_V4_PIN_DEFINE_H
#define DEVKIT_V4_PIN_DEFINE_H

#define PIN_OUT_BUZZER          12  // sotto itr
#define PIN_OUT_ON_3V3          13  // on vcc periferiche.
                                    // Su BMP280 mettere diodo in serie +Vcc per evitare ritorni di tensione

#define PIN_OUT_LED              2

#define PIN_I2C_SCL             21
#define PIN_I2C_SDA             22

#define PIN_IN_LINEA_A          34

#if 0
#define SD_CS                    5
#define TFT_CS   	              26
#define TFT_MOSI 	              23
#define TFT_SCLK 	              18
#define TFT_MISO 	              19
#define TFT_DC   	              14
#define TFT_RST  	              15
#define TOUCH_CS 	               4	//messo in Setup70_ESP32_DevKit_IL9341.h altrimenti non funziona
#endif

#define FLASH_CLK                6  //No usare
#define FLASH_SD0                7  //No usare
#define FLASH_SD1                8  //No usare
#define FLASH_SD2                9  //No usare
#define FLASH_SD3               10  //No usare
#define FLASH_CMD               11  //No usare

#endif