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
#define  PANEL_GRAPH                      2
#define  PANEL_WAVEFORM                   3       /* callback function: waveformCB */
#define  PANEL_STRING_4                   4
#define  PANEL_STRING_3                   5
#define  PANEL_STRING_2                   6
#define  PANEL_STRING                     7
#define  PANEL_AMPLITUDE                  8       /* callback function: amplitudeCB */
#define  PANEL_FREQUENCY                  9       /* callback function: frequencyCB */
#define  PANEL_POINTSNUMBER               10      /* callback function: pointnumberCB */
#define  PANEL_FREQLISTBUTTON             11      /* callback function: freqListCB */
#define  PANEL_COMMANDBUTTON              12      /* callback function: triggerCB */
#define  PANEL_GRAPHFFT                   13
#define  PANEL_STEPSNUMBER                14
#define  PANEL_STEPDURATION               15
#define  PANEL_MINFREQ                    16
#define  PANEL_MAXFREQ                    17
#define  PANEL_TIMER                      18      /* callback function: TIMERCB */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK amplitudeCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK freqListCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK frequencyCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK pointnumberCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TIMERCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK triggerCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK waveformCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
