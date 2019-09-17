/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2004. All Rights Reserved.          */
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
#define  PANEL_LED                        2
#define  PANEL_LED_2                      3
#define  PANEL_LED_3                      4
#define  PANEL_LED_4                      5
#define  PANEL_LED_5                      6
#define  PANEL_LED_6                      7
#define  PANEL_LED_7                      8
#define  PANEL_LED_8                      9
#define  PANEL_COMMANDBUTTON_7            10      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_6            11      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_5            12      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_4            13      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_3            14      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_2            15      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_1            16      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_0            17      /* callback function: BUTTON */
#define  PANEL_COMMANDBUTTON_9            18      /* callback function: LIGHT_RUN */
#define  PANEL_TIMER                      19      /* callback function: TIMER_FUNC */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BUTTON(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LIGHT_RUN(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMER_FUNC(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
