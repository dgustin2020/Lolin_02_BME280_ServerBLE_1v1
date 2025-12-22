/*
usb_rs232.h
*/

#ifndef MY_SERIAL_DRIVER_H
#define MY_SERIAL_DRIVER_H
#include <my_define.h>
#include "my_serial_driver.h"

#define SERIAL_BUFF    1000

class My_Serial_Drv{
  public:
    /* crea un elemeto tpo my_task_gest*/
    My_Serial_Drv();
    /* inizializza i vari puntatori del driver usb
      p_time = pointer ad un orologio globale per eventuali gestioni a tempo
    */
    void    init(T_Lclk* p_time);
    uint8_t load_tx_my_string(uint8_t *P_char);
    uint8_t load_tx_my_string_CR_LF(uint8_t *P_char);
    uint8_t get_rx_str(uint8_t* dove);
    uint8_t get_tx_str(uint8_t* dove);
    uint8_t test_rx_str(void);
    uint8_t test_tx_str(void);
    void    rx_char(uint8_t dato);

  private:
  /*variabili*/
   uint8_t    A_rx_buff[SERIAL_BUFF+2];
   uint8_t    A_tx_buff[SERIAL_BUFF+2];
   uint8_t*   P_rx_wr;
   uint8_t*   P_rx_rd;
   uint8_t*   P_rx_mk;
   uint8_t*   P_tx_wr;
   uint8_t*   P_tx_rd;
   uint8_t*   P_tx_mk;
   uint8_t    tx_start;
   T_Lclk*    p_loc_time;// se -1, non impostato
  /*procedure*/
   bool incre_ptr(uint8_t* *ptr, uint8_t* start_buff, uint16_t len_buff);
};

#endif