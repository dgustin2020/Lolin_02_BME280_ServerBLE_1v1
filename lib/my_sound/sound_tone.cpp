/*
sounf_tone.cpp
*/
//#include <my_include.h>
#include <Arduino.h>
#include <my_define.h>
#include "sound.h"

/* la durata della nota viene convertita in n_cicli da emettere in funzione della frequenza
   in questo modo non uso nessun clock esterno tipo Base_Lclk

   K_NOTA(frequenza in Hz), K_N_CICLI(frequenza in HZ, durata in mSec) con NULL finale per terminare

*/

#define CLOCK_SOUND         (CLOCK_CPU / PRESCALER_TIMER_SOUND) // 80MHz /080 = 1000000l
#define K_NOTA(NOTA)        (CLOCK_SOUND / (2*(NOTA)))
#define DELAY_STOP_TIMER    2 //mSec
#define K_N_CICLI(NOTA, DURATA) ( (DURATA * 1000) / (CLOCK_SOUND / (2*(NOTA))))

/****/
const long NOTA_SCANDI[] = {
    K_NOTA(4000), K_N_CICLI(4000, 40),
    0
};
/****/
const long NOTA_ERROR[] = {
    K_NOTA(3000), K_N_CICLI(3000, 200),
    K_NOTA(2000), K_N_CICLI(2000, 200),
    K_NOTA(3000), K_N_CICLI(3000, 200),
    K_NOTA(2000), K_N_CICLI(2000, 200),
    K_NOTA(3000), K_N_CICLI(3000, 200),
    K_NOTA(2000), K_N_CICLI(2000, 200),
    K_NOTA(3000), K_N_CICLI(3000, 200),
    K_NOTA(2000), K_N_CICLI(2000, 200),
    0
};
const long NOTA_KEY_CLICK[] = {
    K_NOTA(100), K_N_CICLI(100, 10),
    0
};

const long NOTA_KEY_ERR[] = {
    K_NOTA(2000), K_N_CICLI(2000,300),
    K_NOTA(800), K_N_CICLI(800,300),
    0
};
const long NOTA_KEY_OK[] = {
    K_NOTA(1000), K_N_CICLI(1000, 150),
    0
};
