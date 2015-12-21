#include <stdlib.h>
#include <stdio.h>
#include "bytebuffer.h"
#include "bussinessdata.h"

unsigned char dataparse(Bussinessdata *data, unsigned char *buffer, int len)
{
	const unsigned char * tempdata = buffer;
	unsigned char tempstatus = 0;
	unsigned char tempextstatus = 0;
	//0xCE
	bytebuffer_skipbyte(&tempdata);
	//len
	bytebuffer_skipbyte(&tempdata);
	//zoneStatus
	bytebuffer_readbyte(&tempdata, &tempstatus);
	data->status.Alarm1 = ( tempstatus & 0x01 );			
	data->status.Alarm2 = ( tempstatus & 0x02 )>>1;		
	data->status.Tamper = ( tempstatus & 0x04 )>>2;		
	data->status.Battery = ( tempstatus & 0x08 )>>3;		
	data->status.Supreprots= ( tempstatus & 0x10 )>>4;		
	data->status.Restorereps = ( tempstatus & 0x20 )>>5;		
	data->status.Trouble = ( tempstatus & 0x40 )>>6;		
	data->status.Acmains = ( tempstatus & 0x80 )>>7;
	bytebuffer_skipbyte(&tempdata);				
	//extendedStatus
	bytebuffer_readbyte(&tempdata, &tempextstatus);
	data->extrastatus.ZoneID = ( tempextstatus & 0x3f );
	data->extrastatus.Changeorheart = (( tempextstatus & 0xC0) >> 6);
	//zoneId
	bytebuffer_readbyte(&tempdata, &data->zoneID);
	//delay
	bytebuffer_readword(&tempdata, &data->delay);
	//shortAddr
	bytebuffer_readword(&tempdata, &data->shortaddr);
	//zoneType
	bytebuffer_readword(&tempdata, &data->zonetype);

	return 0;
}
