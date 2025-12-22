/*
sound.cpp
uso timer 1 di 4
*/

//#ifdef PIN_NOTA_OUT /////////////////////////////

#include <Arduino.h>
#include <my_define.h>
#include <sound.h>

#if SOUND_BUZZER_DAC == 1
    #include <driver/dac.h>
    #if (OUT_DAC_PIN == 26 )
    #define DAC_CHANNEL DAC_CHANNEL_2
    #else
    #define DAC_CHANNEL DAC_CHANNEL_2
    #endif
    #define DAC_OUT_DEFAULT_LEVEL_0         0
    #define DAC_OUT_DEFAULT_LEVEL_1         1
    static uint8_t   dac_out_value = DAC_OUT_DEFAULT_LEVEL_0;
    static uint8_t   dac_out_level = DAC_OUT_DEFAULT_LEVEL_1;//volume emissione nota
#endif


static portMUX_TYPE     timerMux_sound = portMUX_INITIALIZER_UNLOCKED;
static hw_timer_t*      sound_timer = NULL;
static bool             nota_key_enable = TRUE;
volatile bool           out_sound_pin = true;
volatile const long*    p_nota;
volatile int            sound_n_cicli = 0;
volatile uint32_t       NotaPeriod;


/* nuova versione, la durata e il cambio frequenza della nota,
vengono gestiti nel itr e non più da un task nel main
Dovrei avere durate della nota più precide
*/

static T_Lclk time_Lclk;
/****/
static void start_itr(void)
{
    NotaPeriod = *p_nota++;
    sound_n_cicli = *p_nota++;
    #if SOUND_BUZZER_DAC == 1
    dac_output_enable(DAC_CHANNEL);//abilita dac su out pin
    #endif 
    timerStop(sound_timer);
    timerAlarmWrite(sound_timer, NotaPeriod, true);//load nuov frequenza
    timerStart(sound_timer);
}

/****/
static void stop_itr(void)
{
    timerStop(sound_timer);//start timer

    #if SOUND_BUZZER_DAC == 1
    dac_out_value = DAC_OUT_DEFAULT_LEVEL_0;
    dac_output_voltage(DAC_CHANNEL, dac_out_value);
    dac_output_disable(DAC_CHANNEL);//disable pin out dac per evitare fruscio
    #else
    digitalWrite(OUT_BUZZER_PIN, 0);
    #endif 
    sound_n_cicli = 0;
    p_nota = 0;
    out_sound_pin = 0;
}

/****/
static void IRAM_ATTR sound_onTimer() {
  portENTER_CRITICAL_ISR(&timerMux_sound);
  if (sound_n_cicli){
    //nota non finita, reload timer e toggle out
    sound_n_cicli--;
    out_sound_pin = !out_sound_pin;
    #if SOUND_BUZZER_DAC == 1
    if (out_sound_pin)
        dac_out_value = dac_out_level;
    else
        dac_out_value = DAC_OUT_DEFAULT_LEVEL_0;
    dac_output_voltage(DAC_CHANNEL, dac_out_value);
    #else
    digitalWrite(OUT_BUZZER_PIN, out_sound_pin);
    #endif 
  } 
  else {
    //nota finita, verifica se in sequenza trovo un altra nota
    if (*p_nota){
        //non finita sequenza, riarmo il tutto
        out_sound_pin = !out_sound_pin;
        #if SOUND_BUZZER_DAC == 1
        if (out_sound_pin)
            dac_out_value = dac_out_level;
        else
            dac_out_value = DAC_OUT_DEFAULT_LEVEL_0;
        dac_output_voltage(DAC_CHANNEL, dac_out_value);
        #else
        digitalWrite(OUT_BUZZER_PIN, out_sound_pin);
        #endif
        NotaPeriod = *p_nota++;//load nuova nota
        sound_n_cicli = *p_nota++;//load durata nota
        timerAlarmWrite(sound_timer, NotaPeriod, true);//riarma periodo, qui possiamo modificare in funzione della calibrazione
    }
    else {
        stop_itr();  //finita sequenza, stop itr e set pin out
    }
  }
  portEXIT_CRITICAL_ISR(&timerMux_sound);
}


/**** pubbliche ****/
/****
 * inizializzo port buzzer, timer e procedura itr per generre suono
 * ****/
void sound_nota_init(void)
{
    #if SOUND_BUZZER_DAC == 1
    pinMode(OUT_DAC_PIN, OUTPUT);//metti in out pin dac
    digitalWrite(OUT_DAC_PIN, 0);//metti a massa input ampli x minor rumore
    dac_out_value = DAC_OUT_DEFAULT_LEVEL_0;
    #else
    pinMode(OUT_BUZZER_PIN, OUTPUT);    // sets the digital pin as output
    #endif 
    sound_timer = timerBegin(TIMER_SOUND, PRESCALER_TIMER_SOUND, true);//timer 1 con div 80
    timerStop(sound_timer);//stop timer
    timerAttachInterrupt(sound_timer, &sound_onTimer, true);
    timerAlarmEnable(sound_timer);//abilita itr timer
    timerSetAutoReload(sound_timer, true);//no serve ricaricare timer

}

/****
 * inizio emissione memlodia
 * faccio partire il task emissione nota
 * ****/
void sound_nota_start(const long *nota){
    if (nota){//test se esiste ptr ad una nota
        p_nota = nota;//ptr melodia in ptr globale
        if (*p_nota){//test se nota da emettere
            start_itr();  //nota da emettere, start itr
        }
    }
}

/*
emette nota su pressione tasto, se abilitato il flag
*/
void sound_nota_key(const long *nota)
{   
    if (nota_key_enable)
        sound_nota_start(nota);  
}

void sound_nota_key_enable(void)
{   nota_key_enable = NOTA_KEY_ENABLE; }
void sound_nota_key_disable(void)
{   nota_key_enable = NOTA_KEY_DISABLE;    }
bool sound_nota_key_get_state(void)
{   return nota_key_enable; }

/*
imposta il valore dello stato alto del pin DAC
*/
void soud_set_level_nota(uint8_t value)
{
    dac_out_level = value;
}

/*
legge valore stato alto del pin DAC*/
uint8_t sound_get_level_nota(void)
{   return dac_out_level;   }

//#endif ///////////////////////////////
