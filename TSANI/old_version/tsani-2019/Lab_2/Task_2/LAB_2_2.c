#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "LAB_2_2.h"
#include "toolbox.h"


static int panelHandle;
double u_val = 0.0;
double v_val = 0.0;

int main (int argc, char *argv[])
{
    int error = 0;
	ni6251Slot(2);
    
 
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "LAB_2_2.uir", PANEL));
    

    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
	ni6251Close();
    DiscardPanel (panelHandle);
    return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK MEAS_BUT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_LEFT_CLICK:
			GetCtrlVal(panelHandle, PANEL_NUMERICSLIDE, &u_val);
			analogOut(0, u_val);
			analogIn(0, &v_val);
			SetCtrlVal(panelHandle, PANEL_NUMERICMETER, v_val);
	}
	return 0;
}
