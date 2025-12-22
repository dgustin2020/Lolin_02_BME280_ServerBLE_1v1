/*
cyd32R2BT_pin_define.h
rispetto alla scheda CYD base
- Manca la fotoresistenza
- Il led rosso collegato su altro pin
- Sui connettori non abbiamo disponibile la I2C
  abbiamo solo la SPI su un singolo connettore e un ingresso sull'altro
- L'amplificatore audio ha un comonado di acceso/spento
- Possiamo misurare la tensione della batteria, via partitore da 2 x 100 Kohm
  quindi divisore per 2
*/
#ifndef CYD32R2BT_PIN_DEFINE_H
#define CYD32R2BT_PIN_DEFINE_H

// LCD
#define LCD_CS				        15	//-ok	Low == Enable
#define LCD_RS				         2	//-ok	Low == Command / High == Data
#define LCD_SCK 			        14	//-ok
#define LCD_MISO			        12	//-ok
#define LCD_MOSI			        13	//-ok
//      LCD_RESET			        EN  //-ok	Low == Reset common with RESET pin ESP32
// Touchscreen pins
#define XPT2046_IRQ           36  	// T_IRQ-ok
#define XPT2046_MOSI          32  	// T_DIN-ok
#define XPT2046_MISO          39  	// T_OUT-ok
#define XPT2046_CLK           25  	// T_CLK-ok
#define XPT2046_CS            33  	// T_CS -ok
//SD_Card
#define SD_MISO				        19	//-ok In	common with SPI peripheral
#define SD_MOSI				        23	//-ok Out	common with SPI peripheral
#define SD_SCK				        18	//-ok Out	common with SPI peripheral
#define SD_CS				           5	//-ok Out
// SPI peripheral
#define SPI_MISO_PIN		      19	//-ok In  	common with SD_card
#define SPI_MOSI_PIN		      23	//-ok Out 	common with SD_card
#define SPI_CS_PIN			      27	//-ok Out
#define SPI_SCK_PIN			      18	//-ok Out	common with SD_card
#define SPI_SDO_PIN			      19	//-ok In	common with SD_card
#define SPI_SDI_PIN			      23	//-ok Out	common with SD_card
// serial COM0
#define IN_COM0_RX_PIN		     3	//-ok
#define OUT_COM0_TX_PIN		     1	//-ok
// Battery
#define IN_BATT_PIN			      34	//-ok DAC Vbatt / 2 (part 2 x 100Kohm)
// Input generic
#define IN_GENERIC_PIN		    35	//-ok only input
//backlight
#define LCD_BACKLIGHT_PIN     21	//-ok High == On
//nota
#define OUT_DAC_PIN           26	//-ok
#define OUT_AUDIO_ENABLE_PIN   4	//-ok Low == enable
//led on board
#define OUT_LED_GREEN_PIN     16	//-ok Low == On
#define OUT_LED_RED_PIN       22	//-ok Low == On
#define OUT_LED_BLUE_PIN      17	//-ok Low == On
// DownLoad Key //button boot
#define IN_DOWNLOAD_KEY_PIN    0	//-ok Low == Key pressed
#define IN_BOOT_PIN            0

#if 0 // non disponibili su questa scheda
//fotoresistenza
#define IN_LDR_SENSOR_PIN     34
//I2C bus
#define	OUT_I2C_SCL			      22
#define IO_I2C_SDA			      21
#endif //

#endif