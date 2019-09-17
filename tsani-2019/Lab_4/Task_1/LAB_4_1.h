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
#define  PANEL_CHECKBOX                   2       /* callback function: start_lights */
#define  PANEL_STRING                     3
#define  PANEL_COMMANDBUTTON              4       /* callback function: getname */
#define  PANEL_NUMERIC                    5
#define  PANEL_COMMANDBUTTON_2            6       /* callback function: getTemper */
#define  PANEL_TIMER                      7       /* callback function: timer_light */
#define  PANEL_TIMER_2                    8       /* callback function: timerGraph */
#define  PANEL_STRIPCHART                 9


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK getname(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK getTemper(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK start_lights(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timer_light(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timerGraph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
