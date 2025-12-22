/* 
bme280_task.cpp

inizializza pin del sensore
accendi sensore con delay successivo
verifica presenza per alcune volte
se non funziona va in wait_error
se funziona, arma timer lettura dati sensore
veriica scadenza timer
leggi e salva dati sensore, riarma timer, verifica se stop task sensore

*/
#include <common_include.h>
#include <Adafruit_BME280.h>
#include <my_bme280_driver.h>
#include "bme280_task.h"
#include "..\preferenze_gest\pref_gest.h"

#define AUTOEMIT_DELAY_DEFAULT    AUTOEMIT_DELAY_NONE
#define AUTOEMIT_PORT_DEFAULT     COM_PORT_USB

/* procedure locali */
static void dummy(void);
static void init_sensore(void);
static void test_sensore(void);

static void wait_test_sensore(void);
static void set_sensore(void);
static void read_loop(void);

static FnPtr        tbl_fn_Loop[] = {// dich. array puntatori alle funzioni
  init_sensore,         //00 INIT_SENSORE_IDX
  wait_test_sensore,    //01 WAIT_TEST_SENSORE_IDX
  set_sensore,          //02 SET_SENSORE_IDX
  read_loop,            //03 READ_LOOP_IDX
  dummy,                //04
  dummy,                //05
  dummy,                //06
  dummy,                //07
  0
};
#define LOOP_MASK               0x07  //da modificare in funzione delle procedure usate
#define START_FUNCTION_IDX      0x00  //prima funzione chiamata dall'attivazione del task
#define INIT_SENSORE_IDX        START_FUNCTION_IDX
#define WAIT_TEST_SENSORE_IDX   INIT_SENSORE_IDX + 1
#define SET_SENSORE_IDX         WAIT_TEST_SENSORE_IDX + 1
#define READ_LOOP_IDX           SET_SENSORE_IDX + 1

My_Task_Gest sens_task;           //crea gestione  main_loop

T_sensor_read sensor_read;      /* dati sensore */
#define DELAY_READ_RATE         (200 * L_MIL)
#define BME280_TASK_ENABLE_DEFAULT  1  //abilita il task letture sensore

/************************************************* */
/* pubbliche */
/*
start task bme280
*/
void bme280_task_init(void)
{
  /* inizializza loop */
  proto_tx_info('S', (char*)"BME280_task init\r\n", COM_PORT_USB);
  sens_task.task_enable();
  sens_task.init_and_run(START_FUNCTION_IDX, LOOP_MASK, tbl_fn_Loop, (T_Lclk*)&Base_Lclk);//inizializza main_loop
}

/*
loop sensore bme280
*/
void bme280_task_run(void)
{
  if (sens_task.task_get_state())
    sens_task.run();
}

/*
imposta il com_port dove inviare i dati se autoemit abilitato
false se valore ok
true  se fuori range
*/
bool bme280_task_autoemit_port_set(char port_val)
{
  if (port_val > COM_PORT_BLUE)
    return true;//errore port non valido
  sensor_read.autoemit_port = port_val;
  if (sensor_read.autoemit_delay_val)
    sensor_read.autoemit_Lclk = Base_Lclk + sensor_read.autoemit_delay_val;
  else
    sensor_read.autoemit_Lclk = 0;
  return true;//tutto ok
}

extern T_pref_record* tbl_user_param_rec[];

/*
imposta il valore del timer di autoemit
se valore compreso tra AUTOEMIT_DELAY_MIN e AUTOEMIT_DELAY_MAX
imposta, arma prossimo evento esce con false
altrimnti non fa nulla ed esce con true
*/
bool bme280_task_autoemit_delay_set(int delay_val)
{
  bool result;
  if (delay_val == AUTOEMIT_DELAY_NONE)
  {//spengo autoemit
    sensor_read.autoemit_delay_val = 0;
    sensor_read.autoemit_Lclk = 0;
    result = false;
  }
  else 
  {
    if ( (delay_val >= AUTOEMIT_DELAY_MIN) && (delay_val <= AUTOEMIT_DELAY_MAX) )
    {//valore in range, imposto valore e nuova scadenza
      sensor_read.autoemit_delay_val = delay_val;//nuovo valore
      sensor_read.autoemit_Lclk = Base_Lclk + (sensor_read.autoemit_delay_val * L_SEC);//nuova scadenza
      result = false;
    }
    else 
    {//valore non in range
      result = true;
    }
  }
  if (!result)
  {//parametro modificato salvo il space_flash
    user_param.sens_autoemit_delay = delay_val;;
    pref_gest_save_param_to_space((T_pref_record*)tbl_user_param_rec[PREF_USER_SENS_AUTOEMIT_DELAY_IDX], (Preferences*)&impo_user_space);
  }
  return result;
}

/*
esce con il avlore del delay autoemit
*/
int bme280_task_autoemit_delay_get(void)
{  return sensor_read.autoemit_delay_val; }

/*
abilita il task di lettura dati sensore
non credo serva a nulla, ma lo implemento comunque
*/
void bme280_task_enable_disable_set(uint8_t enable_val)
{ sensor_read.task_enable = enable_val; 
  user_param.sens_task_enable = enable_val;
    pref_gest_save_param_to_space((T_pref_record*)tbl_user_param_rec[PREF_USER_SENS_TASK_ENABLE_IDX], (Preferences*)&impo_user_space);
}

/*
esce con lo stato di task enable
*/
uint8_t bme280_task_enable_disable_get(void)
{ return sensor_read.task_enable; }

/*
return valore temperatura
*/
float bme280_task_read_temp(void)
{ return sensor_read.temp;  }

/*
return valore humidita
*/
float bme280_task_read_humy(void)
{ return sensor_read.humy;  }

/*
return valore pressione
*/
float bme280_task_read_press(void)
{ return sensor_read.press;  }

/*
return valore temperatura
*/
float bme280_task_read_alti(void)
{ return sensor_read.alti;  }

/**************************************************************************** */
/* locali */
static void dummy(void)
{ 
  return;
}

/*
init pin del sensore e on supply
*/
static void init_sensore(void)
{
  #ifdef DEBUG_BME280
  proto_tx_info('S', (char*)"BME280_task init_sensore\r\n", COM_PORT_USB);
  #endif
  sensor_read.new_read            = false;
  sensor_read.alti                = -1;
  sensor_read.press               = -1;
  sensor_read.temp                = -1;
  sensor_read.humy                = -1;
  sensor_read.time_stamp          = L_NO_TIME;
  sensor_read.status              = BME280_STATUS_NONE;
  sensor_read.delay_read_val      = DELAY_READ_RATE;  
  sensor_read.autoemit_port       = AUTOEMIT_PORT_DEFAULT;
  sensor_read.task_enable         = user_param.sens_task_enable; //BME280_TASK_ENABLE_DEFAULT;
  sensor_read.autoemit_delay_val  = user_param.sens_autoemit_delay; //AUTOEMIT_DELAY_DEFAULT;
  sensor_read.autoemit_Lclk       = 0;

  user_param_write_flag &= ~USER_SENS_WRITE;//clear flag

  if (bme280_init_sensor())//attiva e test se sensore ok
  {//sensore attivo, passa impostazione parametri
    sens_task.loc_time = Base_Lclk + (100 * L_MIL);
    sens_task.set_idx(SET_SENSORE_IDX);
  }
  else 
  {//non attivo, riprova dopo delay
    sens_task.loc_time = Base_Lclk + (100 * L_MIL);
    sens_task.set_idx(WAIT_TEST_SENSORE_IDX);
  }
}

/*
attende timeout
poi ritenta test sensore
se non attivo  riarma timeout
se trovato, passa set parametri
*/
static void wait_test_sensore(void)
{
  if (Base_Lclk > sens_task.loc_time)
  {//scaduto timeout
    sens_task.loc_time += 500 * L_MIL;
    #ifdef DEBUG_BME280
    proto_tx_info('S', (char*)"BME280_task wait_test_sensore\r\n", COM_PORT_USB);
    #endif
    if (bme280_sensor_test())
    {//trovato sensore
      //#ifdef DEBUG_BME280
      proto_tx_info('S', (char*)"BME280_task SENSOR OK\r\n", COM_PORT_USB);
      //#endif
      sensor_read.status = BME280_STATUS_OK;
      sens_task.loc_time += 100 * L_MIL;
      sens_task.set_idx(SET_SENSORE_IDX);
    }
    else
    {//sensore non risponde, segnalo e rimango in questo loop
      //#ifdef DEBUG_BME280
      proto_tx_info('S', (char*)"BME280_task SENSOR FAULT\r\n", COM_PORT_USB);
      //#endif
      sensor_read.status = BME280_STATUS_ER_NOT_FOUND;
    }
  }
}

/*
imposta parametri del sensore 
e passa al loop di misura
*/
static void set_sensore(void)
{
  if (Base_Lclk > sens_task.loc_time)
  {
    #ifdef DEBUG_BME280
    proto_tx_info('S', (char*)"BME280_task set_sensore\r\n", COM_PORT_USB);
    #endif
    bme280_set_parameter();
    sens_task.loc_time += sensor_read.delay_read_val;
    sens_task.set_idx(READ_LOOP_IDX);
  }
}

/*
legge parametri dal sensore ad intervallo programmato
*/
static void read_loop(void)
{
  if (user_param_write_flag & USER_SENS_WRITE)//test se modificati parametri user
  {//modificati, carico i nuovi nella struttura sensore
    user_param_write_flag &= ~USER_SENS_WRITE;//clear flag
    sensor_read.task_enable         = user_param.sens_task_enable; //BME280_TASK_ENABLE_DEFAULT;
    sensor_read.autoemit_delay_val  = user_param.sens_autoemit_delay; //AUTOEMIT_DELAY_DEFAULT;
    sensor_read.autoemit_Lclk       = 0;
  }

  if (sensor_read.task_enable)
  {//task abilitato
    if (Base_Lclk > sens_task.loc_time)
    {
      sens_task.loc_time += sensor_read.delay_read_val;// riarma timer con valore programmato per lettura valori sensori
      bme280_read_sensor_value((T_sensor_read*)&sensor_read);//leggi & salva valori sensori
      #ifdef DEBUG_BME280
      if (sensor_read.status == BME280_STATUS_ER_NO_MISURE)
        proto_tx_info('S', (char*)"BME280_task read_loop read=NO\r\n", COM_PORT_USB);
      else
        proto_tx_info('S', (char*)"BME280_task read_loop read=YES\r\n", COM_PORT_USB);
      #endif
    }
  
    if (sensor_read.autoemit_delay_val)//test se autoemit abilitato
    {//autoemitt abilitato
      if(Base_Lclk > sensor_read.autoemit_Lclk)
      {//delay autoemit scaduto, reload & tx
        sensor_read.autoemit_Lclk += (sensor_read.autoemit_delay_val * L_SEC);
        if (sensor_read.new_read)
        {//lettura sensore aggiornate, invia autoemit
          sensor_read.new_read = false;// clear flag nuova lettura
          //invio automatico valori misure su ultima driver seriale usato 
          char* p_buff = (char*)malloc(sizeof(char) * 200);
          sprintf(p_buff,"Autoemit T=%.1f H=%.1f P=%.1f A=%.0f, %i\r\n",
            sensor_read.temp, sensor_read.humy, sensor_read.press, sensor_read.alti, sensor_read.time_stamp
          );
          proto_tx_info('S', (char*)p_buff, sensor_read.autoemit_port);
          free(p_buff);
          //invio su BLE, se connessione abilitata???????????????
        }
      }
    }
  }
}

