//==============================================================================
//
// Title:       User Interface Application(TSANI)
// Purpose:     A short description of the application.
//
// Created on:  04.04.2004 at 4:10:03 by Gennady Kuzin.
// Copyright:   NSU. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "User Interface Application(TSANI).h"
#include "toolbox.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

static int panelHandle;
unsigned char var = 0;
int flag = 0;



void indicator(){
	portIn(0, &var);
	SetCtrlVal(panelHandle, PANEL_LED, var&1);
	SetCtrlVal(panelHandle, PANEL_LED_2, var&2);
	SetCtrlVal(panelHandle, PANEL_LED_3, var&4);
	SetCtrlVal(panelHandle, PANEL_LED_4, var&8);
	SetCtrlVal(panelHandle, PANEL_LED_5, var&16);
	SetCtrlVal(panelHandle, PANEL_LED_6, var&32);
	SetCtrlVal(panelHandle, PANEL_LED_7, var&64);
	SetCtrlVal(panelHandle, PANEL_LED_8, var&128);
}
//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
	ni6251Slot(2);
	
	portMask(0, 0xff);
	portOut(0, 0x00);
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "User Interface Application(TSANI).uir", PANEL));
    
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	

Error:
    /* clean up */
	ni6251Close();
    DiscardPanel (panelHandle);
    return 0;
}

//==============================================================================
// UI callback function prototypes

/// HIFN Exit when the user dismisses the panel.
int CVICALLBACK panelCB (int panel, int event, void *callbackData,
        int eventData1, int eventData2)
{
    if (event == EVENT_CLOSE)
        QuitUserInterface (0);
    return 0;
}

int CVICALLBACK TIMER_FUNC (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			if(flag == 1){
				portIn(0, &var);
				var=(var<<1)|(var>>7);
				portOut(0, var);
				indicator();
			}
			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK BUTTON (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			portIn(0, &var);
			switch (control) 
			{
				case PANEL_COMMANDBUTTON_0:
					portOut(0, var^1<<0);
					break;
				case PANEL_COMMANDBUTTON_1:
					portOut(0, var^1<<1);
					break;
				case PANEL_COMMANDBUTTON_2:
					portOut(0, var^1<<2);
					break;
				case PANEL_COMMANDBUTTON_3:
					portOut(0, var^1<<3);
					break;
				case PANEL_COMMANDBUTTON_4:
					portOut(0, var^1<<4);
					break;
				case PANEL_COMMANDBUTTON_5:
					portOut(0, var^1<<5);
					break;
				case PANEL_COMMANDBUTTON_6:
					portOut(0, var^1<<6);
					break;
				case PANEL_COMMANDBUTTON_7:
					portOut(0, var^1<<7);
					break; 
			}

			indicator();

			break;
		case EVENT_LEFT_CLICK:

			break;
		case EVENT_RIGHT_CLICK:

			break;
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK LIGHT_RUN (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			flag = !flag;
			break;
	}
	return 0;
}
