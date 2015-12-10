#include <stdio.h>
#include "sq.h"
#include "bytebuffer.h"

char setdevicename(struct sqlitedb *sdb,buf)
{ 

	char * p = buf+16;
	char * p1 = buf+22;
	char * p2 = buf+23;
	unsigned int deviceIDlen,devicenamelen;
	unsigned char devicenamelen1;
	unsigned long long  deviceID1;  
	unsigned int deviceID[2] = {0};
	unsigned char devicename[255] = {0};
	unsigned int did;
	bytebuffer_readmac(&p,&deviceID1);

	bytebuffer_readbyte(&p1,&devicenamelen1);
	bytebuffer_readbytes( &p2, devicename, devicenamelen);
	devicename[devicenamelen] = 0;
	sqlitedb_updata_data(sdb,deviceID1,devicename);
}



#define  LOGIN     0x0003

unsigned char buf[1024];
unsigned int encodelogin(struct gateway *gw, char *buf)
{
	char *p = buf;
	bytebuffer_writebyte(&p,0xCE);
	bytebuffer_writeword(&p,0x0000);
	bytebuffer_writeword(&p,LOGIN);
	bytebuffer_writemac(&p,gateway->gatewayid);
	bytebuffer_writeword(&p,gateway->devicecount);

	struct list_head *pos, *n;
	struct device *temp;
	list_for_each_safe(pos, n,&gw->head)
		{
		temp=list_entry(pos, struct device, list);
//		unsigned char deviceidlen = strlen(temp->deviceid);
		bytebuffer_writebyte(&p, 0X06);
		bytebuffer_writebytes(&p, temp->deviceid,0X06);
		bytebuffer_writeword(&p,temp->company);
		bytebuffer_writebyte(&p,temp->devicestatus);
		unsigned char devicenamelen = strlen(temp->devicename);
		bytebuffer_writebyte(&p, devicenamelen);
		bytebuffer_writebytes(&p, temp->devicename,devicenamelen);
		}	
	unsigned int templen = p-buf;
	
	char *p1=buf+1;

	bytebuffer_writeword(&p1,templen+2);
	unsigned checksun = encode_checksum(buf,templen);
	bytebuffer_writebyte(&p,checksum);
	bytebuffer_writebyte(&p,0XCE);
 	
	unsigned int buflen = p-buf;

	for(;;){
		int n = write(events[i].data.fd,buf,buflen);
		if(n < 0){ 
		fprintf(stdout, "%s\n", strerror(errno));
 		}
			break;
		}
	
	return buflen;

}


#define  HEART     0x0002
unsigned char heart_buf[255];
unsigned int encodeheart(struct gateway *gw, char *buf)
 {
        char *p = buf;
        bytebuffer_writebyte(&p,0xCE);
        bytebuffer_writeword(&p,0x0D);
        bytebuffer_writeword(&p,HEART);
	bytebuffer_writemac(&p,gateway->gatewayid);
	
	unsigned templen = p-buf;
        unsigned checksun = encode_checksum(buf,templen);
        bytebuffer_writebyte(&p,checksum);
        bytebuffer_writebyte(&p,0XCE);
  
        unsigned int buflen = p-buf;
  
  
          return buflen;
 } 

 unsigned char encode_checksum(unsigned char * buf, unsigned int buflen){
          unsigned char temp = buf[0];

          int i = 1;
          for(; i < buflen; i++){
                  temp ^= buf[i];
          }
  
          return temp;
  }


















