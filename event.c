#include <stdio.h>
#include "connection.h"
#include "protocol.h"
#include "toolkit.h"
#include "eventhub.h" 
#include "cetimer.h"

void event_accept(int fd){
	struct connection * c = freeconnlist_getconn();
	connection_init(c, fd, CONNSOCKETCLIENT);
	connrbtree_insert(c);
}

void event_recvmsg(struct eventhub * hub, int fd, unsigned char * buf, int buflen){
	fprintf(stdout, "IN ");
	toolkit_printbytes(buf, buflen);
	struct connection * c = connrbtree_getconn(fd);
	if(c && connection_gettype(c) == CONNSOCKETCMD){ 
		if(buf[0] == CERECONNSOCKET[0]){
			struct connection * serverconn = connectserver();
			if(serverconn){
				eventhub_register(hub,connection_getfd(serverconn));
			}
		}
		if(buf[0] == CERECONNSERIAL[0]){
			struct connection * connserial = connserialport();

			if(connserial){
				eventhub_register(hub, connection_getfd(connserial));
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
		fprintf(stdout, "serial \n");
		getchar();
		fprintf(stdout, "serial2 \n");
		
	}
}

void event_close(int fd){
	struct connection * c = connrbtree_getconn(fd);
	if(c){ 
		connrbtree_del(c);
		freeconnlist_add(c);
	}
}
