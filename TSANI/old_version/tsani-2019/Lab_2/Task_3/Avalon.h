#ifndef __Avalon_H__
#define __Avalon_H__

#ifdef __cplusplus
    extern "C" {
#endif



#include "cvidef.h"

int check_value_int(int* value, int min, int max);
int check_value_double(double* value, double min, double max);
int avalon_init(void);
int avalon_write(int address, int subaddress, int value);
int avalon_read(int address, int subaddress, int* value);
int Declare_Your_Functions_Here (int x);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Avalon_H__ */
