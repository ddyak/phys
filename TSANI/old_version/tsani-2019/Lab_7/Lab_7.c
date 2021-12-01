//==============================================================================
//
// Title:       Lab_7
// Purpose:     A short description of the application.
//
// Created on:  01.01.2008 at 0:10:20 by Gennady Kuzin.
// Copyright:   NSU. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <analysis.h>
#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include <rs232.h>
#include "Lab_7.h"
#include "toolbox.h"

//==============================================================================
// Constants
#define PORTNUM  8
#define TERMCHAR  "\n"
#define BUFSIZE   64
#define LEVEL 1 
#define SAMPLING_RATE 1000000
#define N (SAMPLING_RATE/10)
//==============================================================================
// Types

//==============================================================================
// Static global variables
static char Buf[BUFSIZE] = {(0)};

static int panelHandle;

//==============================================================================
// Static functions

//==============================================================================
// Global variables
double Signal_X[N]; 
double Signal[N]; 
double Signal_re[N]; 
double Signal_im[N]; 
double Signal_FFT[N]; 
double Signal_filtered_FFT[N];
double Signal_filtered[N + 40001]; 
double kernel[40001];  
double kernel_FFT[40001];
double kernel_im[40001];  
//==============================================================================
// Global functions
int InitCOM( int portNumber)
{
	char PortName[5] = {(0)};
	sprintf(PortName, "COM%i", portNumber);
	if (OpenComConfig(portNumber, PortName, 19200,0,8,1,0,0)<0)
	{
		goto Error;
	}
	if (!SetComTime(portNumber, 0.4))
	{
		return GetComStat(portNumber);
	}
Error:
	MessagePopup("Serial Configuration Error", GetRS232ErrorString(ReturnRS232Err()));
	return -1;
	
}

int COM_generatot_off(int portnumber)
{
	SetBreakOnLibraryErrors(0);
	return 0;
}
int Com_deinit(int portnumber)
{
	if (GetComStat(portnumber)>=0)
	{ 
		CloseCom(portnumber);
	}
	else
	{
		MessagePopup("Serial Configuration Error", GetRS232ErrorString(ReturnRS232Err()));
		return -1;
	}
	return 0;
	
}

int SerWriteInt (const char comand[],int param)
{
	char buf[1024] = {(0)};
	sprintf(buf, "%s %d%s", comand, param, TERMCHAR);
	if (ComWrt(PORTNUM, buf, strlen(buf)) != strlen(buf))
		return(ReturnRS232Err());
	return 0;
}

int SerWriteFp (const char comand[],double param)
{
	char buf[1024] = {(0)};  
	sprintf(buf, "%s %lf%s", comand, param, TERMCHAR);
	if (ComWrt(PORTNUM, buf, strlen(buf)) != strlen(buf))
		return (ReturnRS232Err());
	return 0;
}
int SerReadInt( char *buf)
{
	if (ComRdTerm(PORTNUM, buf, BUFSIZE, (int)TERMCHAR)<=0)
		return ReturnRS232Err();
	return 0;
}

int SerReadFp( char *buf)
{
	if (ComRdTerm(PORTNUM, buf, BUFSIZE, (int)TERMCHAR)<=0)
		return ReturnRS232Err();
	return 0;
}

int CalcLPFKernel(int window, int length, double cutoff, double *kernel)
{
	int i, M = (length - 1);
	double kernel_sum = 0.0;
	switch(window)
	{
		case 1: //Rectangular
		for(i = 0; i < length; i++)
		{
			*(kernel + i) = (i != M/2) ? sin(2*PI*cutoff*(i-M/2))/((i-M/2)*PI)*1 : (2*cutoff);
			kernel_sum += *(kernel + i);
		}
		
		break;
		
		case 2: //Hamming
		for(i = 0; i < length; i++)
		{
			*(kernel + i) = (i != M/2) ? sin(2*PI*cutoff*(i-M/2))/((i-M/2)*PI)*(0.54 - 0.46*cos(2*PI*i/M)) : (2*cutoff);
			kernel_sum += *(kernel + i);
		}
		break;
		
		case 3: //Blackman
		for(i = 0; i < length; i++)
		{
			*(kernel + i) = (i != M/2) ? sin(2*PI*cutoff*(i-M/2))/((i-M/2)*PI)*(0.42 - 0.5*cos(2*PI*i/M) + 0.08*cos(4*PI*i/M)) : (2*cutoff);
			kernel_sum += *(kernel + i);
		}
		break;
	}
	for (i = 0; i < length; i++) *(kernel + i) /= kernel_sum;
	return 0;
}
void SpectralInversion(double* kernel, int length)
{
	int M = length-1;
	for (int i = 0; i < length; i++) 
		*(kernel + i) = (i != M/2) ? (-*(kernel + i)) : (1 - *(kernel + i));
}


/// HIFN The main entry-point function.
int main (int argc, char *argv[])
{
    int error = 0;
    ni6251Slot(2);
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "Lab_7.uir", PANEL));
	SetCtrlAttribute(panelHandle,PANEL_TIMER,ATTR_ENABLED,0);
    SetCtrlAttribute(panelHandle,PANEL_TIMER_2,ATTR_ENABLED,0);
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());

Error:
    /* clean up */
    DiscardPanel (panelHandle);
	ni6251Close();  
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

int CVICALLBACK Init (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		int state;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON,&state);
			if (state ==0)
				Com_deinit(PORTNUM);
			else
				InitCOM(PORTNUM);
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

int CVICALLBACK RADIOBUTTON_function (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			int generator_state;
			SetCtrlVal(PANEL, PANEL_RADIOBUTTON_SIN,0);
			SetCtrlVal(PANEL, PANEL_RADIOBUTTON_SQUARE,0); 
			SetCtrlVal(PANEL, PANEL_RADIOBUTTON_TRIANGLE,0); 
			SetCtrlVal(PANEL, control,1); 
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_2,&generator_state);
			if (generator_state == 1)
			{
				switch(control)
				{
					case PANEL_RADIOBUTTON_SIN:
						SerWriteInt(":FUNction:WAVeform",1);
						break;
					
					case PANEL_RADIOBUTTON_SQUARE:
						SerWriteInt(":FUNction:WAVeform",3);
						break;
					
					case PANEL_RADIOBUTTON_TRIANGLE:
						SerWriteInt(":FUNction:WAVeform",2);
						break;
				}
			}
						
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

int CVICALLBACK Generator (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		double value;
		int value_int;
		int generator_state, modulation_state;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_2,&generator_state);
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_3,&modulation_state); 
			if(generator_state==1)
			{
				GetCtrlVal(PANEL, PANEL_RADIOBUTTON_TRIANGLE,&value_int);
				if (value_int == 1) SerWriteInt(":FUNction:WAVeform",2);
				GetCtrlVal(PANEL, PANEL_RADIOBUTTON_SIN,&value_int); 
				if (value_int == 1) SerWriteInt(":FUNction:WAVeform",1); 
				GetCtrlVal(PANEL, PANEL_RADIOBUTTON_SQUARE,&value_int);
				if (value_int == 1) SerWriteInt(":FUNction:WAVeform",3); 
				
				GetCtrlVal(PANEL, PANEL_NUMERIC,&value);
				SerWriteFp(":FREQuency",value);
				GetCtrlVal(PANEL, PANEL_NUMERIC_2,&value);
				SerWriteFp(":AMPLitude:VOLTage",value); 
				GetCtrlVal(PANEL, PANEL_NUMERIC_3,&value);
				SerWriteFp(":OFFset",value);
				if (modulation_state == 1)
				{
					GetCtrlVal(PANEL,PANEL_NUMERIC_4, &value);
					SerWriteFp(":SOURce:MODFM:RATe",value);
					
					GetCtrlVal(PANEL,PANEL_NUMERIC_5, &value);
					SerWriteFp(":SOURce:MODFM:SPAN",value);
				}
			}
			else 
			{
				COM_generatot_off(PORTNUM);

			}
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

int CVICALLBACK Frequensy (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		int generator_state;
		double value;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_2,&generator_state);
			if (generator_state==1)
			{
				GetCtrlVal(PANEL, PANEL_NUMERIC,&value);
				SerWriteFp(":FREQuency",value);
			}
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

int CVICALLBACK Amplitude (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		int generator_state;
		double value;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_2,&generator_state);
			if (generator_state==1)
			{
				GetCtrlVal(PANEL, PANEL_NUMERIC_2,&value);
				SerWriteFp(":AMPLitude:VOLTage",value);
			}

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

int CVICALLBACK OffSet (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		int generator_state;
		double value;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_2,&generator_state);
			if (generator_state==1)
			{
				GetCtrlVal(PANEL, PANEL_NUMERIC_3,&value);
				SerWriteFp(":OFFset",value);
			}

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

int CVICALLBACK Deviation (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		double value;
		int modulation_state;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_3,&modulation_state); 
			if (modulation_state==1)
			{
				GetCtrlVal(PANEL, PANEL_NUMERIC_5,&value);
				SerWriteFp(":SOURce:MODFM:SPAN",value); 
			}

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

int CVICALLBACK Frequensy_MOD (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		double value;
		int modulation_state, generator_state;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_3,&modulation_state); 
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_2,&generator_state);
			if ((modulation_state==1)&&(generator_state == 1)) 
			{
				GetCtrlVal(PANEL, PANEL_NUMERIC_4,&value);
				SerWriteFp(":SOURce:MODFM:RATe",value); 
			}

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

int CVICALLBACK Modulation (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		double value;
		int modulation_state, generator_state;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_3,&modulation_state); 
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_2,&generator_state);
			if ((modulation_state==1)&&(generator_state == 1)) 
			{
				SerWriteFp(":SOURce:SOURce",0);
				SerWriteFp(":SOURce:WAVeform",1);
				GetCtrlVal(PANEL, PANEL_NUMERIC_4,&value);
				SerWriteFp(":SOURce:MODFM:RATe",value);
				GetCtrlVal(PANEL, PANEL_NUMERIC_5,&value);
				SerWriteFp(":SOURce:MODFM:SPAN",value);
				SerWriteFp(":SOURce:STATe",2); 
			}
			else if(modulation_state == 0)
			{
				SerWriteFp(":SOURce:STATe",0);
			}

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

int CVICALLBACK Graph (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		int n;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_4,&n);
			SetCtrlAttribute(panelHandle,PANEL_TIMER_2,ATTR_ENABLED,0); 
			SetCtrlAttribute(panelHandle,PANEL_TIMER,ATTR_ENABLED,n); 
			

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

int CVICALLBACK Time1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		int i, n, k;
		double max; 
		case EVENT_TIMER_TICK:
			analogInClk(NULL, SAMPLING_RATE);
			analogInTrigger("APFI0", LEVEL);
			analogRead(0, Signal, N, &n);
			DeleteGraphPlot(panelHandle,PANEL_GRAPH,-1,VAL_IMMEDIATE_DRAW);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH,VAL_BOTTOM_XAXIS,VAL_MANUAL,0,n);
			PlotY(panelHandle,PANEL_GRAPH, Signal, n, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_YELLOW);

			for(k=0;k<N;k++)
			{	
				Signal_re[k]=Signal[k];
				Signal_im[k]=0;
			}				 
			FFT(Signal_re, Signal_im, n);
			for(k=0;k<N;k++)
			{	
				Signal_FFT[k]=sqrt(Signal_re[k]*Signal_re[k]+Signal_im[k]*Signal_im[k]);  
			}
			
			max = 0;
			for(i = 0; i < n; i++)
			{
				if(Signal_FFT[i] > max) 
					max = Signal_FFT[i];
				Signal_X[i] = i*10; 
			}
			for(i = 0; i < n; i++) 
				Signal_FFT[i] /= max;
			
			DeleteGraphPlot(panelHandle,PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW); 
			SetAxisScalingMode(panelHandle,PANEL_GRAPH_2, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.0, 20000.0);
			PlotXY(panelHandle,PANEL_GRAPH_2, Signal_X, Signal_FFT, n, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK Window (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	
			SetCtrlVal(panelHandle, PANEL_RADIOBUTTON_3, 0);
			SetCtrlVal(panelHandle, PANEL_RADIOBUTTON_4, 0);
			SetCtrlVal(panelHandle, PANEL_RADIOBUTTON_5, 0);
			SetCtrlVal (panelHandle, control, 1);
//			LPF(panel, control, event, callbackData, eventData1, eventData2);
			break;
	}
	return 0;
}

int CVICALLBACK Low_and_high (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			SetCtrlVal(panelHandle, PANEL_RADIOBUTTON, 0);
			SetCtrlVal(panelHandle, PANEL_RADIOBUTTON_2, 0);
			SetCtrlVal (panelHandle, control, 1); 
//			LPF(panel, control, event, callbackData, eventData1, eventData2);
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

int CVICALLBACK LPF (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		case EVENT_COMMIT:
			int i, temp, window, length, cutoff_val;
			double cutoff, df, max = 0, fq = .1;
		
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_3, &temp);
			if(temp == 1) window = 1;
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_4, &temp);
			if(temp == 1) window = 2; 
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_5, &temp);
			if(temp == 1) window = 3;
		
			GetCtrlVal(panelHandle, PANEL_NUMERIC_7, &length);
			GetCtrlVal(panelHandle, PANEL_NUMERIC_6, &cutoff_val);
			
			cutoff = (double)cutoff_val/length;

			CalcLPFKernel(window, length, cutoff, &kernel[0]);
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_2, &temp);
			if(temp == 1) 
				SpectralInversion(&kernel[0], length); 
		
			for(i = 0; i < length; i++) 
			{
				Signal_X[i] = (double)i*fq/length;
			}
			
			for(i = 0; i < length; i++) 
			{
				if(max < *(kernel + i)) 
				{
					max = *(kernel + i);
				}
			}
			
			for(i = 0; i < length; i++) 
			{
				*(kernel + i) = *(kernel + i)/max;
			}
		
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_3, -1, VAL_DELAYED_DRAW);  
			SetAxisScalingMode(panelHandle, PANEL_GRAPH_3, VAL_LEFT_YAXIS, VAL_MANUAL, 0, 1);
			SetAxisScalingMode(panelHandle, PANEL_GRAPH_3, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, fq);
			PlotXY(panelHandle, PANEL_GRAPH_3, Signal_X, kernel, length, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
		

			for(int i = 0; i < length; i++) kernel_im[i] = 0;
			
			FFT(kernel, kernel_im, length);
			for(int i = 0; i < length; i++)
			{
				kernel_FFT[i] = abs(sqrt(pow(kernel[i], 2) + pow(kernel_im[i], 2)));
			}
		
			for(i = 0; i < length; i++)
			{
				Signal_X[i] = i;
			}
			
			for(i = 0; i < length; i++)
			{
				if(max < *(kernel_FFT + i))
				{
					max = *(kernel_FFT + i);
				}
			}
			
			for(i = 0; i < length; i++) 
			{
				*(kernel_FFT + i) = *(kernel_FFT + i)/max;
			}
			
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_4, -1, VAL_DELAYED_DRAW);  
			SetAxisScalingMode(panelHandle, PANEL_GRAPH_4, VAL_LEFT_YAXIS, VAL_MANUAL, 0, 1);
			SetAxisScalingMode(panelHandle, PANEL_GRAPH_4, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.0, 20000.0);
			PlotXY(panelHandle, PANEL_GRAPH_4, Signal_X, kernel_FFT, length, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);

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

int CVICALLBACK Time2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		static double kernel[N], kernel_re[N], kernel_im[N], kernel_FFT[N], kernel_FFT_graph[N]; 
		case EVENT_TIMER_TICK:
			int i, n;
			int temp, window, length, cutoff_val;
			double cutoff, df, max = 0, fq = .1;
		
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_3, &temp);
			if(temp == 1) window = 1;
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_4, &temp);
			if(temp == 1) window = 2; 
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_5, &temp);
			if(temp == 1) window = 3;
		
			GetCtrlVal(panelHandle, PANEL_NUMERIC_7, &length);
			GetCtrlVal(panelHandle, PANEL_NUMERIC_6, &cutoff_val);
		
			cutoff = .01*(double)cutoff_val/length; 
		
			CalcLPFKernel(window, length, cutoff, kernel);
			GetCtrlVal(panelHandle, PANEL_RADIOBUTTON_2, &temp);
			if(temp == 1) 
				SpectralInversion(&kernel[0], length);
			
			
			
			analogInClk(NULL, SAMPLING_RATE);
			analogInTrigger("APFI0", LEVEL);
			analogRead(4, Signal, N, &n);

			DeleteGraphPlot(panelHandle,PANEL_GRAPH,-1,VAL_IMMEDIATE_DRAW);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH,VAL_BOTTOM_XAXIS,VAL_MANUAL,0,n);
			PlotY(panelHandle,PANEL_GRAPH, Signal, n, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_YELLOW);
		
			for(int i = 0; i < n; i++)
			{
				Signal_re[i] = Signal[i];
				Signal_im[i] = 0;
			}
			FFT(Signal_re, Signal_im, N);
			max = 0;
			for(int i = 0; i < n; i++)
			{
				Signal_FFT[i] = sqrt(pow(Signal_re[i], 2) + pow(Signal_im[i], 2)); 
				if(max < Signal_FFT[i]) max = Signal_FFT[i];
				Signal_X[i] = i*10;
			}
			for(int i = 0; i < n; i++) 
			{
				Signal_FFT[i] /= max;
			}

			DeleteGraphPlot(panelHandle, PANEL_GRAPH_3, -1, VAL_DELAYED_DRAW);  
			SetAxisScalingMode(panelHandle, PANEL_GRAPH_3, VAL_LEFT_YAXIS, VAL_MANUAL, 0, 1);
			SetAxisScalingMode(panelHandle, PANEL_GRAPH_3, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0, fq);
			PlotXY(panelHandle, PANEL_GRAPH_3, Signal_X, kernel, length, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
		
			for(int i = 0; i < length; i++)
			{
				kernel_re[i] = kernel[i];
				kernel_im[i] = 0;
			}
			FFT(kernel_re, kernel_im, length);
			max = 0;
			for(int i = 0; i < length; i++)
			{
				kernel_FFT[i] = sqrt(pow(kernel_re[i], 2) + pow(kernel_im[i], 2)); 
				if(max < kernel_FFT[i]) 
				{
					max = kernel_FFT[i];
				}
			}
			for(int i = 0; i < length; i++) 
			{
				kernel_FFT_graph[i] = kernel_FFT[i]/max;
			}
		
		
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_4, -1, VAL_DELAYED_DRAW);
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH_2, VAL_BOTTOM_XAXIS, VAL_MANUAL, 0.0, 20000.0);  
			PlotXY(panelHandle, PANEL_GRAPH_2, Signal_X, Signal_FFT, n, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
			SetAxisScalingMode(panelHandle,PANEL_GRAPH_4,VAL_BOTTOM_XAXIS,VAL_MANUAL,0,2*cutoff_val*.01);
			PlotY(panelHandle, PANEL_GRAPH_4, kernel_FFT_graph, length, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_RED);
		

			
	/*		for (int i = length-1;i<N; i++)
			{
				Signal_filtered[i]=0;
				for(int j=0; j<=length-1;j++)
					Signal_filtered[i]+=Signal[i-j]*kernel[j];
			}	 */
			
			ConvolveEx(Signal, n, kernel, length, ALGORITHM_CONCOR_FREQ_DOMAIN, Signal_filtered);
			DeleteGraphPlot(panelHandle, PANEL_GRAPH_5,-1,VAL_IMMEDIATE_DRAW);
			SetAxisScalingMode(panelHandle, PANEL_GRAPH_5,VAL_BOTTOM_XAXIS,VAL_MANUAL,0,n);
			PlotY(panelHandle, PANEL_GRAPH_5, Signal_filtered, n, VAL_DOUBLE, VAL_THIN_LINE, VAL_NO_POINT, VAL_SOLID, 1, VAL_YELLOW);

			break;
		case EVENT_DISCARD:

			break;
	}
	return 0;
}

int CVICALLBACK Speak (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2) {
	switch (event) {
		int n;
		case EVENT_COMMIT:
			GetCtrlVal(PANEL, PANEL_TOGGLEBUTTON_5,&n);
			SetCtrlAttribute(panelHandle,PANEL_TIMER,ATTR_ENABLED,0);  
			SetCtrlAttribute(panelHandle,PANEL_TIMER_2,ATTR_ENABLED,n); 

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
