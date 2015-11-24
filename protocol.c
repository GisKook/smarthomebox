#include "connection.h"
#include "bytebuffer.h"

#define ILLEGAL  0
#define HALFPACK 255

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
	unsigned short tmp;
	if(buf[0] != 0xCE){ 
		connection_readbuf_pop(c);
		protocol_check(c, &tmp);
	}else if(len > 2){ 
		unsigned short messagelen=bytebuffer_getword(buf);
		if(messagelen < 8 || messagelen > 2048){
			connection_readbuf_pop(c);
			protocol_check(c,&tmp);
		}
		if(messagelen > len){
			*messageid= HALFPACK;
			return 0;
		}else{
			unsigned char checksum = protocol_checksum(buf, messagelen);
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
