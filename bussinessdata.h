#ifndef _BUSSINESSDATA_H_
#define _BUSSINESSDATA_H_


typedef struct tagzoneStatus
{
	unsigned char Alarm1;
	unsigned char Alarm2;
	unsigned char Tamper;
	unsigned char Battery;
	unsigned char Supreprots;
	unsigned char Restorereps;
	unsigned char Trouble;
	unsigned char Acmains;
}ZoneStatus;


typedef struct tagextendStatus
{
	unsigned char ZoneID;
	unsigned char Changeorheart;
}ExtendStatus;


typedef struct tagbussinessdata
{
	ZoneStatus status;
	ExtendStatus extrastatus;
	unsigned char zoneID;
	unsigned short delay;
	unsigned short shortaddr;
	unsigned short zonetype;
}Bussinessdata;


unsigned char dataparse(Bussinessdata *data, unsigned char *buffer, int len);

#endif
