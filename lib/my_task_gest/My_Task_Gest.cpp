/*
My_Task.cppp
*/
//#include <my_include.h>
#include <Arduino.h>
#include <my_define.h>
#include <variabili_base.h>
#include <my_itr_time.h>
#include <usb_gest.h>
#include <../my_task_gest/My_Task_Gest.h>

My_Task_Gest::My_Task_Gest()
{
  _idx = 0;
  _mask = 0;
  _tbl_ptr = 0;
  _Pfn = 0;
  _p_time = 0;
  loc_time = -1;
  task_active = false;
}

void My_Task_Gest::init(uint8_t idx, uint8_t mask, FnPtr* tbl_ptr, T_Lclk* p_time)
{
  #ifdef MY_TASK_GEST_DEBUG
  Serial.println("my_task_gest init");
  #endif
  _idx = idx;
  _mask = mask;
  _tbl_ptr = tbl_ptr;
  _Pfn = *_tbl_ptr[_idx];
  _p_time = p_time;
  if (_p_time)
    loc_time = *_p_time;
  else
    loc_time = -1;
  task_active = true;//abilita task
}

bool My_Task_Gest::init_and_run(uint8_t idx, uint8_t mask, FnPtr* tbl_ptr, T_Lclk* p_time)
{
  #ifdef MY_TASK_GEST_DEBUG
  Serial.println("my_task_gest init end run");
  #endif
  _mask = mask;
  _idx = (idx & _mask);
  _tbl_ptr = tbl_ptr;
  _Pfn = *_tbl_ptr[_idx];
  _p_time = p_time;
  task_active = true;//abilita task
  /* test se procedura indicizzata esiste, di norma sempre !!!!*/
  if (!_tbl_ptr[_idx]){
    #ifdef MY_TASK_GEST_DEBUG
    Serial.println("my_task_gest int_and_run err: no function");
    #endif
    return false;//return Error
  }
  _Pfn = _tbl_ptr[_idx];
  #ifdef MY_TASK_GEST_DEBUG
  //Serial.printf("my_task_gest int_and_run goto funcction %ld \r\n", _Pfn);
  #endif

  _Pfn();//lancia la procedura indicizzata
  return true;//return OK
}

bool My_Task_Gest::run(void)
{
  #ifdef MY_TASK_GEST_DEBUG
  Serial.println("my_task_gest run");
  #endif
  if (! _tbl_ptr[_idx]){//test se esiste indirizzo procedura
    #ifdef MY_TASK_GEST_DEBUG
    Serial.println("my_task_gest next_and_run: err no function");
    #endif
    return false;
  }
  #ifdef MY_TASK_GEST_DEBUG
  //Serial.print("my_task_gest next_and_run: si function _idx= ");
  //Serial.println(_idx);
  #endif
  _Pfn = _tbl_ptr[_idx];//load ptr alla procedura
  _Pfn();// chiamata procedura indicizzata
  return true;
}

bool My_Task_Gest::next_and_run(void)
{
  bool result = true;
  _idx = (++_idx & _mask);
  if (! _tbl_ptr[_idx]){//test se indice in range
    #ifdef MY_TASK_GEST_DEBUG
    Serial.println("my_task_gest next_and_run: clear _idx");
    #endif
    _idx = 0;//punta la prima procedura della tabella
    result = false;//segnala Errore
  }
  if (! _tbl_ptr[_idx]){//verifica se ptr ptocedura impostato
    #ifdef MY_TASK_GEST_DEBUG
    Serial.println("my_task_gest next_and_run: err no function");
    #endif
    return false;//return Errore, ptr procedura non impostato
  }

  #ifdef MY_TASK_GEST_DEBUG
  //Serial.print("my_task_gest next_and_run: si function _idx= ");
  //Serial.println(_idx);
  #endif

  _Pfn = _tbl_ptr[_idx];//load ptr ptocedura
  _Pfn();//lancia la procedura indicizzata
  return result;//return OK--> indice in range - Error--> indice fuori range
}

bool My_Task_Gest::next(void)
{
  bool result = true;
  _idx = (++_idx & _mask);
  if (! _tbl_ptr[_idx]){
    #ifdef MY_TASK_GEST_DEBUG
    Serial.println("my_task_gest next_and_run: clear _idx");
    #endif
    _idx = 0;
    result = false;
  }
  if (! _tbl_ptr[_idx]){
    #ifdef MY_TASK_GEST_DEBUG
    Serial.println("my_task_gest next_and_run: err no function");
    #endif
    return false;//return error
  }
  return result;//return OK--> indice in range - Error--> indice fuori range
}

uint8_t My_Task_Gest::get_idx(void)
{ return _idx;  }

bool My_Task_Gest::set_idx(uint8_t idx)
{
  _idx = (idx & _mask);
  if (! _tbl_ptr[_idx]){//test se esiste funzione all'indice impostato
    #ifdef MY_TASK_GEST_DEBUG
    Serial.println("my_task_gest next_and_run: err no function");
    #endif
    return false;
  }
  return true;
}

void My_Task_Gest::task_enable(void)
{
  task_active = true;
}

void My_Task_Gest::task_disable(void)
{
  task_active = false;
}

bool My_Task_Gest::task_get_state(void)
{
  return task_active;
}