#include "tsani.h"
#include <utility.h>


int check_value_int(int* value, int min, int max) //Проверяет в диапазоне ли значение и возвращает в диапазон, если нет  
{
	if (*value > max)	*value = max;
	else if (*value < min)	*value = min;
	
	return 0;
}

int check_value_double(double* value, double min, double max) //Проверяет в диапазоне ли значение и возвращает в диапазон, если нет  
{
	if (*value > max)	*value = max;
	else if (*value < min)	*value = min;
	
	return 0;
}

int avalon_init(void)
{
	portMask(0,0xff); 
	portMask(1,0xff); 
	portMask(2,0x07); 
	portOut(0,0x00);
	portOut(1,0x00);
	portOut(2,0x06); 
	
	return 0;
}

int avalon_write(int address, int subaddress, int value)
{
	int full_address = address + (subaddress << 3);
	portMask(0, 0xff); 
	portMask(1, 0xff); 
	portMask(2, 0x07); 
	
	check_value_int(&full_address, 0, 65535);  
	check_value_int(&value, 0, 65535);
	
	portOut(0,(full_address & 0xff)); 			 
    portOut(1,((full_address >> 8) & 0xff));
	
	portOut(2,0x07);
	portOut(2,0x06); 
	
	portOut(0,(value & 0xff));	 
    portOut(1,((value >> 8) & 0xff));
	
	portOut(2,0x02);
	portOut(2,0x06);
	
	return 0;
}

int avalon_read(int address, int subaddress, int* value)
{
	int full_address = address + (subaddress << 3);
	unsigned char data0, data1;
	portMask(0,0xff); 
	portMask(1,0xff); 
	portMask(2,0x07); 
	
	check_value_int(&full_address, 0, 65535);
	
	portOut(0,(full_address & 0xff)); 			 
    portOut(1,((full_address >> 8) & 0xff)); 
	
	portOut(2,0x07);
	portOut(2,0x06); 
	portOut(2,0x04);
	
	portMask(0, 0x00); 
	portMask(1, 0x00); 
	
	portIn(0, &data0); 
	portIn(1, &data1); 
	
	portOut(2, 0x06); 
	
	portMask(0,0xff); 
	portMask(1,0xff);
	
	*value = (((int)data1 & 0xff) << 8) + ((int)data0 & 0xff);
	
	return 0;
}



int Define_Your_Functions_Here (int x)
{
    return x;
}
