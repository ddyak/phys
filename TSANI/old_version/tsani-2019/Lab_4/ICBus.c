#include "tsani.h"
#include <utility.h>
#include "ICBus.h"
#define DELAY 0.001

void initPorts(void) {
	portMask(0,0x1F);
	portMask(1,0x00);
	portOut(0,0x07);
}



int sendStart(void) {
	writeBus (1, 1);	              //все подняли
	Delay(DELAY);
	writeBus(0, 1);				  //пошёл старт
	Delay(DELAY);
	writeBus(0, 0);	              //вернули в нач положение	(делаем так всегда)															 
	Delay(DELAY);
	return 0;
}

int sendStop(void) {
	writeBus(0, 1);                //нач положение
	Delay(DELAY);
	writeBus(1, 1);		        //пошёл STOP
	Delay(DELAY);
	writeBus(0, 0);                //нач полож 
	Delay(DELAY);
	return 0;
}



int sendBit(int bit) {			   
	writeBus(currentSDA(), 0);		
	writeBus(bit, 0);			    //SDA меняем только тогда, когда тактовый сиглан низкий
	Delay(DELAY);
	writeBus(currentSDA(), 1);		//1 когда закончили запись (после каждого бита), это отправка бита
	Delay(DELAY);
	writeBus(currentSDA(), 0);   	//вернуть в нач положение
	Delay(DELAY);
	return 0;
}

int sendByte(int byte)	{				  
	for (int i = 0; i < 8; i++)
		sendBit ((byte >> (7 - i)) & 1);
	return 0;
}

int readBit(int* bit) {
	writeBus(1, 0);                  
	writeBus(1, 1);                  
	*bit = currentSDA(); 			
	return 0;
}

int readByte(int* byte) {
	int date;
	*byte = 0;
	for(int i = 0; i < 8; i++) {
	    readBit(&date);
		*byte = *byte | (date << (7 - i));		         //Date или 1 или 0; записывает сначала старший и т.д.
	}
	return 0;
}

int currentSDA(void) { 
	int sda, scl;
	readBus(&sda, &scl);
	return sda;
}

int getACK(void) {					   
	double time = 0.0;
	writeBus(currentSDA(), 0);        
	writeBus(1, 1);	               
	Delay(DELAY);
	do {
		Delay(DELAY);
	    time = time + DELAY;
		if(time > 0.005)
			return -1;
	} while(currentSDA() != 0);      
	return 0;
}

void sendACK(void) 
{			
	sendBit(0);
//	(currentSDA(), 1);	         
}



void NACK(void) {			
	writeBus(currentSDA(), 1);	         
}


void writeBus(int sda, int scl){   	         //вывод на шину sda & scl{
	unsigned char data;
	data = (sda << 3) | (scl << 4) | 0x07;	 //сдвиг чтобы было на своей линии
	portOut (0, data);		                 //0 т.к. такой порт (см табл)
}							        

int readBus(int* sda, int* scl){       		 //читает с шины sda & scl{
	unsigned char val0, val1;
	if ((!portIn(0, &val0)) && (!portIn(1, &val1))){
			*sda = (val0 >> 7);
			*scl = val1;
			return 0;		  
		}
	else
		return -1;	                        
}

