/*
lib_clock_view.h
*/
#ifndef LIB_CLOCK_VIEW_H
#define LIB_CLOCK_VIEW_H

#define SET_NONE  0
#define SET_ORE   1
#define SET_MIN   2
#define SET_SEC   3
#define SET_MIL   4

/* procedure */
#if USE_LVGL_TFT_ESPI == 1
#else
extern int16_t  view_clkset_pixel(Tclkset* p_clkset, int row_px, int col, char resol, bool no_zeri, char chi_blank);
extern int16_t  view_Lclk_pixel(T_Lclk* p_Lclk, int row_px, int col, char resol, bool no_zeri, char chi_blank);
extern int16_t  view_clkset(Tclkset* p_clkset, int row, int col, char resol, bool no_zeri, char chi_blank);
extern int16_t  view_Lclk(T_Lclk* p_Lclk, int row, int col, char resol, bool no_zeri, char chi_blank);
#endif
extern void     view_clock_clkset_to_buff(Tclkset *p_clkset, char* buff, char resol, bool no_zeri, char chi_blank);
extern void     view_clock_Lclk_to_buff  (T_Lclk * p_Lclk,   char* buff, char resol, bool no_zeri, char chi_blank);

#endif