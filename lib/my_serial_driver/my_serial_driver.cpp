/*
my_serial_driver.cpp
*/
#include "my_serial_driver.h"

/****
* incrementa con wrap il ptr del buffer passato
* exit --> true  == si wrap buffer
* exit --> false == no wrap buffer
****/
bool My_Serial_Drv::incre_ptr(uint8_t* *ptr, uint8_t* start_buff, uint16_t len_buff)
{
  if (++*ptr >= (start_buff + len_buff)){
    *ptr = start_buff;
    return true;
  }
  return false;
}

/*********************************************************************************/
/* procedure pubbliche */
/*********************************************************************************/

/****ricevuto un char gestione dello stesso****/
void My_Serial_Drv::rx_char(uint8_t dato)
{
    #ifdef DEBUG_BLUE
    //Serial.printf("char= %c\r\n",dato);
    #endif

   switch (dato){
    case LF: // se LF, butto il carattere
      break;
    case CR:  // se CR, finita stringa, muovo i pointer
    #ifdef DEBUG_BLUE
    //Serial.println("char= CR gestione");
    #endif
      *P_rx_wr = MIO_NULL;// metti fine stringa
      incre_ptr(&P_rx_wr, &A_rx_buff[0], SERIAL_BUFF);
      P_rx_mk = P_rx_wr;// sposta mk per indicare nuova stringa
      break;
    default: // carattere normale, lo metto nel buffer
      *P_rx_wr = dato;// salvo char & incre wr pointer
      incre_ptr(&P_rx_wr, &A_rx_buff[0], SERIAL_BUFF);
      if (P_rx_wr == P_rx_rd)  // test se wr_pointer ha raggiunto rd_pointer
        incre_ptr(&P_rx_rd, &A_rx_buff[0], SERIAL_BUFF);
      if (P_rx_mk == P_rx_wr)
        incre_ptr(&P_rx_mk, &A_rx_buff[0], SERIAL_BUFF);
      break;
  }
}

/********/
My_Serial_Drv::My_Serial_Drv()
{
  P_rx_wr     = A_rx_buff;
  P_rx_rd     = A_rx_buff;
  P_rx_mk     = A_rx_buff;
  P_tx_wr     = A_tx_buff;
  P_tx_rd     = A_tx_buff;
  P_tx_mk     = A_tx_buff;
  tx_start    = FALSE;
}

/**** tx una stringa ****/
uint8_t My_Serial_Drv::load_tx_my_string(uint8_t *P_char)
{
 while (*P_char != MIO_NULL)
  {
    *P_tx_wr = *P_char++; // copia char
    incre_ptr(&P_tx_wr, &A_tx_buff[0], SERIAL_BUFF); 
    if (P_tx_wr == P_tx_rd)
      incre_ptr(&P_tx_rd, &A_tx_buff[0], SERIAL_BUFF); 
  }
  P_tx_mk = P_tx_wr;
  return TRUE;
}

/**** tx una stringa con CR e LF finale ****/
uint8_t My_Serial_Drv::load_tx_my_string_CR_LF(uint8_t *P_char)
{
 while (*P_char != MIO_NULL)
  {
    *P_tx_wr = *P_char++; // copia char
    incre_ptr(&P_tx_wr, &A_tx_buff[0], SERIAL_BUFF); 
    if (P_tx_wr == P_tx_rd)
      incre_ptr(&P_tx_rd, &A_tx_buff[0], SERIAL_BUFF); 
  }
  *P_tx_wr++ = CR; // copia char CR
  *P_tx_wr++ = LF; // copia char LF
  P_tx_mk = P_tx_wr;
  return TRUE;
}
/********/
uint8_t My_Serial_Drv::get_rx_str(uint8_t* dove)
{
  if (P_rx_rd == P_rx_wr)
    return FALSE;// segnalo no dati
  do{
    if (*P_rx_rd == MIO_NULL){// test se stringa presente
      incre_ptr(&P_rx_rd, &A_rx_buff[0], SERIAL_BUFF);
      return FALSE; // segnalo no dati
    }
    *dove++ = *P_rx_rd;
    incre_ptr(&P_rx_rd, &A_rx_buff[0], SERIAL_BUFF);
    if (P_rx_rd == P_rx_wr)
      break;// exit se fine dati
  }while(*P_rx_rd > MIO_NULL);
  incre_ptr(&P_rx_rd, &A_rx_buff[0], SERIAL_BUFF);
  *dove = MIO_NULL; // metto EndOfString
  return TRUE;// segnalo dati
}
/********/
uint8_t My_Serial_Drv::get_tx_str(uint8_t* dove)
{
  if (P_tx_rd == P_tx_wr)
    return FALSE;// segnalo no dati
  do{
    if (*P_tx_rd == MIO_NULL){// test se stringa presente
      incre_ptr(&P_tx_rd, &A_tx_buff[0], SERIAL_BUFF); //primo char == MO_NULL, skip MIO_NULL
      return FALSE; // segnalo no dati
    }
    *dove++ = *P_tx_rd;//copia char
    incre_ptr(&P_tx_rd, &A_tx_buff[0], SERIAL_BUFF); //incre read ptr
    if (P_tx_rd == P_tx_wr)//test se read raggiunto write
      break;// raggiunto, exit se fine dati
  } while(*P_tx_rd > MIO_NULL);//copia fino a MIO_NULL
  *dove = MIO_NULL; // metto EndOfString
  return TRUE;// segnalo dati
}
/****test se ricevuta stringa ****/
uint8_t My_Serial_Drv::test_rx_str(void)
{
  if (P_rx_rd == P_rx_mk)
    return FALSE;
  else
    return TRUE;
}
/**** test se nuova stringa da trasmettere ****/
uint8_t My_Serial_Drv::test_tx_str(void)
{
  if (tx_start == FALSE){
    if (P_tx_rd == P_tx_mk)
      return FALSE;
    else{
      return TRUE;
    }
  }
  return FALSE;
}
/****/
void My_Serial_Drv::init(T_Lclk* p_time)
{
  P_rx_wr     = A_rx_buff;
  P_rx_rd     = A_rx_buff;
  P_rx_mk     = A_rx_buff;
  P_tx_wr     = A_tx_buff;
  P_tx_rd     = A_tx_buff;
  P_tx_mk     = A_tx_buff;
  tx_start    = FALSE;
  p_loc_time  = p_time;
}


#if 0
/**********************************************************************************/
/****
 * tx stringa
****/
bool My_Serial_Drv::usb_tx_string(void)
{
  if (get_tx_str(print_tx_buff)){//estragggo stringa da inviare
    if (p_usb->availableForWrite()){
      p_usb->printf("%s\r",print_tx_buff);//out stringg
      return true;//info tx OK  
    }
    return false;
  }
  return false;// info tx NO OK
}
/****/
void My_Serial_Drv::usb_init(HardwareSerial* p_HWserial, T_Lclk* p_time)
{
  P_rx_wr = A_rx_buff;
  P_rx_rd = A_rx_buff;
  P_rx_mk = A_rx_buff;
  P_tx_wr = A_tx_buff;
  P_tx_rd = A_tx_buff;
  P_tx_mk = A_tx_buff;
  tx_start = FALSE;
  p_usb = p_HWserial;
  p_usb->begin(115200);
  p_usb->println("Start p Seriale");
}
/****/
void My_Serial_Drv::blue_init(BluetoothSerial* p_HWserial, T_Lclk* p_time)
{
  P_rx_wr = A_rx_buff;
  P_rx_rd = A_rx_buff;
  P_rx_mk = A_rx_buff;
  P_tx_wr = A_tx_buff;
  P_tx_rd = A_tx_buff;
  P_tx_mk = A_tx_buff;
  tx_start = FALSE;

  blue_status = BLUE_STATUS_NONE;
  p_blue = p_HWserial;
  p_blue->begin(BLUE_NAME); //Bluetooth device name
  p_blue->setPin(BLUE_PIN); //bluetooth pin value
  //p_blue->register_callback( BT_Eventhandler ); // Attach The CallBack Function To SerialBlutooth Event
  
}
/****/
void My_Serial_Drv::usb_rx_task(void)
{
  if (p_usb->available()){//test se rx char
    //rx char, metti in buffe
    size_t l = p_usb->available();
    uint8_t dato;
    l = p_usb->read((char*)&dato, l);
    rx_char(dato);
  }
}
/*
procedura chiamata da itr generato evento bluetooth
*/
static uint8_t blue_status;
void BT_EventHandler(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) 
{
  blue_status = event;//set status driver blue;
#ifdef DEBUG_BLUE
  if (event == ESP_SPP_START_EVT) {
    Serial.println("Initialized SPP");
  }
  else if (event == ESP_SPP_SRV_OPEN_EVT ) {
    Serial.println("Client connected");
  }
  else if (event == ESP_SPP_CLOSE_EVT  ) {
    Serial.println("Client disconnected");
  }
  else if (event == ESP_SPP_DATA_IND_EVT ) {
    Serial.println("Data received");
  }
  else if (event == ESP_SPP_WRITE_EVT ) {
    Serial.println("Data sending");
  }
#endif
}
#endif


