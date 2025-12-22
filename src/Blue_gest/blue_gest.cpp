/*
blue_common.cpp
*/
#include <Arduino.h>
#include <my_define.h>
#include <variabili_base.h>
#include <my_serial_driver.h>
#include <BluetoothSerial.h>
#include  <proto.h>
#include  <proto_common.h>
#include  <my_itr_time.h>
#include  "../../src/preferenze_gest/pref_gest.h"
#include "blue_gest.h"

/* varaibili comuni */
BluetoothSerial SerialBlue;
My_Serial_Drv blue_232; //canale seriale usato per gestione rx/tx dati su bluetooth

char blue_device_name_local[50];//nome del device locale
char blue_device_name_remote[50];//nome del device remoto

uint8_t       blue_status;
bool          blue_slave_master = BLUE_SET_SLAVE;
bool          blue_task_enable = false;

static void blue_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);

/*
verifica se dati su rx bluetooth
se ho dati, leggo char e lo metto nel buffer
da chimare nel main loop in modo da leggere il prima possibile*/
void blue_rx_char(void)
{
  if (SerialBlue.available()){   //test se rxdati
    blue_232.rx_char(SerialBlue.read());//read & save dato in buffer
  }
}

/*
start driver bletooth e set callback procedure
inizializza la gestione tx/rx dati su canale bluetooth
a seconda di 'slave_master', imposta il funzionamento come slave o come master
*/
bool blue_task_init(bool slave_master)
{
#ifdef DEBUG_BLUE  
  Serial.println("start blue");
#endif
  bool result;
  bool test;
  blue_slave_master = slave_master;
  blue_status = BLUE_STATUS_NONE;//set flag gemerico
  blue_232.init((T_Lclk*)Base_Lclk);//apri canale comunicazioni bluetooth

  /* attiva servizio blue a seconda del tipo impostato */
  if (blue_slave_master){
    //imposta master
    /*crea nome device bluetooth unendo nome_blue_slave e numero di serie*/
    sprintf(blue_device_name_local,"%s #%s",base_param.blue_name_master, base_param.dev_serie);
    #ifdef DEBUG_BLUE  
    Serial.printf("blue name= %s ",blue_device_name_local);
    if (SerialBlue.begin(blue_device_name_local)){
      Serial.println("blue master begin ok ");
      result = true;
    }
    else { 
      Serial.println("begin master err");
      return false;
    }
    #else
    if ( !SerialBlue.begin(blue_device_name_local, true) ){ //start master Bluetooth device name
      Serial.println("========== serialBT master failed!");
      return false;
    }
    #endif
  }
  else {
    //imposta slave
    /*creo nome device bluetooth unendo nome_blue_slave e numero di serie*/
    sprintf(blue_device_name_local,"%s #%s",base_param.blue_name_slave, base_param.dev_serie);
    #ifdef DEBUG_BLUE  
    Serial.printf("blue name= %s ",blue_device_name_local);
    if (SerialBlue.begin(blue_device_name_local)){
      Serial.println("blue slave begin ok ");
      result = true;
    }
    else {
      Serial.println("begin slave err");
      return false;
    }
    #else
    if ( !SerialBlue.begin(blue_device_name_local))
    { //start slave Bluetooth device name
      Serial.println("========== serialBT slave failed!");
      return false;
    }
    #endif
    blue_task_enable = true;//se slave, abilita subito task gestione rx/tx dati
  }
  SerialBlue.setPin(base_param.blue_pin_slave); //bluetooth pin value
  #ifdef DEBUG_BLUE
  Serial.println("Blue End init");
  #endif
  /* imposta procedura di callback*/
  SerialBlue.register_callback(blue_callback); // Attach The CallBack Function Definition To SerialBlutooth Events
  return true;
}

/*
task gestione stringhe ricevute e da trasmettere
*/
void blue_task_run(void)
{
  if (blue_task_enable){
    //gestione ricezione rs232 via bluetooth
    char* p_buff = (char*)malloc(SERIAL_BUFF * sizeof(char));
    if (blue_232.test_rx_str()){//test se ricevuto striga
      blue_232.get_rx_str((uint8_t*)p_buff); //ricevuta stringa, estrai
      proto_gest((char*)p_buff, COM_PORT_BLUE); //gestione stringa ricevuta in rs232_rx_buff
    }
    #if 0
    if (blue_232.test_rx_str()){//test se ricevuto striga
      blue_232.get_rx_str((uint8_t*)&blue_232.rx_buff); //ricevuta stringa, estrai
      proto_gest((char*)&blue_232.rx_buff, COM_PORT_BLUE); //gestione stringa ricevuta in rs232_rx_buff
    }
    #endif
    //gestione trasmissione rs232 via bluetooth
    if (blue_232.test_tx_str()){//test se ho una stringa da trasmettere
      if (blue_232.get_tx_str((uint8_t*)p_buff)){//estragggo stringa da inviare
        SerialBlue.print((char*)p_buff);//out stringg
      }
    }
    #if 0
    if (blue_232.test_tx_str()){//test se ho una stringa da trasmettere
      if (blue_232.get_tx_str((uint8_t*)&blue_232.tx_buff)){//estragggo stringa da inviare
        SerialBlue.print((char*)&blue_232.tx_buff);//out stringg
      }
    }
    #endif
    free(p_buff);
  }
}

/*
fine driver bluetooth
aspetta disconnessione, se connesso, pi chiude il driver
exit
True == attesa disconnessio
false == canale chiuso
*/
bool blue_task_end(void)
{
  if (SerialBlue.connected()){//test se connesso
    #ifdef DEBUG_BLUE
    Serial.printf("sono connesso %i\r\n", blue_status);
    #endif
    SerialBlue.disconnect();
    #ifdef DEBUG_BLUE
    Serial.printf("disconnetti %i\r\n", blue_status);
    #endif
    return true;
  }
  #ifdef DEBUG_BKUE
  else {
    Serial.printf("sono disconnesso %i\r\n", blue_status);
  
  Serial.printf("end blue slave blue_status= %i\r\n",blue_status);
  #endif
  SerialBlue.end();
  blue_232.init((T_Lclk*)Base_Lclk);//cler sll driver
  blue_status = BLUE_STATUS_NONE;
  blue_task_enable = false;//disable task blue_slave
  return false;
}

/** private ****************************************************/
#define CALLBACK_MAX_EVENT          35
static char* Ap_text[] ={
  (char*)"ESP_SPP_INIT_EVT ",           // 0  ESP_SPP_INIT_EVT
  (char*)"ESP_SPP_UNINIT_EVT ",         // 1  ESP_SPP_UNINIT_EVT
  (char*)"dummy ",                      // 2
  (char*)"dummy ",                      // 3
  (char*)"dummy ",                      // 4
  (char*)"dummy ",                      // 5
  (char*)"dummy ",                      // 6
  (char*)"dummy ",                      // 7
  (char*)"ESP_SPP_DISCOVERY_COMP_EVT ", // 8  ESP_SPP_DISCOVERY_COMP_EVT
  (char*)"dummy ",                      // 9
  (char*)"dummy ",                      //10
  (char*)"dummy ",                      //11
  (char*)"dummy ",                      //12
  (char*)"dummy ",                      //13
  (char*)"dummy ",                      //14
  (char*)"dummy ",                      //15
  (char*)"dummy ",                      //16
  (char*)"dummy ",                      //17
  (char*)"dummy ",                      //18
  (char*)"dummy ",                      //19
  (char*)"dummy ",                      //20
  (char*)"dummy ",                      //21
  (char*)"dummy ",                      //22
  (char*)"dummy ",                      //23
  (char*)"dummy ",                      //24
  (char*)"dummy ",                      //25
  (char*)"ESP_SPP_OPEN_EVT ",           //26  ESP_SPP_OPEN_EVT
  (char*)"Client disconnected ",        //27  ESP_SPP_CLOSE_EVT
  (char*)"Initialized SPP ",            //28  ESP_SPP_START_EVT
  (char*)"ESP_SPP_CL_INIT_EVT ",        //29  ESP_SPP_CL_INIT_EVT
  (char*)"Data received  ",             //30  ESP_SPP_DATA_IND_EVT
  (char*)"ESP_SPP_CONG_EVT ",           //31  ESP_SPP_CONG_EVT
  (char*)"dummy ",                      //32
  (char*)"Data sending ",               //33  ESP_SPP_WRITE_EVT
  (char*)"Client connected ",           //34  ESP_SPP_SRV_OPEN_EVT
  (char*)"ESP_SPP_SRV_STOP_EVT ",       //35  ESP_SPP_SRV_STOP_EVT  
  (char*)"Error event over range ",     //35  event non conoscuto

  0                                     //36
};

static uint8_t A_flag[] ={
  BLUE_DUMMY,         // 0  ESP_SPP_INIT_EVT
  BLUE_DUMMY,         // 1  ESP_SPP_UNINIT_EVT
  BLUE_DUMMY,         // 2
  BLUE_DUMMY,         // 3
  BLUE_DUMMY,         // 4
  BLUE_DUMMY,         // 5
  BLUE_DUMMY,         // 6
  BLUE_DUMMY,         // 7
  BLUE_DUMMY,         // 8  ESP_SPP_DISCOVERY_COMP_EVT
  BLUE_DUMMY,         // 9
  BLUE_DUMMY,         //10
  BLUE_DUMMY,         //11
  BLUE_DUMMY,         //12
  BLUE_DUMMY,         //13
  BLUE_DUMMY,         //14
  BLUE_DUMMY,         //15
  BLUE_DUMMY,         //16
  BLUE_DUMMY,         //17
  BLUE_DUMMY,         //18
  BLUE_DUMMY,         //19
  BLUE_DUMMY,         //20
  BLUE_DUMMY,         //21
  BLUE_DUMMY,         //22
  BLUE_DUMMY,         //23
  BLUE_DUMMY,         //24
  BLUE_DUMMY,         //25
  BLUE_DUMMY,         //26  ESP_SPP_OPEN_EVT
  BLUE_DISCONNECTED,  //27  ESP_SPP_CLOSE_EVT
  BLUE_INIT_SPP,      //28  ESP_SPP_START_EVT
  BLUE_DUMMY,         //29  ESP_SPP_CL_INIT_EVT
  BLUE_DUMMY,         //30  ESP_SPP_DATA_IND_EVT
  BLUE_DUMMY,         //31  ESP_SPP_CONG_EVT
  BLUE_DUMMY,         //32
  BLUE_DUMMY,         //33  ESP_SPP_WRITE_EVT
  BLUE_CONNECTED,     //34  ESP_SPP_SRV_OPEN_EVT
  BLUE_DUMMY,         //35  ESP_SPP_SRV_STOP_EVT
  BLUE_DUMMY,         //36  event non conosiuto
};

/*
*/
static void blue_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) 
{
  if (event > CALLBACK_MAX_EVENT){
    Serial.printf("BLUE_GEST %s %i %i\r\n", Ap_text[CALLBACK_MAX_EVENT+1], event, blue_status);//print info
    return;
  }
  if ( A_flag[event] < BLUE_DUMMY)//test se modificare flag blue_status
    blue_status = A_flag[event];//da modificare
  Serial.printf("BLUE_GEST %s %i %i\r\n", Ap_text[event], event, blue_status);//print info
}

/********************************************************************* */
#if 0//*************** */
  if (event == ESP_SPP_START_EVT) {
    Serial.println("Initialized SPP");
    blue_status = BLUE_INIT_SPP;
  }
  else if (event == ESP_SPP_SRV_OPEN_EVT ) {
    Serial.println("Client connected");
    blue_status = BLUE_CONNECTED;
  }
  else if (event == ESP_SPP_CLOSE_EVT  ) {
    Serial.println("Client disconnected");
    blue_status = BLUE_DISCONNECTED;
  }
  else if (event == ESP_SPP_DATA_IND_EVT ) {
    Serial.println("Data received");
    while (SerialBlue.available()){   //leggo dati da blue rx
      blue_232.rx_char(SerialBlue.read());//salvo dati in buffer
    }
  }
  else if (event == ESP_SPP_WRITE_EVT ) {
    Serial.println("Data sending");
  }
#endif