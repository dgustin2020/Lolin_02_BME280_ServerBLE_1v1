/*
battery_gest.cpp
*/
#include  <common_include.h>
//#include  <My_Enable_Debug.h>
#include  <my_Batt_Drv.h>
#include  "battery_task.h"
#include  "../Proto_Serial/proto_serial_command.h"

#define LOC_H_DEBUG "BATTERY_TASK "
my_Batt_Drv  Litio;//crea driver lettura batteria
My_Task_Gest batt_task;//crea task gestione lettura batteria
T_batt_state Batt_Litio;
#define V_BATT_MIN            2900 //in millivolt tensione batteria scarica
#define V_BATT_MAX            4200 // in milliVolt tnsione batteria carica
#define V_BATT_STEP           ((V_BATT_MAX - V_BATT_MIN) / 100)
#define V_BATT_NO_PRESENT     4300  //in millivolt tensione batteria non collegata

static T_Lclk batt_Lclk;    // timer per evere lettura periodica tensione batteria
static T_Lclk charge_Lclk;  // timer per avere lettura periodica stato ricarica batteria
static T_Lclk usb_232_Lclk; // timer invio su batteria su usb_232, se == -1, non ivia

#define READ_BATT_DELAY       1000  //in mSec
#define READ_CHAREG_DELAY     1000  //in mSecS
#define WRITE_USB_BATT_DELAY  10000  //in mSec

/** procedure **/

/*
calcola percento batteria
*/
static int percento_batt(T_batt_state batt)
{ 
  if (batt.recharge)
    return 100;
  else
    return ((batt.v_batt - batt.v_batt_min) / V_BATT_STEP);
}

/* iizializza task lettura batteria e ricarica 
return:
true  -> init error
false .> init ok
*/
void battery_task_init(void)
{
  #ifdef DEBUG_BATT
    Serial.println(LOC_H_DEBUG"battery_task_init");
  #endif
  //inizializza dac lettura tensione e input lettura recharge
  Litio.init(PIN_IN_ADC_BATT_VOLT, PIN_IN_BATT_CHARGE, BATT_VOLT_RATE);
  batt_Lclk = 0;
  charge_Lclk = 0;
  usb_232_Lclk = 0;
  Batt_Litio.v_batt = -1;//no lettura
  Batt_Litio.recharge = false;//non recharge
  Batt_Litio.com_delay = WRITE_USB_BATT_DELAY;
  Batt_Litio.com_sel = -1;
  Batt_Litio.recharge_delay = READ_CHAREG_DELAY;
  Batt_Litio.batt_delay = READ_BATT_DELAY;
  Batt_Litio.v_batt_min = V_BATT_MIN;
  Batt_Litio.v_batt_max = V_BATT_MAX;
  Batt_Litio.v_batt_no_present = V_BATT_NO_PRESENT;
  Batt_Litio.v_batt_percento = 0;
}


/* task gestione btteria
  a comando e a tempo, 
  lettura tensione
  stato ricarica
  per adessogestisco solo a tempo
*/
void battery_task_run(void)
{
  if (Base_Lclk > batt_Lclk){//test scaduto delay lettura tensione
    #ifdef DEBUG_BATT
    Serial.println(LOC_H_DEBUG"battery_task_run read v_batt");
    #endif
    batt_Lclk += READ_BATT_DELAY;
    Batt_Litio.v_batt = Litio.voltage();//legge e salva tensione batteria
    Batt_Litio.v_batt_percento = percento_batt(Batt_Litio);
    if (Batt_Litio.v_batt >= V_BATT_NO_PRESENT)
      Batt_Litio.v_batt_no_present = true;
    else  
      Batt_Litio.v_batt_no_present = false;
  }
  if (Base_Lclk > charge_Lclk){  //test se scaduto delay test recharge
    #ifdef DEBUG_BATT
      Serial.println(LOC_H_DEBUG"battery_task_run read recharge");
    #endif
    charge_Lclk += READ_CHAREG_DELAY;
    Batt_Litio.recharge = Litio.status_charge();//legge e salva stato recharge
  }

  //test se scaduto delay invio su usb_232
  if (Batt_Litio.com_sel == -1){
    usb_232_Lclk = Base_Lclk + Batt_Litio.com_delay;
  }
  else { 
    if (Base_Lclk > usb_232_Lclk){
      usb_232_Lclk += Batt_Litio.com_delay;
      if (Batt_Litio.com_sel != -1)
        proto_invia_cmd_b(Batt_Litio.com_sel);//invia su usb232
    }
  }

}
