#include <userint.h>
#include "tsani.h"
#include "avalon.h"

static int panelHandle;
int data, word, word2;
unsigned char ad;
unsigned int subad, adr;
double u,v;

void initial(void)
{
	portMask(0, 0xff);		//поднимаем только write read, AD в режиме записи
	portOut(0, 0x00);
	portMask(1, 0xff);
	portOut(1, 0x00);
	portMask(2, 0x07);
	portOut(2, 0x06);	
}

void add(unsigned char a, unsigned int suba, unsigned int *adr)
{
	*adr=((unsigned int)a)|(suba<<3);	//лепим из адреса и суб-адреса один адрес
}

void divide(unsigned int adr, unsigned char *adr0, unsigned char *adr1)
{
	*adr0=(unsigned char)adr;		  //разделили для 0 и для 1 порта
	*adr1=(unsigned char)(adr>>8);
}

void write(unsigned int adr, unsigned int word)
{
	unsigned char adr0, adr1;
	divide(adr, &adr0, &adr1);
	portOut(0, adr0);
	portOut(1, adr1);
	portOut(2, 0x07); //запись 1 в ALE
	portOut(2, 0x06); //ALE снова 0
	divide(word, &adr0, &adr1);
	portOut(0, adr0);
	portOut(1, adr1);
	portOut(2, 0x02);
	portOut(2, 0x06);
}

void read(unsigned int adr, unsigned int *word)
{
	unsigned char adr0, adr1;
	divide(adr, &adr0, &adr1);
	portOut(0, adr0);
	portOut(1, adr1);
	portOut(2, 0x07);
	portOut(2, 0x06);
	portMask(0, 0x00);   //перевод шины AD в режим чтения
	portMask(1, 0x00);
	portOut(2, 0x04);
	portIn(0, &adr0);
	portIn(1, &adr1);
	portOut(2, 0x06);
	portMask(0, 0xff);   //перевод шины AD в режим записи
	portMask(1, 0xff);
	
	*word=((unsigned int)adr1<<8)|((unsigned int)adr0); //вернуть прочитанные данные
}

void kod(unsigned char k)	  //если приходит >8бит, то остаются только 8 так как чар
{
	unsigned int adr;
	add(0x02,0x00,&adr);
	write(adr,0x03);
	add(0x02,0x02,&adr);
	write(adr,(unsigned int)k);
	add(0x02,0x03,&adr);
	write(adr,(unsigned int)k);
}

void voltage(double v)
{   
	unsigned int n, adr;
	unsigned char kodd;
	if(v<0)
		{
			kodd=0;
		}
	else
		{
			n=(unsigned int)(255*v/3.3);	
			kodd=(unsigned char)n;
			
		}
	add(0x02,0x00,&adr);
	write(adr,0x03);
	add(0x02,0x02,&adr);
	write(adr,(unsigned int)kodd);
	add(0x02, 0x03, &adr);
	write(adr,(unsigned int)kodd);
	
	
}

void setll()
{
	unsigned int adr;
	add(0x02,0x11,&adr);
	write(adr,0x04);
}

void iack()
{
	unsigned int adr; 
	add(0x02,0x11,&adr);
	write(adr,0x02);
}

void acp(double *word2)
{
	unsigned int adr, word;
	
	add(0x02,0x10,&adr);
	write(adr,0x05);
	add(0x02,0x12,&adr);
	write(adr,0x00);
	add(0x02,0x13,&adr); 
	write(adr,0x00);
	add(0x02,0x14,&adr);
	write(adr,0x00);
	add(0x02,0x11,&adr);
	write(adr,0x03);

	do
	{
		read(adr,&word);
	}  while ( (word&0x01) == 0x01 );   
	
	/*do 
	{
		read(adr,&word);
	} while( (word&0x04) != 0x04 );	
		
	iack();   */  
	add(0x02,0x16,&adr);
	read(adr,&word); 
	
	*word2=word*2.56/1024;
}

 int avalon_write(int address, int subaddress, int value) {
	int full_address = address + (subaddress << 3);
	portMask(0, 0xff); //порт0 в режиме записи
	portMask(1, 0xff); //порт1 в режиме записи
	portMask(2, 0x07); //порт2 в режиме записи ALE READ WRITE    
	//check_value_int(&full_address, 0, 65535);  
	//check_value_int(&value, 0, 65535);  
	portOut(0,(full_address & 0xff)); //В AD0-AD7 пишем младшие 8 бит			 
    portOut(1,((full_address >> 8) & 0xff)); //В AD8-AD15 пишем старшие 8 бит
	portOut(2,0x07); //ALE1 READ1 WRITE1
	portOut(2,0x06); //ALE0 READ1 WRITE1
	portOut(0,(value & 0xff)); //Теперь пишем значение value в AD	 
    portOut(1,((value >> 8) & 0xff));
	portOut(2,0x02); //ALE0 READ1 WRITE0
	portOut(2,0x06); //ALE0 READ1 WRITE1
	
	return 0;
}

 int avalon_read(int address, int subaddress, int* value) {
	int full_address = address + (subaddress << 3);
	unsigned char data0, data1;
	portMask(0,0xff); //порт0 в режиме записи
	portMask(1,0xff); //порт1 в режиме записи
	portMask(2,0x07); //порт2 в режиме записи ALE READ WRITE
//	check_value_int(&full_address, 0, 65535);
	portOut(0,(full_address & 0xff)); //В AD0-AD7 пишем младшие 8 бит			 
    portOut(1,((full_address >> 8) & 0xff)); //В AD8-AD15 пишем старшие 8 бит
	portOut(2,0x07); //ALE1 READ1 WRITE1
	portOut(2,0x06); //ALE0 READ1 WRITE1
	portOut(2,0x04); //ALE1 READ0 WRITE0
	portMask(0, 0x00); //порт0 в режиме чтения
	portMask(1, 0x00); //порт1 в режиме чтения
	portIn(0, &data0); //Из AD0-AD7 читаем младшие 8 бит
	portIn(1, &data1); //Из AD8-AD15 читаем старшие 8 бит
	portOut(2, 0x06); //ALE1 READ1 WRITE0
	portMask(0,0xff); //порт0 в режиме записи
	portMask(1,0xff); //порт1 в режиме записи
	*value = (((int)data1 & 0xff) << 8) + ((int)data0 & 0xff);
	
	return 0;
}

