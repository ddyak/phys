#ifndef __dac_adc_H__
#define __dac_adc_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"

int dac_init(void);
int adc_init(void);
int dac_adc_out(int address, double value);
int dac_adc_out_code(int dac_num, int code);
int dac_adc_in(int address, double* value);
int dac_adc_in_code(int adc_num, int* value);
int dac_adc_in_IACK(int adc_num, double* value);
int dac_adc_SetI(void); 
int dac_adc_IACK(void);


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __dac_adc_H__ */
