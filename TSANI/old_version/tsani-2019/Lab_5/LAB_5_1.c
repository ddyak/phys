#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "LAB_5_1.h"
#include "toolbox.h"

#define NUM_SAMPLES 1300000
#define FREQUENCY 60000000
#define K = 1
#define R = 50

static int panelHandle;

int peaksAmount = (int)NUM_SAMPLES / 16.0;
double U1[NUM_SAMPLES];
double U2[NUM_SAMPLES];
double U3[NUM_SAMPLES];
double U4[NUM_SAMPLES];
double Xr[NUM_SAMPLES];
double Zr[NUM_SAMPLES];
double* coord1;
double* coord2;
double* coord3;
double* coord4;
double* XFFT_re;
double* XFFT_im;
double * module;
double* wform;
double coeff = 0.8;

void initializePorts() {
	
	double f;
	unsigned char delay = 229;
	
	portMask(0, 0xff);
	portMask(1, 0xff);
	portMask(2, 0x07);
			
	portOut(2, 0x00); 
	//portOut(1, 0x00);
	//portOut(0, 0x00);

	
	portOut(0, ~delay);
	portOut(1, 0x00);
	
	
	scopeVertical("0, 1, 2, 3, 7", 5.0, SCOPE_50_OHM);
	scopeFrequency("PFI1", 10000000, NUM_SAMPLES);
	scopeGetFrequency(&f);
	scopeTrigger("7", 0.2, SCOPE_POSITIVE); 
	
}


void readData(){
	portOut(1, 0x01);
	scopeStartRead("0, 1, 2, 3, 6", wform, NUM_SAMPLES);
	
	for(int i = 0; i < NUM_SAMPLES; i++){
		U1[i] = wform[i];
		U2[i] = wform[i + NUM_SAMPLES];
		U3[i] = wform[i + 2 * NUM_SAMPLES];
		U4[i] = wform[i + 3 * NUM_SAMPLES];
	}
	
	
	for(int i = 0; i < peaksAmount; i++){
		coord1[i] = U1[2 + i * 16];
		coord2[i] = U2[2 + i * 16];
		coord3[i] = U3[2 + i * 16];
		coord4[i] = U4[2 + i * 16];
		Xr[i] = coeff * 50.0 * (coord1[i] + coord2[i] - coord3[i] - coord4[i]) / (coord1[i] + coord2[i] + coord3[i] + coord4[i]);
		Zr[i] = coeff * 50.0 * (coord1[i] + coord4[i] - coord2[i] - coord3[i]) / (coord1[i] + coord2[i] + coord3[i] + coord4[i]);
		XFFT_re[i] = Xr[i];
		XFFT_im[i] = 0;
		
	}
	
	FFT(XFFT_re, XFFT_im, peaksAmount);
	
	for(int i = 0; i < peaksAmount; i++)
		module[i] = sqrt(XFFT_re[i]*XFFT_re[i] + XFFT_im[i]*XFFT_im[i]);
	module[0] = 0;
	
	
	PlotY(panelHandle, PANEL_GRAPH, Xr, peaksAmount, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);
	PlotY(panelHandle, PANEL_GRAPH_2, Zr, peaksAmount, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);
	//PlotY(panelHandle, PANEL_GRAPH_3, coord3, peaksAmount, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);
	PlotY(panelHandle, PANEL_GRAPH_4, coord4, peaksAmount, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);
	PlotY(panelHandle, PANEL_GRAPH_5, U1, NUM_SAMPLES, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);
	/*PlotY(panelHandle, PANEL_GRAPH, U1, NUM_SAMPLES, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);
	PlotY(panelHandle, PANEL_GRAPH_2, U1, NUM_SAMPLES, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN); 
	PlotY(panelHandle, PANEL_GRAPH_3, U3, NUM_SAMPLES, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN); 
	PlotY(panelHandle, PANEL_GRAPH_4, U4, NUM_SAMPLES, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);*/
	
	PlotY(panelHandle, PANEL_GRAPH_3, module, peaksAmount, VAL_DOUBLE, VAL_THIN_LINE, VAL_SOLID_SQUARE, VAL_SOLID, 100, VAL_GREEN);
	portOut(1, 0);
}

int main (int argc, char *argv[])
{
    int error = 0;
    scopeSlot(5);
	
	wform = (double*)malloc(sizeof(ViReal64) * NUM_SAMPLES * 5);
	coord1 = (double*)malloc(sizeof(double) * peaksAmount);
	coord2 = (double*)malloc(sizeof(double) * peaksAmount);
	coord3 = (double*)malloc(sizeof(double) * peaksAmount);
	coord4 = (double*)malloc(sizeof(double) * peaksAmount);
	XFFT_re = (double*)malloc(sizeof(double) * peaksAmount);
	XFFT_im = (double*)malloc(sizeof(double) * peaksAmount);
	module =  (double*)malloc(sizeof(double) * peaksAmount);
	
	
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "LAB_5_1.uir", PANEL));


	initializePorts();
	readData();
	
	
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	free(wform);
	free(coord1);
	free(coord2);
	free(coord3);
	free(coord4);
	free(XFFT_re);
	free(XFFT_im);
	free(module);
	scopeClose();
Error:
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

int CVICALLBACK restart (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			DeleteGraphPlot(panelHandle, PANEL_GRAPH, -1, VAL_DELAYED_DRAW);
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_3, -1, VAL_DELAYED_DRAW);
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_4, -1, VAL_DELAYED_DRAW);
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_5, -1, VAL_DELAYED_DRAW);
			GetCtrlVal(panelHandle, PANEL_NUMERIC, &coeff);
			readData();
			break;
	}
	return 0;
}









/*
	int k = 0;
	double maxElem;
	
	for(int i = 0; i < peaksAmount; i++){
		coord1[i] = U1[k];
		coord2[i] = U2[k];
		coord3[i] = U3[k];
		coord4[i] = U4[k];
		for(; k < 16 * (i + 1); k++){
			if(U1[k] > coord1[i])	coord1[i] = U1[k];	
			if(U2[k] > coord2[i])	coord2[i] = U2[k];
			if(U3[k] > coord3[i])	coord3[i] = U3[k];
			if(U4[k] > coord4[i])	coord4[i] = U4[k];
		}
		Xr[i] = 1.0 * 50.0 * (coord1[i] + coord2[i] - coord3[i] - coord4[i]) / (coord1[i] + coord2[i] + coord3[i] + coord4[i]);
		Zr[i] = 1.0 * 50.0 * (coord1[i] + coord4[i] - coord2[i] - coord3[i]) / (coord1[i] + coord2[i] + coord3[i] + coord4[i]);
	}*/
