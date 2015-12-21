#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "connection.h"
#include "protocol.h"
#include "toolkit.h"
#include "eventhub.h" 
#include "cetimer.h"
#include "socket.h"
#include "termcontrol.h" 
#include "gateway.h"
#include "list.h"
#include "bussinessdata.h"

void event_accept(int fd){
	struct connection * c = freeconnlist_getconn();
	connection_init(c, fd, CONNSOCKETCLIENT);
	connrbtree_insert(c);
}


int _check_command(unsigned char * buffer, int buflen, unsigned char command){
	int i = 0;
	for(; i < buflen; i++){
		if(buffer[i] == command){
			return 1;
		}
	}

	return 0;
}

void event_recvmsg(struct eventhub * hub, int fd, unsigned char * buf, int buflen){
	fprintf(stdout, "IN ");
	toolkit_printbytes(buf, buflen);
	struct connection * c = connrbtree_getconn(fd);
	if(c && connection_gettype(c) == CONNSOCKETCMD){ 
		if( _check_command(buf, buflen, CECHECK[0])){
			time_t t = time(NULL);
			connlist_checkstatus(t);
		}
		if( _check_command(buf, buflen, HEARTBEAT[0])){

			unsigned char heart_buf[255];
			unsigned int hbuflen;

			hbuflen = encode_heart(getgateway(), heart_buf);
			struct list_head *pos, *n;
			list_for_each_safe(pos, n, connlist_get()){
				struct connection *c = list_entry(pos, struct connection, list);
				if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER))
				{
					for(;;){
						int n = write(connection_getfd(c),heart_buf,hbuflen);
						if(n < 0){
							fprintf(stdout, "%s \n", strerror(errno));
						}
						break;
					}
				}
			}
		}
	}else if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER)){
		connection_put(c, buf, buflen); 
		unsigned short messageid = 0;
		int messagelen = protocol_check(c, &messageid);
		char buffer[1024] = {0};
		connection_get(c,buffer, messagelen);
		switch(messageid){
			case LOGINFEEDBACK:
			case HEARTFEEDBACK:
				break;
			case REQDEVICELIST:
				break;
			case REQOPERATE:
				break;
			case REQSETDEVICENAME:
				break;
			case REQDELDEVICE:
				break;
			case ILLEGAL:
				break;
		}
	}else if(c && connection_gettype(c) == CONNSERIALPORT){
		connection_put(c, buf, buflen);
		unsigned short messageid = 0;	
		int messagelen = znpframe_check(c, &messageid);
		char buffer[1024] = {0};
		connection_get(c, buffer, messagelen);

		Bussinessdata payload;
		memset(&payload, 0, sizeof(Bussinessdata));	

		switch(messageid){
			case  AFINCOMINGDATA:
				break;
			case ZDOSTATECHANGE:
				break;
			case SYSRESETIND:
				break;
			case ZBWRITERSP:
				break;
			case AFREGISTERRSP:
				break;
			case ZDOSTARTUPRSP:
				break;
			case AFDATAREQRSP:
				break;



			case BUSSINESSDATA:
				{
					dataparse(&payload, buffer, messagelen);	
					char databuf[255]={0};
					int datalen;
					datalen = encode_alarm(getgateway(), databuf,&payload);
					struct list_head *pos, *n;
					list_for_each_safe(pos, n, connlist_get()){
					struct connection *c = list_entry(pos, struct connection, list);
					if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER))
							{
							
									int n = sendnonblocking(connection_getfd(c),databuf,datalen);
						}


				}
				break;


			case ILLEGAL:
				break;
			default:
				printf("unrecognized serial messageid\r\n");
		}		
	}
}

void event_close(int fd){
	struct connection * c = connrbtree_getconn(fd);
	if(c){ 
		connrbtree_del(c);
	}
}
