/*
bme280_task
*/
#ifndef _BME280_TASK_H
#define _BME280_TASK_H

#define AUTOEMIT_DELAY_NONE         0 // no autoemit
#define AUTOEMIT_DELAY_MIN          0 // secondi
#define AUTOEMIT_DELAY_MAX        200 // secondi

extern void     bme280_task_init(void);
extern void     bme280_task_run(void);
extern bool     bme280_task_autoemit_port_set(char port_val);
extern bool     bme280_task_autoemit_delay_set(int delay_val);
extern int      bme280_task_autoemit_delay_get(void);
extern void     bme280_task_enable_disable_set(uint8_t enable_val);
extern uint8_t  bme280_task_enable_disable_get(void);

extern float    bme280_task_read_temp(void);
extern float    bme280_task_read_humy(void);
extern float    bme280_task_read_press(void);
extern float    bme280_task_read_alti(void);


#endif //_BME280_TASK_H