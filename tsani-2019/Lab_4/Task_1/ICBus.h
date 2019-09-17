#ifndef __ICBus_H__
#define __ICBus_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "cvidef.h"


void initPorts(void); //
int sendStart(void);  //
int sendStop (void);  
int sendBit (int bit);//
int sendByte (int byte);//
int readBit(int* bit);
int readByte(int* byte);
int currentSDA(void);
int getACK(void); //
void sendACK(void);
void NACK(void);
void writeBus(int sda, int scl); //
int readBus(int* sda, int* scl);   //


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __ICBus_H__ */

