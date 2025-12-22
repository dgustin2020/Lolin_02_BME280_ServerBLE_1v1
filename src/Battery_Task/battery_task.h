/*
battery_gest.h
*/
#ifndef _BATTERY_TASK_H
#define _BATTERY_TASK_H

typedef struct Batt_Status{
  int     v_batt;             // tensione batteria
  bool    recharge;           // stato ricarica L= No charge / H= Yws charge
  T_Lclk  batt_delay;         // delay lettura tensione batteria
  T_Lclk  recharge_delay;     // delay lttura stato ricarica
  T_Lclk  com_delay;          // delay invio dati batteria su seriale. -1=non invio
  int     com_sel;            // quale seriale a richiesto i dati batteria
  int     v_batt_min;         // valore minimo in millivolt tensione batteria scarica
  int     v_batt_max;         // valore massimo in millivolt tensione batteria carica
  int     v_batt_no_present;  // valore tensione in millivolt tensione batteria quando non presente
  int     v_batt_percento;    // percentuale di batteria
} T_batt_state;

extern T_batt_state Batt_Litio;

/* procedure */
extern void battery_task_init(void);
extern void battery_task_run(void);

#endif
