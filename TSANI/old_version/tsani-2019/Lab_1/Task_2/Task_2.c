#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include "Task_2.h"
#include "toolbox.h"
#include <stdio.h>
#include <math.h>

static int panelHandle;
double a, w, sin_arr[1000], sin_fft_real[1000], sin_fft_im[1000], module[1000];
double n_rate = 0.7;
FILE *file;

void create_signal(){
	for(int i = 0; i !=1000; i++ ){
		sin_arr[i] = a * sin(2 * Pi() * w * i / 1000) + Random(-n_rate, n_rate);
		sin_fft_real[i] = sin_arr[i];
		sin_fft_im[i] = 0;
	}
}

void draw_graph(){
	DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
	SetAxisScalingMode(panelHandle, PANEL_GRAPH, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, 1000);
	SetAxisScalingMode(panelHandle, PANEL_GRAPH, VAL_LEFT_YAXIS, VAL_MANUAL, -15, 15);
	PlotY(panelHandle, PANEL_GRAPH, sin_arr, 1000, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 10000, VAL_BLACK);
}

void calculate_module(){
	for(int i = 0; i < 1000; i++)
		module[i] = sqrt(sin_fft_real[i]*sin_fft_real[i] + sin_fft_im[i]*sin_fft_im[i]);
}

void draw_graph_FFT(){
	DeleteGraphPlot(panelHandle, PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
	SetAxisScalingMode(panelHandle, PANEL_GRAPH_2, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, 1000);
	SetAxisScalingMode(panelHandle, PANEL_GRAPH_2, VAL_LEFT_YAXIS, VAL_AUTOSCALE, 0, 0);
	PlotY(panelHandle, PANEL_GRAPH_2, module, 1000, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 10000, VAL_GREEN);
}

int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "Task_2.uir", PANEL));
	
    a = 7;
	w = 3;
	create_signal();
	draw_graph();
	
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    /* clean up */
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

int CVICALLBACK TIMER_CALL (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_TIMER_TICK:
			GetCtrlVal(panelHandle, PANEL_NUMERIC, &a);
			GetCtrlVal(panelHandle, PANEL_NUMERIC_2, &w);
			create_signal();
			FFT(sin_fft_real, sin_fft_im, 1000);
			calculate_module();
			draw_graph();
			draw_graph_FFT();

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK SAVE_ARR (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:

			break;
		case EVENT_LEFT_CLICK:
			file = fopen("Saved_Array.txt", "w");
			for(int i = 0; i < 1000; i++)
				fprintf(file, "%lf %d\n", sin_arr[i], i);
			fclose(file);

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
