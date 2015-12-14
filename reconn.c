#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include "eventhub.h"
#include "connection.h"
#include "socket.h"
#include "termcontrol.h"

struct reconn{ 
	int rfd;
	pthread_t reconnthread;
	struct eventhub * hub;

};

void event_reconnect(struct eventhub * hub){
	struct list_head * head = connlist_get();
	if(!connlist_check(CONNSOCKETSERVER)){
		struct connection * serverconn = connectserver();
		if(serverconn){
			eventhub_register(hub,connection_getfd(serverconn));
		}
	}
	if(!connlist_check(CONNSERIALPORT)){
		struct connection * serialconn = connserialport();
		if(serialconn){
			eventhub_register(hub,connection_getfd(serialconn));
		}
	}
}

void * ceconnect(void * args){ 
	struct reconn * rc = (struct reconn *)args;
	int rfd = rc->rfd;
	struct eventhub * hub = rc->hub;
	for(;;){
		ssize_t count;
		char buf[1024];

		count = read (rfd, buf, sizeof buf);
		fprintf(stdout, "start to reconnect ...\n");
		event_reconnect(hub);
	}
}

void reconn_start(int rfd, struct eventhub * hub){
	struct reconn  * rc = (struct reconn *)malloc(sizeof(struct reconn));
	memset(rc, 0, sizeof(struct reconn));
	rc->rfd = rfd;
	rc->hub = hub;
	pthread_t threadid;
	pthread_create(&threadid, NULL, &ceconnect, rc);
}
