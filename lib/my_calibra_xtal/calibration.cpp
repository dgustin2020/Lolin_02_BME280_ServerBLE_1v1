/*
calibration.cpp
*/
#include <Arduino.h>
#include <my_define.h>
#include <variabili_base.h>
#include <my_itr_time.h>
#include <SPIFFS.h>

#ifndef BOARD_LOLIN_02
	#if USE_LVGL_TFT_ESPI == 1
	#else
	#include <my_tft_lib.h>//
	#endif
#endif

#include <calibration.h>

T_CalSet calset;

static portMUX_TYPE timerMux_itr_cal_xtal = portMUX_INITIALIZER_UNLOCKED;
static uint64_t cal_read_counter       = 0;
static uint64_t cal_read_counter_start = 0;
static uint64_t cal_read_counter_stop  = 0;
static uint64_t cal_tmr_debouncer      = 0;
static uint16_t cal_cnt_seconds        = 0;
static uint64_t n_clock_x_sec;
static uint64_t old_itr_time;
static uint64_t tmp;
static bool     cal_start_misure       = false;
static bool			cal_flag_read_input			= false;
//static T_Lclk		deb_Lclk;

/**** sviluppo ****/
/****
 * itr su fronte discesa ingresso
 * leggo timer che genera itr1000 
 * uso stesso timer pr fare il debouncer della linea
 * */
static void IRAM_ATTR lib_cal_xtal_itr()
{
	portENTER_CRITICAL_ISR(&timerMux_itr_cal_xtal);
	uint64_t tmp = itr_time_read_counter();
	if (tmp > cal_tmr_debouncer){
		cal_tmr_debouncer = tmp + (500 * TIC_TIMER_1MSEC);
		cal_read_counter = tmp;
	}
#if 0
	if (Base_Lclk > deb_Lclk){
		deb_Lclk = Base_Lclk + 500*L_MIL;
		cal_read_counter = tmp;
	}
#endif
	portEXIT_CRITICAL_ISR(&timerMux_itr_cal_xtal);
}

/****
 * abilita ingresso impulso calibrazione
 * disabilita eventuale precedent procedura itr del pim
 * abiliita procedura itr del pin
 * blocca il sistema in attesa del apposito comando
 * */
void lib_cal_xtal_init(uint16_t pin)
{
	if (pin > 40){
		Serial.printf("cal_xtal_pin_error %d\r", pin);
		return;
	}
		
	pinMode(pin, INPUT_PULLUP);
	detachInterrupt(pin);//disabilita eventuale precedente procedura itr
  attachInterrupt(pin, lib_cal_xtal_itr, FALLING);//abilita procedura itr corretta
	cal_start_misure = false;
	cal_flag_read_input = 0;
	
}

/****/
void lib_cal_xtal_start(void)
{
	cal_start_misure = true;
	cal_cnt_seconds = 0;
	cal_read_counter_start = 0;
	cal_read_counter = 0;
	cal_tmr_debouncer = 0;
	//deb_Lclk = 0;
}

bool lib_cal_get_flag_read_input(void)
{	return cal_flag_read_input; }
void lib_cal_clr_flag_read_input(void)
{ cal_flag_read_input = 0; }
uint16_t lib_cal_get_cnt_seconds(void)
{ return cal_cnt_seconds; }


/****
 * exit true = fine misura con modifica parametri
 * exit fals = misura non finita
 * */
bool lib_cal_xtal_run(T_CalSet* p_calset)
{
	if ( (cal_start_misure) && (cal_read_counter) ){//test se abilitata misura e se rilevamento
		cal_flag_read_input = true;
		#ifdef DEBUG_CAL_XTAL
		Serial.printf("cnt = %d\r", cal_cnt_seconds);
		#endif
		if (cal_cnt_seconds < 3){//skip primi rilevamenti per evitare errori
			cal_read_counter_start = cal_read_counter;//inizio misura, save start
		}
		else {//misura in corso, test se fine misura
			if (cal_cnt_seconds >= CAL_SECONDS_X_MISURE){ //test se raggiuta fine misura
				//fine misura, calcolo le variabili di calibrazione
				cal_read_counter_stop = cal_read_counter;//save fine misura
				n_clock_x_sec = ((cal_read_counter_stop - cal_read_counter_start) / 10);// n clock x 1 sec
				//n_clock_x_sec = cal_read_counter_start / 10;
				#ifdef DEBUG_CAL_XTAL
				Serial.printf("stop  = %d\r", (uint32_t)cal_read_counter_stop);    
				Serial.printf("start = %d\r", (uint32_t)cal_read_counter_start);    
				Serial.printf("n_clk_x_S  = %d\r", (uint32_t)n_clock_x_sec);   
				#endif
				p_calset->cal_base = n_clock_x_sec / 1000;
				p_calset->cal_base_1 = (calset.cal_base + 1);
				p_calset->cal_sel = (n_clock_x_sec -(calset.cal_base * 1000));
				#ifdef DEBUG_CAL_XTAL
				Serial.printf("cal_base   = %d\r",   p_calset->cal_base);    
				Serial.printf("cal_base_1 = %d\r",   p_calset->cal_base_1);
				Serial.printf("cal_sel    = %d\r\n", p_calset->cal_sel);
				#endif
				cal_start_misure = false;//fine calibrazione
				cal_read_counter = 0;
				return true;//segnalo fine misura e salvataggio
			}
			#if 0
			else {
				cal_read_counter_start += cal_read_counter;
				#ifdef DEBUG_CAL_XTAL
				Serial.printf("start = %d\r", (uint32_t)cal_read_counter_start);    
				#endif
			}
			#endif
		}

		#ifdef DEBUG_CAL_XTAL
		tmp = cal_read_counter - old_itr_time;
		Serial.printf("Counter: %d\r", (uint32_t) (cal_read_counter));
		Serial.printf("delta:   %d\r\n", (uint32_t) (tmp));
		old_itr_time = cal_read_counter;
		#endif

		cal_cnt_seconds++;
		cal_read_counter = 0;
	}
	return false;//no fine misura
}
#ifndef BOARD_LOLIN_02
/****/
void lib_cal_write_data(T_CalSet* p_calset, uint16_t pos_v, uint16_t pos_h)
{
  uint16_t pos;
  #if USE_LVGL_TFT_ESPI == 1
	#else
	//metti risultati
  sprintf((char*)temp_displ_char,"%6d", p_calset->cal_base);

	displ_clr_row(pos_v);
  pos = displ_str_direct((char*)"Base=    ", pos_v,pos_h);
  displ_str_direct((char*)temp_displ_char, pos_v,pos);

	pos_v++;

  sprintf((char*)temp_displ_char,"%6d", p_calset->cal_base_1);
  displ_clr_row(pos_v);
  pos = displ_str_direct((char*)"Base_1=  ", pos_v,pos_h);
  displ_str_direct((char*)temp_displ_char, pos_v,pos);
  pos_v++;

  sprintf((char*)temp_displ_char,"%d", p_calset->cal_sel);
  displ_clr_row(pos_v);
	if (p_calset->cal_default)
  	pos = displ_str_direct((char*)"Base_sel= dflt ", pos_v,pos_h);
	else
  	pos = displ_str_direct((char*)"Base_sel= calc ", pos_v,pos_h);
  pos = displ_str_direct((char*)temp_displ_char, pos_v,pos);
  #endif

}
#endif

/****/
bool lib_cal_xtal_read_flash(T_CalSet* p_calset)
{
		// check file system exists
  if (!SPIFFS.begin()) {
		#ifdef DEBUG_CAL_XTAL		
    Serial.println("Formatting file system");
    #endif
		SPIFFS.format();
    SPIFFS.begin();
  }
  if (SPIFFS.exists(CAL_ITR_FILE)) {
    File f = SPIFFS.open(CAL_ITR_FILE, "r");
		if (f) {
			#ifdef DEBUG_CAL_XTAL		      
			Serial.print("file esiste\r");
			#endif
			uint16_t f_result = f.readBytes((char *)p_calset, sizeof(T_CalSet));    		
			if (f_result == sizeof(T_CalSet)){
			//if (f.readBytes((char *)p_calset, sizeof(T_CalSet)) == sizeof(T_CalSet)){
				f.close();
				#ifdef DEBUG_CAL_XTAL		
				Serial.printf("cal.base    = %0d-%0d\r", (uint32_t)(p_calset->cal_base >> 32), (uint32_t)p_calset->cal_base);
				Serial.printf("cal.base_1  = %0d-%0d\r", (uint32_t)(p_calset->cal_base_1 >> 32), (uint32_t)p_calset->cal_base_1);
				Serial.printf("cal.sel     = %0d\r", p_calset->cal_sel);
				Serial.printf("cal.default = %d\r\n", p_calset->cal_default);
				#endif
				return true;
			}
			#ifdef DEBUG_CAL_XTAL		
			Serial.printf("read file error  f_result= %d\r", f_result);
  		Serial.printf("sizeof_calset = %0d\r", sizeof(T_CalSet));
			Serial.printf("cal.base      = %0d-%0d\r", (uint32_t)(p_calset->cal_base >> 32), (uint32_t)p_calset->cal_base);
  		Serial.printf("cal.base_1    = %0d-%0d\r", (uint32_t)(p_calset->cal_base_1 >> 32), (uint32_t)p_calset->cal_base_1);
  		Serial.printf("cal.sel       = %0d\r", p_calset->cal_sel);
  		Serial.printf("cal.default   = %d\r\n", p_calset->cal_default);
			#endif
		}
  }
	#ifdef DEBUG_CAL_XTAL		
	Serial.print("file not fond\r");
	#endif
	return false;
}

/****/
bool lib_cal_write_data_flash(T_CalSet* p_calset)
{
	// check file system exists
  if (!SPIFFS.begin()) {
		#ifdef DEBUG_CAL_XTAL		
    Serial.println("Formatting file system");
    #endif
		SPIFFS.format();
    SPIFFS.begin();
  }
	if (SPIFFS.exists(CAL_ITR_FILE)){
    #ifdef DEBUG_CAL_XTAL		
		Serial.println("Remove File");
		#endif
		SPIFFS.remove(CAL_ITR_FILE);//remove????
	}

	File f = SPIFFS.open(CAL_ITR_FILE, "w", false);
	if (f) {
		uint16_t f_result = f.write((const unsigned char *)&calset, sizeof(calset)); 
		if (f_result == sizeof(T_CalSet)){
			f.close();
			#ifdef DEBUG_CAL_XTAL		
		  Serial.printf("write file ok / f_resutl = %d\r",f_result);
			#endif
			return true;
		}
		#ifdef DEBUG_CAL_XTAL		
		Serial.printf("read file error / f_resutl = %d\r",f_result);
		#endif
		return false;
	}
	#ifdef DEBUG_CAL_XTAL		
	Serial.print("file not created \r");
	#endif
	return false;
}
