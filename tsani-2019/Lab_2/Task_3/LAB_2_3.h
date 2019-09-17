/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2008. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: panelCB */
#define  PANEL_NUMERIC                    2
#define  PANEL_NUMERIC_2                  3
#define  PANEL_NUMERIC_3                  4
#define  PANEL_COMMANDBUTTON              5       /* callback function: BUTTON_WRITE */
#define  PANEL_NUMERIC_4                  6
#define  PANEL_NUMERIC_5                  7
#define  PANEL_NUMERIC_6                  8
#define  PANEL_COMMANDBUTTON_2            9       /* callback function: BUTTON_READ */
#define  PANEL_NUMERICSLIDE_OUT           10      /* callback function: VOUT_SLIDER */
#define  PANEL_NUMERICMETER_VIN           11
#define  PANEL_MEAS_COMMANDBUTTON_3       12      /* callback function: BUTTON_MEASURE */
#define  PANEL_COMMANDBUTTON_3            13      /* callback function: SetI_func */
#define  PANEL_COMMANDBUTTON_4            14      /* callback function: IACK_func */
#define  PANEL_NUMERICSLIDE_IACK          15      /* callback function: IACK_SLIDER */
#define  PANEL_NUMERICMETER_MEAS          16
#define  PANEL_COMMANDBUTTON_5_IACKM      17      /* callback function: Meas_IACK */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BUTTON_MEASURE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BUTTON_READ(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK BUTTON_WRITE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK IACK_func(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK IACK_SLIDER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Meas_IACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetI_func(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK VOUT_SLIDER(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
