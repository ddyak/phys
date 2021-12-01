#ifndef __avalon_H__
#define __avalon_H__

#ifdef __cplusplus
    extern "C" {
#endif



#include "cvidef.h"
 

void initial(void);
void write(unsigned int adr, unsigned int word);
void read(unsigned int adr, unsigned int *word);
void divide(unsigned int adr, unsigned char *adr0, unsigned char *adr1) ;
void add(unsigned char a, unsigned int suba, unsigned int *adr);
void kod(unsigned char k);
void voltage(double v);
void acp(double *word2);
void setll(void);
void iack(void);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __Avalon_H__ */
