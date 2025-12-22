/*
my_bme280_driver.cpp
*/

#include <common_include.h>

//#include <my_define.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "my_bme280_driver.h"

/* uso con I2C */
#ifndef BME280_SPI 
Adafruit_bme280 bme; // I2C
#else
//Adafruit_BME280 bme(PIN_OUT_bme280_CS); // hardware SPI
Adafruit_BME280 bme(PIN_OUT_BME280_CS, PIN_OUT_BME280_MOSI, PIN_IN_BME280_MISO, PIN_OUT_BME280_SCK); // software SPI
#endif

#define SEALEVELPRESSURE_HPA (1013.25)

/* dati sensore */

/* sviluppo */

/**** pubbliche ****/
/* 
accendi alimentazione sensore 
*/
void bme280_power_on(void)
{ 
  pinMode(PIN_OUT_BME280_VCC, OUTPUT);
  digitalWrite(PIN_OUT_BME280_VCC, HIGH); 
}

/* spegni alientazione sensore */
void bme280_power_off(void)
{ pinMode(PIN_OUT_BME280_VCC, OUTPUT);
  digitalWrite(PIN_OUT_BME280_VCC, LOW); 
}

/*
prepara i parametri per ttivazione sensore
delay_rate = intervallo lettura sensori
*/
bool bme280_init_sensor(void)
{
  /* abilita pin alimentzione sensore in out*/
  bme280_power_on(); //acceso 
  #ifdef DEBUG_BME280
  usb_232.load_tx_my_string_CR_LF((uint8_t*)"BME280 init_start module powwer on");
  #endif
  #ifndef BME280_SPI 
  /* set pin x I2C */
  Wire.begin(pin_sda, pin_scl);//set pin da usare per cmp280
  #else
  /* set pin x SPI */
  #endif

  #ifdef DEBUG_BME280
  usb_232.load_tx_my_string_CR_LF((uint8_t*)"BME280 init_start find BME280 sensor!");
  #endif
  return bme.begin(0x76);//prova attivazione
}

/*
attiva il driver sensore
esce con false se non attivato
true attivto ok
*/
bool bme280_sensor_test(void)
{
  #ifdef DEBUG_BME280
  usb_232.load_tx_my_string_CR_LF((uint8_t*)"BME280 init_test find BME280 sensor!");
  #endif
  return bme.begin(0x76);
}

/*
imposta i parametri default
*/
void bme280_set_parameter(void)
{
  /* Default settings from datasheet. */
  // weather monitoring --> adafruit_bme280_library
  #ifdef DEBUG_BME280
  Serial.println("-- Weather Station Scenario --");
  Serial.println("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling,");
  Serial.println("filter off");
  #endif
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   // filter off
  );
}

/*
forza lettura dei sensori del bme280
leggi i valori e li memorizza nel T_sensor_read passato
*/
bool bme280_read_sensor_value(T_sensor_read* sensor_read)
{
  if (bme.takeForcedMeasurement()) 
  {
    sensor_read->time_stamp = Base_Lclk;
    sensor_read->status     = BME280_STATUS_OK;
    sensor_read->temp       = bme.readTemperature();
    sensor_read->press      = (bme.readPressure() / 100.0F);
    sensor_read->alti       = bme.readAltitude(SEALEVELPRESSURE_HPA);/* Adjusted to local forecast! */
    sensor_read->humy       = bme.readHumidity();
    sensor_read->new_read   = true;//set nuova lettura x main
    #ifdef DEBUG_BME280
    Serial.print(F("Temperature = "));
    Serial.print(sensor_read->temp);
    Serial.println(" *C");
    Serial.print(F("Pressure = "));
    Serial.print(sensor_read->press);
    Serial.println(" hPa");
    Serial.print(F("Approx altitude = "));
    Serial.print(sensor_read->alti); /* Adjusted to local forecast! */
    Serial.println(" m");
    Serial.print(F("Humyditi = "));
    Serial.print(sensor_read->humy); /* Adjusted to local forecast! */
    Serial.println(" %");
    Serial.println();
    #endif
    return true;
  }
  else 
  {
    sensor_read->status = BME280_STATUS_ER_NO_MISURE;
    sensor_read->new_read  = false; //set no nuova lettura x main
    #ifdef DEBUG_BME280
    Serial.println("Forced measurement failed!");
    #endif
    return false;
  }
}

