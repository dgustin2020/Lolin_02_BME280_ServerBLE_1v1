/*
sound.h
*/
#ifndef SOUND_H
#define SOUND_H 

//#ifdef PIN_NOTA_OUT /////////////////////////////

/****/
#define PRESCALER_TIMER_SOUND   80 // per avere clock a 1 MHz

/* cost note da generare */
extern const long NOTA_SCANDI[];
extern const long NOTA_ERROR[];
extern const long NOTA_KEY_CLICK[];
extern const long NOTA_KEY_ERR[];
extern const long NOTA_KEY_OK[];

/* procedure */
extern void     sound_nota_init(void);
extern void     sound_nota_start(const long *nota);
extern void     sound_nota_key(const long *nota);
extern void     sound_nota_key_enable(void);
extern void     sound_nota_key_disable(void);
extern bool     sound_nota_key_get_state(void);
extern void     soud_set_level_nota(uint8_t value);
extern uint8_t  sound_get_level_nota(void);
#define SOUND_NOTA_LEVEL_MIN  1
#define SOUND_NOTA_LEVEL_MAX  128

#define NOTA_KEY_ENABLE     true
#define NOTA_KEY_DISABLE    false

//#endif ///////////////////////////////////

#endif