#include <stdio.h>
#include <sys/time.h>
#include "sq.h"
#include "bytebuffer.h"
#include "gateway.h"
#include "bussinessdata.h"

unsigned char encode_checksum(unsigned char * buf, unsigned int buflen);
//unsigned char parse_setdevicename(struct sqlitedb *sdb,buf)
//{ 
//
//	char * p = buf+15;
//	char * p1 = buf+21;
//	char * p2 = buf+22;
//	unsigned int deviceIDlen,devicenamelen;
//	unsigned char devicenamelen1;
//	unsigned long long  deviceID1;  
//	unsigned int deviceID[2] = {0};
//	unsigned char devicename[255] = {0};
//	unsigned int did;
//	bytebuffer_readmac(&p,&deviceID1);
//
//	bytebuffer_readbyte(&p1,&devicenamelen1);
//	bytebuffer_readbytes( &p2, devicename, devicenamelen);
//	devicename[devicenamelen] = 0;
//	sqlitedb_updata_data(sdb,deviceID1,devicename);
//}




//unsigned char parse_deldevice(struct sqlitedb *sdb, unsigned char * buf)
//{
//	unsigned char *p = buf+15;
//	unsigned long long  deviceID;
//	bytebuffer_readmac((const unsigned char **)&p,&deviceID);
//	sqlitedb_delete_id(sdb, "save_date",deviceID);
//}





#define  LOGIN     0x0003
unsigned int encode_login(struct gateway *gw, unsigned char *buf)
{
	unsigned char *p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0x0000);
	bytebuffer_writeword(&p,LOGIN);
	bytebuffer_writemac(&p,gw->gatewayid);
	bytebuffer_writeword(&p,gw->devicecount);

	struct list_head *pos, *n;
	struct device *temp;
	list_for_each_safe(pos, n,&gw->head)
	{
		temp=list_entry(pos, struct device, list);
		//		unsigned char deviceidlen = strlen(temp->deviceid);
		bytebuffer_writebyte(&p, 0X06);
		bytebuffer_writemac(&p, temp->deviceid);
		bytebuffer_writeword(&p,temp->company);
		bytebuffer_writebyte(&p,temp->devicestatus);
		unsigned char devicenamelen = strlen(temp->devicename);
		bytebuffer_writebyte(&p, devicenamelen);
		bytebuffer_writebytes(&p, temp->devicename,devicenamelen);
	}	
	unsigned int templen = p-buf;

	unsigned char *p1=buf+1;

	bytebuffer_writeword(&p1,templen+2);
	unsigned checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	unsigned int buflen = p-buf;


	return buflen;

}


#define  HEART     0x0002
unsigned int encode_heart(struct gateway *gw, unsigned char *buf)
{
	unsigned char *p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0x0D);
	bytebuffer_writeword(&p,HEART);
	bytebuffer_writemac(&p,gw->gatewayid);

	unsigned int templen = p-buf;
	unsigned char checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	unsigned int buflen = p-buf;


	return buflen;
} 

#define OPERBACK   0x0004
unsigned int encode_operback(struct gateway *gw, unsigned char *buf,unsigned short serialID, unsigned char result)
{
	unsigned char *p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0X12);
	bytebuffer_writeword(&p,OPERBACK);
	bytebuffer_writemac(&p,gw->gatewayid);
	bytebuffer_writedword(&p,serialID);
	bytebuffer_writebyte(&p,result);

	unsigned int templen = p-buf;
	unsigned char checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	return 0X12;

}

#define ALARM  0X0006
unsigned int encode_alarm(struct gateway *gw, unsigned char *buf,Bussinessdata *data)
{
	unsigned char *p = buf;
	unsigned long long ctime;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0X1E);
	bytebuffer_writeword(&p,ALARM);
	bytebuffer_writemac(&p,gw->gatewayid);
	bytebuffer_writebyte(&p,data->zonetype);
	bytebuffer_writebyte(&p,0x06);
	bytebuffer_writemac(&p,0x000000000001);

	ctime = time(NULL);
	bytebuffer_writequadword(&p,ctime);
	bytebuffer_writebyte(&p,data->status.Alarm1);

	unsigned int templen = p-buf;
	unsigned char checksum = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);

	return 0X1E;

}



/*
#define LOGBACK	 0x8001
unsigned int parse_logback(struct gateway *gw, char *buf, unsigned char result)
{
char *p = buf;
bytebuffer_writebyte(&p,0xCE);
bytebuffer_writeword(&p,0x0E);
bytebuffer_writeword(&p,LOGBACK);


unsigned int templen = p-buf;
unsigned char checksun = encode_checksum(buf,templen);
bytebuffer_writebyte(&p,checksum);
bytebuffer_writebyte(&p,0XCE);

return 0X0E;
}


#define HEARTBACK  0x8001
unsigned int parse_heartback(struct gateway *gw, char *buf)
{
char *p = buf;
bytebuffer_writebyte(&p,0xCE);
bytebuffer_writeword(&p,0x0D);
bytebuffer_writeword(&p,HEARTBACK);
bytebuffer_writemac(&p,gateway->gatewayid);

unsigned int templen = p-buf;
unsigned char checksun = encode_checksum(buf,templen);
bytebuffer_writebyte(&p,checksum);
bytebuffer_writebyte(&p,0XCE);

return 0X0D;
}

#define QUREYLIST  0x8003
unsigned int parse_querylist(struct gateway *gw, char *buf)
{
char *p = buf;
bytebuffer_writebyte(&p,0xCE);
bytebuffer_writeword(&p,0x0D);
bytebuffer_writeword(&p,QUREYLIST);
bytebuffer_writemac(&p,gateway->gatewayid);

unsigned int templen = p-buf;
unsigned char checksun = encode_checksum(buf,templen);
bytebuffer_writebyte(&p,checksum);
bytebuffer_writebyte(&p,0XCE);

return 0X0D;
}

 */


unsigned char encode_checksum(unsigned char * buf, unsigned int buflen){
	unsigned char temp = buf[0];

	int i = 1;
	for(; i < buflen; i++){
		temp ^= buf[i];
	}

	return temp;
}









