/*
Lolin32_pin_define.h
*/
#ifndef LOLIN32_PIN_DEFINE_H
#define LOlIN32_PIN_DEFINE_H


//lato DX (top view)
//+3V3
#define PIN_IO_22                22 // GPIO22 --> LED_BUILTIN
#define PIN_IO_19                19 // GPIO19 MISO
#define PIN_IO_23                23 // GPIO23 MOSI
#define PIN_IO_18                18 // GPIO18 CLK
#define PIN_IO_05                 5 // GPIO05 SS
#define PIN_IO_17                17 // GPIO17 U2TX
#define PIN_IO_16                16 // GPIO16 U2RX
#define PIN_IO_04                 4 // GPIO04 ADC2-0 Touch0 RTC-10 I2C1-CL SD-D1  HSPIHD
#define PIN_IO_00                 0 // GPIO00 ADC2-1 Touch1 RTC-11 I2C1-DA
#define PIN_IO_02                 2 // GPIO02 ADC2-2 Touch2 RTC-12 I2C2-CL SD-D0  HSPIWP
#define PIN_IO_15                15 // GPIO15 ADC2-3 Touch3 RTC-13 I2C2-DA SD-CMD HSPICS
#define PIN_IO_13                13 // GPIO13 ADC2-4 RTC-14 SD-D3  HSPIID

/** Lato SX (top view) **/
#define PIN_IN_36                36 // GPIO36 ADC1-0 RTC-0
#define PIN_IN_39                39 // GPIO39 ADC1-3 RTC-3
//RESET
#define PIN_IN_34                34 // GPIO36   ADC1-6   RTC-4
#define PIN_IN_35                35 // GPIO36   ADC1-5   RTC-5
#define PIN_IO_32                32 // GPIO32   ADC1-4   Touch9   RTC-9
#define PIN_IO_33                33 // GPIO33   ADC1-5   Touch8   RTC-8
#define PIN_IO_25                25 // GPIO25   ADC1-8   DAC1     RTC-6
#define PIN_IO_26                26 // GPIO26   ADC1-9   DAC2     RTC-7
#define PIN_IO_27                27 // GPIO27   ADC1-7   Touch7   RTC-17
#define PIN_IO_14                14 // GPIO14   ADC1-6   Touch6   RTC-16   SD-CLK   HSPICL
#define PIN_IO_12                12 // GPIO12   ADC1-5   Touch5   RTC-15   SD-D2    HSPIQ
//GND

/* definizione pin usati */
#define PIN_OUT_LED_BUILTIN      PIN_IO_22 // LED_BUILTIN
#define PIN_IN_ADC_BATT_VOLT     PIN_IO_12 // (battery voltage / 2)  --> partitore (100K+100K)
#define PIN_IN_BATT_CHARGE       PIN_IO_27 // Low = charge run / High = charge end
#define OUT_DAC_PIN              PIN_IO_26 // uso Dac_2
#define OUT_LED_GREEN_PIN        PIN_OUT_LED_BUILTIN

/*BATTERIA*/
/* in battery,
   calcolo rapporto partitore ingresso
   valore da moltiplicare alla lettura della tensione del ADC
   per avere la vera tensione di batteria
   Vbatt -- R1 --|------- Vout PIN_IN_ADC_BATT_VOLT
                 R2
        ---------|------- GND
*/
#define BATT_R1_PART            100000 //in ohm
#define BATT_R2_PART            100000 //in ohm
#define BATT_VOLT_RATE          ((BATT_R1_PART + BATT_R2_PART) / BATT_R2_PART)

/*SENSORE BMP280*/
/* BMP280 temp/press sensor */
/*da usare con Wire.beginn(SDA,SCL,)*/
#ifndef BME280_SPI
#define PIN_OUT_SENSOR_I2C_SCL      PIN_IO_04 // scl BMP280
#define PIN_IO_SENSORE_I2C_SDA      PIN_IO_00 // sda BMP280
#else
#define PIN_OUT_BME280_VCC	        PIN_IO_13  // High == On / usato come alimentazione del modulo
#define PIN_OUT_BME280_SCK		PIN_IO_18
#define PIN_OUT_BME280_CS		PIN_IO_05   // Low == mode SPI / HIGH == mode I2C
#define PIN_OUT_BME280_MOSI		PIN_IO_23
#define PIN_IN_BME280_MISO		PIN_IO_19
#endif

#endif