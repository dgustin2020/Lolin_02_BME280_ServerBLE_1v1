/*
calibration.h
*/
#ifndef CALIBRATION_H
#define CALIBRATION_H

#define PIN_IN_CALIBRATION 		PIN_IN_LINEA_A

#define CAL_SECONDS_X_MISURE    10+2
#define CAL_ITR_FILE            "/CalItr1000"

typedef struct{
    uint64_t cal_base;
    uint64_t cal_base_1;
    uint16_t cal_sel;
    uint16_t cal_default;
}T_CalSet;
extern T_CalSet calset;

extern void lib_cal_xtal_init(uint16_t pin);
extern void lib_cal_xtal_start(void);
extern bool lib_cal_xtal_run(T_CalSet* p_calset);
extern bool lib_cal_get_flag_read_input(void);
extern void lib_cal_clr_flag_read_input(void);
extern uint16_t lib_cal_get_cnt_seconds(void);
extern void lib_cal_write_data(T_CalSet* p_calset, uint16_t pos_v, uint16_t pos_h);
extern bool lib_cal_xtal_read_flash(T_CalSet* p_calset);
extern bool lib_cal_write_data_flash(T_CalSet* p_calset);

#endif
