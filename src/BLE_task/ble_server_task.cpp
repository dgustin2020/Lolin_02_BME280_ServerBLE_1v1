/*
ble_server_task.cpp
*/

// quello che segue, serva per abilitare qualcosa relativa a BLE
#define BLE_42_FEATURE_SUPPORT                      TRUE
#define BLE_50_FEATURE_SUPPORT                      TRUE

#include <common_include.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include "..\BME280_Task\bme280_task.h"
#include "ble_server_task.h"
#include "ble_server_task_config.h"

/*procedure locali*/
static void dummy(void);
static void server_init(void);
static void server_run(void);

/*variabili locali*/
static FnPtr        tbl_fn_Loop[] = {// dich. array puntatori alle funzioni
  server_init,      //00 SERVER_INIT_IDX
  server_run,       //01 SERVER_RUN_IDX
  0
};
#define LOOP_MASK               0x01  //da modificare in funzione delle procedure usate
#define START_FUNCTION_IDX      0x00  //prima funzione chiamata dall'attivazione del task
#define SERVER_INIT_IDX         START_FUNCTION_IDX
#define SERVER_RUN_IDX          SERVER_INIT_IDX + 1

My_Task_Gest    ble_task;           //crea gestione  main_loop
static T_Lclk   ble_delay_notify = BLE_DELAY_NOTIFY_DEFAULT;//delay invio nitifiche al client

extern bool     deviceConnected;
bool            deviceConnected = false;//non connesso

/************************************************************************************************ */
/*procedure pubbliche*/
/************************************************************************************************ */
/*
start task bme280
*/
void ble_server_task_init(void)
{
  /* inizializza loop */
  proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)"BLE_SERVER_task loop init", COM_PORT_USB);
  ble_task.task_enable();//abilita task
  ble_task.init_and_run(SERVER_INIT_IDX, LOOP_MASK, tbl_fn_Loop, (T_Lclk*)&Base_Lclk);//inizializza loop
}

/*
loop BLE
*/
void ble_server_task_run(void)
{
  if (ble_task.task_get_state())//test se task attivo, passato da init
    ble_task.run();
}

/*
*/
void ble_server_task_enable(void)
{ ble_task.task_enable(); }

/*
*/
void ble_server_task_disable(void)
{ ble_task.task_disable();  }


/*****************************************************************************/
/* locali */
/*****************************************************************************/
/**/
static void dummy(void)
{ return; }


#define INVIA_ALTI

static BLEService*        bmeService;
static BLEServer*         pServer;
/*
inizializza serve, e caratteristiche BLE dei parametri da trasmettere
*/
static void server_init(void)
{
  #ifdef DEBUG_BLE
  proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)"BLE_SERVER_task init", COM_PORT_USB);
  #endif

  // Create the BLE Device
  BLEDevice::init(bleServerName);
  // Create the BLE Server
  pServer = BLEDevice::createServer();//create server object
  pServer->setCallbacks(new MyServerCallbacks());//attach ti server callback function
  // Create the BLE Service
  bmeService = pServer->createService(SERVICE_UUID);
  /**/
  bmeService->addCharacteristic(&elevationCharacteristic);
  elevationCharacteristic.addDescriptor(&elevationDescriptor);

  // Create BLE Characteristics and corresponding Descriptors
  bmeService->addCharacteristic(&temperatureCharacteristic);
  temperatureCharacteristic.addDescriptor(&temperatureDescriptor);
  /**/
  bmeService->addCharacteristic(&humidityCharacteristic);
  humidityCharacteristic.addDescriptor(&humidityDescriptor);  
  /**/
  bmeService->addCharacteristic(&pressureCharacteristic);
  pressureCharacteristic.addDescriptor(&pressureDescriptor);

  // Start the service
  bmeService->start();
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  
  #ifdef DEBUG_BLE
  proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)"BLE_SERVER_task Waiting a client connection to notify...\r\n", COM_PORT_USB);
  #endif
  ble_delay_notify = BLE_DELAY_NOTIFY_DEFAULT;
  ble_task.loc_time = 0;
  ble_task.set_idx(SERVER_RUN_IDX);
}

/*
loop atesa connessione ed invio parametri
*/
static void server_run(void)
{
if (deviceConnected) //test se connesso ble
  {//connesso, verifica se scaduto timer
    //if (Base_Lclk > loc_time)
    if (Base_Lclk > ble_task.loc_time)
    {//scaduto
      #ifdef DEBUG_BLE
      char* p_buff = (char*)malloc(200 * sizeof(char));
      sprintf (p_buff,"BLE_task_run timeout %i %i", ble_task.loc_time, ble_delay_notify);
      proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)p_buff, COM_PORT_USB);
      #endif
      if (ble_task.loc_time)//test se prima impostazione timer o riarma timer
        ble_task.loc_time += ble_delay_notify;// arma timer
      else
        ble_task.loc_time = Base_Lclk + ble_delay_notify;//riarma timer
    
      // Read temperature as Celsius (the default)
      float t = bme280_task_read_temp();// bme.readTemperature();
      // Read humidity
      float h = bme280_task_read_humy();// bme.readHumidity();
      // Read pressure
      float p = bme280_task_read_press();//bme.readPressure()/100.0F;
      // Read altitudine
      float a = bme280_task_read_alti();

    #ifdef INVIA_ALTI
      //Notify altitudine reading
      uint32_t alti = (uint32_t)( a * 100);
      //Set elevation Characteristic value and notify connected client
      elevationCharacteristic.setValue(alti);
      elevationCharacteristic.notify();   
      #ifdef DEBUG_BLE
      static char elevationTemp[6];
      dtostrf(a, 6, 2, elevationTemp);
      sprintf (p_buff,"Alti: %.0f mt - %s", a, elevationTemp);
      proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)p_buff, COM_PORT_USB);
      #endif
    #endif

      //Notify pressure reading
      uint32_t pressure = (uint32_t)(p * 10);
      //Set pressure Characteristic value and notify connected client
      pressureCharacteristic.setValue(pressure);
      pressureCharacteristic.notify();   
      #ifdef DEBUG_BLE
      static char pressureTemp[6];
      dtostrf(p, 6, 2, pressureTemp);
      sprintf (p_buff,"Press: %.1f hPa - %s", p, pressureTemp);
      proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)p_buff, COM_PORT_USB);
      #endif

      //Notify humidity reading
      uint16_t humidity = (uint16_t) (h * 100);
      //Set humidity Characteristic value and notify connected client
      humidityCharacteristic.setValue(humidity);
      humidityCharacteristic.notify();   
      #ifdef DEBUG_BLE
      static char humidityTemp[6];
      dtostrf(h, 6, 2, humidityTemp);
      sprintf (p_buff,"Humidity: %.1f %% - %s", h, humidityTemp);
      proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)p_buff, COM_PORT_USB);
      #endif

      //Notify temperature reading
      uint32_t temperature = (uint32_t)(t * 100);
      //Set temperature Characteristic value and notify connected client
      temperatureCharacteristic.setValue(temperature);
      temperatureCharacteristic.notify();
      #ifdef DEBUG_BLE
      static char temperatureCTemp[6];
      dtostrf(t, 6, 2, temperatureCTemp);
      sprintf (p_buff,"Temp. Celsius: %.1f C - %s", t, temperatureCTemp);
      proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)p_buff, COM_PORT_USB);
      #endif

      #ifdef DEBUG_BLE
      free(p_buff);
      #endif
    }
  }
  else 
  {//no connection
    if (ble_task.loc_time)
    {
      ble_task.loc_time = 0;//set timer for next connection
//      ble_task.set_idx(SERVER_INIT_IDX);
      #ifdef DEBUG_BLE
      char* p_buff = (char*)malloc(200 * sizeof(char));
      sprintf (p_buff,"BLE_task_run BLE disconnect");
      proto_tx_info(BLE_SERVER_TYPE_CHAR, (char*)p_buff, COM_PORT_USB);
      free(p_buff);
      #endif
    }
  }

}
