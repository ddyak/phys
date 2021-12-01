#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "LAB_2_3.h"
#include "toolbox.h"
#include "Avalon.h"
#include "dac_adc.h"

int address = 0;
int sub_address = 0;
int value = 0;


static int panelHandle;

int main (int argc, char *argv[])
{
    int error = 0;
	ni6251Slot(2);
	avalon_init();
	dac_init();
	adc_init();
    
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "LAB_2_3.uir", PANEL));
    
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface());

Error:
    DiscardPanel (panelHandle);
	ni6251Close();
    return 0;
}


int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK BUTTON_WRITE (int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		GetCtrlVal(panelHandle, PANEL_NUMERIC, &address);
		GetCtrlVal(panelHandle, PANEL_NUMERIC_2, &sub_address);
		GetCtrlVal(panelHandle, PANEL_NUMERIC_3, &value);
		avalon_write(address, sub_address, value);
		break;
	}
	return 0;
}

int CVICALLBACK BUTTON_READ (int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
		GetCtrlVal(panelHandle,PANEL_NUMERIC_4, &address);
		GetCtrlVal(panelHandle,PANEL_NUMERIC_5, &sub_address);
		avalon_read(address, sub_address, &value);
		SetCtrlVal(panelHandle, PANEL_NUMERIC_6, value);
		break;
	}
	return 0;
}


int CVICALLBACK VOUT_SLIDER (int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
	double val;
	switch (event) {
		
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERICSLIDE_OUT, &val);
			//dac_adc_out(2, val);
			analogOut(1, val);
			break;
	}
	return 0;
}


int CVICALLBACK BUTTON_MEASURE (int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
	double val;
	switch (event) {
		case EVENT_COMMIT:
			//analogIn(0, &val);
			dac_adc_in(2, &val);
			SetCtrlVal(panelHandle, PANEL_NUMERICMETER_VIN, val);
			break;
	}
	return 0;
}








int CVICALLBACK IACK_func (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			dac_adc_IACK();
			break;
	}
	return 0;
}

int CVICALLBACK SetI_func (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			dac_adc_SetI();
			break;
	}
	return 0;
}


int CVICALLBACK IACK_SLIDER (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	double val;
	switch (event) {
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle, PANEL_NUMERICSLIDE_IACK, &val);
			dac_adc_out(1, val);
			break;
	}
	return 0;
}

int CVICALLBACK Meas_IACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	double val;
	switch (event) {
		case EVENT_COMMIT:
			dac_adc_in_IACK(1, &val);
			SetCtrlVal(1, PANEL_NUMERICMETER_MEAS, val);
			
			break;
	}
	return 0;
}
