/*
usb_gest.h
*/
#ifndef USB_GEST_H
#define USB_GEST_H
#include <my_serial_driver.h>

extern My_Serial_Drv usb_232;

extern void usb_gest_init(T_Lclk *p_clk);
extern void usb_gest_run(void);
extern void usb_gest_rx_task(void);

#endif
