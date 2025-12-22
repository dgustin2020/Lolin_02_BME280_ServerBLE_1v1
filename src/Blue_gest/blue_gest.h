/*
blue_gest.h
*/
#ifndef BLUE_GEST_H
#define BLUE_GEST_H

extern char             blue_device_name_local[];
extern char             blue_device_name_remote[];

extern My_Serial_Drv    blue_232;

extern uint8_t        blue_status;
#define       BLUE_STATUS_NONE    0
#define       BLUE_INIT_SPP       1
#define       BLUE_CONNECTED      2
#define       BLUE_DISCONNECTED   3
#define       BLUE_DUMMY          255

extern bool   blue_slave_master;
#define BLUE_SET_SLAVE    false
#define BLUE_SET_MASTER   true
extern bool   blue_task_enable;

extern void blue_rx_char(void);
extern bool blue_task_init(bool slave_master);
extern void blue_task_run(void);
extern bool blue_task_end(void);

#endif
