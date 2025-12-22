/*
ble_server_task.h
*/
#ifndef _BLE_GEST_H
#define _BLE_GEST_H

/*variabili pubbliche*/
#define BLE_SERVER_TYPE_CHAR                        'B'
#define BLE_DELAY_NOTIFY_DEFAULT                    (5 * L_SEC)//

/*procedure*/
extern void ble_server_task_init(void);
extern void ble_server_task_run(void);

extern void ble_server_task_enable(void);
extern void ble_server_task_disable(void);

#endif //end _BLE_GEST_H
