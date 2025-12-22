/*
cyd_pin_define.h
*/
#ifndef CYD_PIN_DEFINE_H
#define CYD_PIN_DEFINE_H

// Touchscreen pins
#define XPT2046_IRQ           36  // T_IRQ
#define XPT2046_MOSI          32  // T_DIN
#define XPT2046_MISO          39  // T_OUT
#define XPT2046_CLK           25  // T_CLK
#define XPT2046_CS            33  // T_CS
//backlight
#define LCD_BACKLIGHT_PIN     21
//nota
#define OUT_DAC_PIN           26
//led on board
#define OUT_LED_GREEN_PIN     16
#define OUT_LED_RED_PIN        4
#define OUT_LED_BLUE_PIN      17
//fotoresistenza
#define IN_LDR_SENSOR_PIN     34
//button boot
#define IN_BOOT_PIN            0
//I2C bus
#define	OUT_I2C_SCL			  22
#define IO_I2C_SDA			  21

#endif