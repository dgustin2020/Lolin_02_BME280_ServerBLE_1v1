/*
my_bme280_driver.h
*/
#ifndef MY_BME280_DRIVER_H
#define MY_BME280_DRIVER_H
//#include <my_define.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "my_bme280_driver.h"

/*
vedi Lolin32_Pin_define.h

I2C default
pin da usare da default sono
SCL -> D22
SDA -> D21

SPI
MISO (SDO)  -> D19
MOSI (SDI)  -> D23  
SCK  (SCL)  -> D18
CS   (CSB)  -> D05
*/

#define SEALEVELPRESSURE_HPA (1013.25)

typedef struct bme280_sensor{
  float   temp;
  float   press;
  float   alti;
  float   humy;
  T_Lclk  time_stamp;           //ora lettura valori misure
  bool    new_read;             //true == nuova lettura / false == vecchia lettura
  int     status;               //stato del sensore
  T_Lclk  delay_read_val;       //valore in mSec delay lettura sensori con memorizzaione
  int     autoemit_delay_val;   //valore in secondi (_L_SEC) 0 = non auto emissione
  T_Lclk  autoemit_Lclk;        //timer per emissione valori
  uint8_t task_enable;          //abilta task lettura sensore. 1 = enable / 0 disable
  char    autoemit_port;        //port su cui inviare autoemit parametri. 0=USB 1=BLUE
}T_sensor_read;

#define BME280_STATUS_NONE            0
#define BME280_STATUS_OK              1
#define BME280_STATUS_ER_NOT_FOUND    2
#define BME280_STATUS_ER_NO_MISURE    3

#define BME280_DELAY_RATE             2000 //in mec
#if 0
#define BME280_TASK_RESULT_WAIT       0
#define BME280_TASK_RESULT_ERR        1
#define BME280_TASK_RESULT_OK         2
#define BME280_TASK_RESULT_STOP       3
#define BME280_TASK_RESULT_NOT_FOUND  4
#endif

/* procedure */
extern void     bme280_power_on(void);
extern void     bme280_power_off(void);
extern bool     bme280_init_sensor(void);
extern bool     bme280_sensor_test(void);
extern void     bme280_set_parameter(void);
extern bool     bme280_read_sensor_value(T_sensor_read* sensor_read);

#endif //BME280_DRIVER_H