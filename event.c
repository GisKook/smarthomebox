#include <stdio.h>
#include "connection.h"
#include "protocol.h"
#include "toolkit.h"
#include "eventhub.h" 
#include "cetimer.h"
#include "socket.h"
#include "termcontrol.h" 

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
		if( _check_command(buf, buflen, CERECONNSOCKET[0])){
			struct connection * serverconn = connectserver();
			if(serverconn){
				eventhub_register(hub,connection_getfd(serverconn));
			}
		}
		if( _check_command(buf, buflen, CERECONNSERIAL[0])){
			struct connection * connserial = connserialport();

			if(connserial){
				eventhub_register(hub, connection_getfd(connserial));
			}
		}
		if( _check_command(buf, buflen, HEARTBEAT[0])){

			unsigned char heart_buf[255];
			unsigned int hbuflen;
			
			hbuflen = encodeheart(gateway , heat_buf);
			struct list_head *pos, *n;
			list_for_each_safe(pos, n, &connlisthead){
				struct connection * c = list_entry(pos, struct connection, list);
				if(c && (connection_gettype(c) == CONNSOCKETCLIENT || connection_gettype(c) == CONNSOCKETSERVER))
				{
					for(;;){
						int n = write(c->fd,heart_buf,hbuflen);
						if(n < 0){
							fprintf(stdout, "%s\n", strerror(errno));
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
		freeconnlist_add(c);
	}
}
