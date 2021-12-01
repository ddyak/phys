#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "LAB_4_1.h"
#include "toolbox.h"
#include <ICBus.h>

static int panelHandle;

char name[7] = {0};
int date = 0x01;
int flag = 0;
double temper[1000];
int idx = 0;
double X[1000];


void writeI2C(int address, int sub_address, int word)   
{
	sendStart(); 
	sendByte(address<<1); 
	getACK();    
	sendByte(sub_address); 
	getACK();
	sendByte(word); 		                        
	getACK();									   
	sendStop();
}


void readI2C(int address, int sub_address, int* word)      
{
	sendStart();
	sendByte((address<<1) | 0x00);
	getACK();
	sendByte(sub_address);
	getACK();
	sendStop();
	sendStart();
	sendByte((address<<1) | 0x01);
	getACK();
	readByte(word);
	NACK();
	sendStop();
}


void read_name(){
	int letter = 0;
	for(int i = 0; i < 7; i++){
		readI2C(0, i + 8, &letter);
		name[i] = (char)letter;
	}
}


double getTemp()
{
	int date1, date2;
	
	sendStart();
	sendByte((0x28 << 1) | 0x01);
	getACK();
	readByte(&date1);
	sendACK();
	readByte(&date2);
	sendACK();
	sendStop();

	short temp = (((date1)<<8) | date2) & 0xFFC0;
	
	return temp / 256.;
}



int main (int argc, char *argv[])
{
    int error = 0;
    
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "LAB_4_1.uir", PANEL));
	initPorts();
	SetAxisScalingMode(panelHandle, PANEL_STRIPCHART, VAL_LEFT_YAXIS, VAL_AUTOSCALE, -10.00, 35.00);
	SetAxisScalingMode(panelHandle, PANEL_STRIPCHART, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, 10.00);
	
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    DiscardPanel (panelHandle);
    return 0;
}


int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface(0);
    return 0;
}


int CVICALLBACK timer_light (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			if(flag){
				date=((date & 0x80) >> 7) | ((date << 1));
				writeI2C(0, 0x00, date);
			}
			break;
	}
	return 0;
}


int CVICALLBACK start_lights (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			flag = !flag;
			break;
	}
	return 0;
}

int CVICALLBACK getname (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			read_name();
			SetCtrlVal(panelHandle,PANEL_STRING, name);
			break;

	}
	return 0;
}

int CVICALLBACK getTemper (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			SetCtrlVal(panelHandle, PANEL_NUMERIC, getTemp());
			break;
	}
	return 0;
}


int CVICALLBACK timerGraph (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			PlotStripChartPoint(panelHandle, PANEL_STRIPCHART,  getTemp()); 
			break;
	}
	return 0;
}
