/*
usb_gest.cpp
*/
#include <Arduino.h>
#include <my_define.h>
#include <proto.h>
#include "usb_gest.h"


#define HWSerial  Serial
My_Serial_Drv usb_232;


/* procedure comuni */

/*
init driver rs232 su usb
*/
void usb_gest_init(T_Lclk *p_clk)
{
  usb_232.init((T_Lclk*)p_clk);
  HWSerial.begin(115200);
  HWSerial.println("USB_GEST Start Task USB-seriale");
}

/*
task a tempo che verifica se ricevuta una stribgs oppure
se bisogna inviarne una nuov
*/
void usb_gest_run(void)
{
  //gestione ricezione rs232 via usb
  char* p_buff = (char*)malloc(SERIAL_BUFF * sizeof(char));
  if (usb_232.test_rx_str()){//test se ricevuto striga
    
    //ricevuta stringa, estrai
    if (usb_232.get_rx_str((uint8_t*)p_buff)){//leggi stringa ricevuta
      //Serial.printf("ricevuta stringa ->%s\r\n",usb_232.rx_buff);
      proto_gest((char*)p_buff, COM_PORT_USB); //gestione stringa ricevuta in rs232_rx_buff
    }
#if 0
    if (usb_232.get_rx_str((uint8_t*)&usb_232.rx_buff)){//leggi stringa ricevuta
      //Serial.printf("ricevuta stringa ->%s\r\n",usb_232.rx_buff);
      proto_gest((char*)&usb_232.rx_buff, COM_PORT_USB); //gestione stringa ricevuta in rs232_rx_buff
    }
#endif

  }
  //gestione trasmissione rs232 via usb
  if (usb_232.test_tx_str()){//test se ho una stringa da trasmetter

    if (usb_232.get_tx_str((uint8_t*)p_buff)){//estragggo stringa da inviare
      if (HWSerial.availableForWrite()){
        HWSerial.print((char*)p_buff);//out stringg
      }
#if 0
    if (usb_232.get_tx_str((uint8_t*)&usb_232.tx_buff)){//estragggo stringa da inviare
      if (HWSerial.availableForWrite()){
        HWSerial.print((char*)&usb_232.tx_buff);//out stringg
      }
#endif
    }
  }
  free(p_buff);
}

/****/
void usb_gest_rx_task(void)
{
  if (HWSerial.available()){//test se rx char
    //rx char, metti in buffe
    size_t l = HWSerial.available();
    uint8_t dato;
    l = HWSerial.read((char*)&dato, l);
    usb_232.rx_char(dato);
  }
}