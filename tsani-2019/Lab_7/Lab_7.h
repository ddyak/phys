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
#define  PANEL_TOGGLEBUTTON               2       /* callback function: Init */
#define  PANEL_RADIOBUTTON_TRIANGLE       3       /* callback function: RADIOBUTTON_function */
#define  PANEL_RADIOBUTTON_SQUARE         4       /* callback function: RADIOBUTTON_function */
#define  PANEL_RADIOBUTTON_SIN            5       /* callback function: RADIOBUTTON_function */
#define  PANEL_TOGGLEBUTTON_3             6       /* callback function: Modulation */
#define  PANEL_TOGGLEBUTTON_4             7       /* callback function: Graph */
#define  PANEL_TOGGLEBUTTON_2             8       /* callback function: Generator */
#define  PANEL_NUMERIC_3                  9       /* callback function: OffSet */
#define  PANEL_NUMERIC_2                  10      /* callback function: Amplitude */
#define  PANEL_NUMERIC_4                  11      /* callback function: Frequensy_MOD */
#define  PANEL_NUMERIC_5                  12      /* callback function: Deviation */
#define  PANEL_NUMERIC                    13      /* callback function: Frequensy */
#define  PANEL_GRAPH                      14
#define  PANEL_TIMER                      15      /* callback function: Time1 */
#define  PANEL_GRAPH_2                    16
#define  PANEL_GRAPH_4                    17
#define  PANEL_GRAPH_5                    18
#define  PANEL_GRAPH_3                    19
#define  PANEL_RADIOBUTTON                20      /* callback function: Low_and_high */
#define  PANEL_RADIOBUTTON_2              21      /* callback function: Low_and_high */
#define  PANEL_NUMERIC_6                  22
#define  PANEL_NUMERIC_7                  23
#define  PANEL_RADIOBUTTON_5              24      /* callback function: Window */
#define  PANEL_RADIOBUTTON_4              25      /* callback function: Window */
#define  PANEL_RADIOBUTTON_3              26      /* callback function: Window */
#define  PANEL_TEXTMSG                    27
#define  PANEL_TOGGLEBUTTON_5             28      /* callback function: Speak */
#define  PANEL_TEXTMSG_2                  29
#define  PANEL_TIMER_2                    30      /* callback function: Time2 */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Amplitude(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Deviation(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Frequensy(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Frequensy_MOD(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Generator(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Graph(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Init(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Low_and_high(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Modulation(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OffSet(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK panelCB(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RADIOBUTTON_function(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Speak(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Time1(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Time2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Window(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
