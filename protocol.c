#include "connection.h"
#include "bytebuffer.h"
#include "protocol.h"


unsigned char protocol_checksum(unsigned char * buf, unsigned int buflen){
	unsigned char temp = buf[0];
	int i = 1;
	for(; i < buflen; i++){
		temp ^= buf[i];
	}

	return temp;
}

#define BUFLEN 1024
int protocol_check(struct connection * c, unsigned short * messageid){ 
	unsigned char buf[BUFLEN] = {0};
	unsigned int len = connection_readbuf_getahead(c, buf, BUFLEN);
	if(len == 0){
		messageid = ILLEGAL;

		return 0;
	}
	unsigned short tmp;
	if(buf[0] != 0xCE){ 
		connection_readbuf_pop(c);
		protocol_check(c, &tmp);
	}else if(len > 2){ 
		unsigned short messagelen=bytebuffer_getword(&buf[1]);
		if(messagelen < 8 || messagelen > 2048){
			connection_readbuf_pop(c);
			protocol_check(c,&tmp);
		}
		if(messagelen > len){
			*messageid= HALFPACK;
			return 0;
		}else{
			unsigned char checksum = protocol_checksum(buf, messagelen-2);
			if(checksum == buf[messagelen - 2] && buf[messagelen - 1] == 0xCE){ 
				unsigned short cmdid = bytebuffer_getword(&buf[3]);
				*messageid = cmdid;
				return messagelen;
			}else{
				connection_readbuf_pop(c);
				protocol_check(c, &tmp); 
			}
		}
 

	}else{
		*messageid = HALFPACK;
		return 0;
	}
}

/*******************************************************************************************
 * ZNP数据格式
 *
 * Bytes:  1     3-253 General format frame                    1
 *        SOF    Length(1)  Cmd(2)   Data(0-253)              FCS
 ********************************************************************************************/
int znpframe_check(struct connection *c, unsigned short *messageid) {
	unsigned char buf[BUFLEN] = {0};
	unsigned int len = connection_readbuf_getahead(c, buf, BUFLEN);
	if(len == 0) {
		messageid = ILLEGAL;
		return 0;
	}
	unsigned short tmp;
	if(buf[0] != 0xFE){ //SOF: Start of frame indicator. This is always set to 0xFE.
		connection_readbuf_pop(c);
		znpframe_check(c, &tmp);
	}else if(len > 2) {
		unsigned char datalen = buf[1];
		if(datalen < 0 || datalen > 250) {//The length of the data field of the frame. The length can range from 0-250.
			connection_readbuf_pop(c);
			znpframe_check(c, &tmp);
		}
		unsigned char messagelen = datalen + 5;
		if(messagelen > len){
			*messageid = HALFPACK;
			return 0;
		}else {
			unsigned char checksum = protocol_checksum(buf + 1, datalen + 3); //This field is computed as an XOR of all the bytes in the general format frame fields.
			if(checksum == buf[messagelen - 1]) {
				unsigned short cmdid = bytebuffer_getword(buf + 2);
				*messageid = cmdid;
				return messagelen;
			}else {	
				connection_readbuf_pop(c);
				znpframe_check(c, &tmp);
			}
		}		
	}else{
			*messageid = HALFPACK;
			return 0;
	}
}
