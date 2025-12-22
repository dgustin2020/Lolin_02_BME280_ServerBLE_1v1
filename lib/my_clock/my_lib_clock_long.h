/*
lib_clock_long.h
*/
#ifndef LIB_CLOCK_LONG_H
#define LIB_CLOCK_LONG_H


typedef uint32_t    T_Lclk;
typedef uint32_t*   PT_Lclk;
typedef struct      Lclk
{
  T_Lclk    time;
  uint16_t  flag;
}TS_Lclk, *PTS_Lclk;

typedef struct clkset
{
  signed    int segno;
  signed    int ore;
  signed    int min;
  signed    int sec;
  signed    int mil;//millessimi completi
  signed    int enable;
}Tclkset, *PTclkset;
#define CLKSET_ENABLE_SI       1
#define CLKSET_ENABLE_NO       0
#define CLKSET_ENABLE_NOT_USE -1
/* procedure pubbliche */
extern uint8_t* Lclk_to_byte(PT_Lclk p_clk, uint8_t *p_dove);
extern char*    Lclk_to_char_no_inter(PT_Lclk p_clk, char *p_dove);

extern uint32_t Lclk_incre(PT_Lclk p_clk);
extern uint32_t Lclk_add(PT_Lclk p_add1, PT_Lclk p_add2, PT_Lclk p_res);
extern uint32_t Lclk_sub(PT_Lclk p_sub1, PT_Lclk p_sub2, PT_Lclk p_res);
extern uint32_t Lclk_compare(PT_Lclk p_clk_1, PT_Lclk p_clk_2);
extern uint32_t Lclk_add_value(PT_Lclk p_add1, T_Lclk value, PT_Lclk p_res);
extern uint32_t Lclk_sub_value(PT_Lclk p_add1, T_Lclk value, PT_Lclk p_res);

extern uint8_t  Lclk_to_ore(T_Lclk clk);
extern uint8_t  Lclk_to_min(T_Lclk clk);
extern uint8_t  Lclk_to_sec(T_Lclk clk);

extern T_Lclk   Lclk_clear_frazioni(T_Lclk clk);
extern T_Lclk   Lclk_clear_secondi(T_Lclk clk);

extern void     Lclk_Lclk_to_clkset(T_Lclk* clk, Tclkset* dest);
extern int      Lclk_clkset_to_Lclk(Tclkset* clk_set, T_Lclk* clk);

extern void     Lclk_time_to_sec_mil(T_Lclk val, uint32_t* sec, uint32_t* mil);

/* risoluzione al millesimo */
#define L_MIL           (1*L_MILL)      //          1
#define L_CEN           10            //         10
#define L_DEC           (10*L_CEN)      //        100
#define L_SEC           (10*L_DEC)      //      1 000
#define L_MIN           (60*L_SEC)      //     60 000
#define L_ORA           (60*L_MIN)      //  3 600 000
#define L_DAY           (24*L_ORA)      // 86 400 000
#define L_CLK_OVERFLOW  ( 7*L_DAY)      //604 800 000
#define L_NEGATIVO      0x80000000

#define L_CONV_DAY      0
#define L_CONV_ORE      1
#define L_CONV_MIN      2
#define L_CONV_SEC      3
#define L_CONV_CEN      4
#define L_CONV_MIL      5
#define L_CONV_LONG     6+4

#define L_MAGGIORE      2
#define L_MINORE        1
#define L_UGUALE        0
#define L_NO_TIME       (long)-1
#define L_MASK_TIME     0x7fffffff

/* risoluzione */
#define RESOL_SEC       0
#define RESOL_DEC       1
#define RESOL_CEN       2
#define RESOL_MIL       3

/* zeri prima */
#define L_NO_ZERI       true
#define L_SI_ZERI       false

/* varaibili pubbliche */
extern uint8_t Lclk_conv[];
extern Tclkset clock_set;

/* constanti */                               
extern T_Lclk const LCLK_1_SEC;
extern T_Lclk const LCLK_ON_INTERCIFRE;
#endif
