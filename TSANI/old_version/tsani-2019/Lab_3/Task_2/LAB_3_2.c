#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <tsani.h>
#include "LAB_3_2.h"
#include "toolbox.h"
#include "avalon.h"



static int panelHandle;

double u, dif[1024], arr[1024], ide[1024], arr_2[1024];
int k = 0; 
int	h = 0;
double integ = 0; 

int dac_adc_in(double* value){
	int temp = 0x03;
	int subaddress;
	subaddress = 0x17;
	avalon_write(2, 0x12, 0x00); 
	avalon_write(2, 0x13, 0x00);
	avalon_write(2, 0x14, 0x00);		       
	avalon_write(2, 0x11, 0x03); 
	while((temp & 0x01) == 0x01)
	    avalon_read(2, 0x11, &temp);
	avalon_read(2, subaddress, &temp);
	*value = (double)(temp*(2.56 / 1024));
	
	return 0;
}

void per()
{
	double val = 0;
	for(int i = 0; i < 1024; i++)
	{
		avalon_write(2, 2, i);
		u = i * 2.56 / 1024 ;
		analogOut(0, u);
		dac_adc_in(&val);
		arr[k] = val;
		arr_2[k] = val;
		k++;
		ide[i]=2.56 * i / 1024;     
	}
	
	for(int i = 0; i < 1024; i++)
	{
		arr_2[i] = arr_2[i] + arr[5];             		
	}
	
	for(int i = 0; i < 1024; i++)
	{
		arr_2[i] = arr_2[i] / arr_2[1010] * ide[1010] ;
	}

	PlotY(panelHandle,PANEL_GRAPH,arr,1024,VAL_DOUBLE,VAL_THIN_LINE,VAL_SOLID_SQUARE,VAL_SOLID,1,VAL_RED);
	PlotY(panelHandle,PANEL_GRAPH,arr_2,1024,VAL_DOUBLE,VAL_THIN_LINE,VAL_SOLID_SQUARE,VAL_SOLID,1,VAL_BLUE);
	PlotY(panelHandle,PANEL_GRAPH,ide,1024,VAL_DOUBLE,VAL_THIN_LINE,VAL_SOLID_SQUARE,VAL_SOLID,1,VAL_GREEN);

}


void inl()
{
	int i;
	integ = 0;
	for(i = 0; i < 1024; i++)
	{
		dif[i] = (arr_2[i] - ide[i]) / 2.56 * 1024;
		if(integ < dif[i]) integ = dif[i];
	}
	
	PlotY(panelHandle,PANEL_GRAPH_2,dif,1023,VAL_DOUBLE,VAL_THIN_LINE,VAL_SOLID_SQUARE,VAL_SOLID,1,VAL_GREEN);
	//SetCtrlVal(panelHandle,PANEL_NUMERIC,integ);
}

int main (int argc, char *argv[])
{
    int error = 0;
    
    /* initialize and load resources */
    nullChk (InitCVIRTE (0, argv, 0));
    errChk (panelHandle = LoadPanel (0, "LAB_3_2.uir", PANEL));
	ni6251Slot(2);
	initial();
    per();
	inl();
    /* display the panel and run the user interface */
    errChk (DisplayPanel (panelHandle));
    errChk (RunUserInterface ());
	ni6251Close();

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
