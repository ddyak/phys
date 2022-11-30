/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2022. All Rights Reserved.          */
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
#define  PANEL_NUMERICTHERM               2       /* callback function: cbtermometr */
#define  PANEL_TOGGLEBUTTON               3       /* callback function: BTNcb */
#define  PANEL_LED                        4
#define  PANEL_NUMERIC                    5
#define  PANEL_CONTROLBUTTON              6       /* callback function: controlcb */
#define  PANEL_STRIPCHART                 7
#define  PANEL_NUMERIC_2                  8
#define  PANEL_K1                         9
#define  PANEL_K2                         10
#define  PANEL_K3                         11
#define  PANEL_K1val                      12
#define  PANEL_K2val                      13
#define  PANEL_K3val                      14
#define  PANEL_STRIPCHART_2               15
#define  PANEL_STRIPCHART_4               16
#define  PANEL_STRIPCHART_5               17
#define  PANEL_STRIPCHART_3               18
#define  PANEL_CONTROLTIMER               19      /* callback function: timecontrol */
#define  PANEL_TIMER                      20      /* callback function: cbtimer */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BTNcb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbtermometr(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK cbtimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK controlcb(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK timecontrol(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
