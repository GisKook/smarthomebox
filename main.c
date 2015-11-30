#include <string.h>
#include <stdio.h>
#include "eventhub.h"
#include "connection.h"
#include "toolkit.h"
#include "cetimer.h"
#include "socket.h"
#include "ceconf.h"
#include "connection.h"

int main(){ 
	unsigned long long mac = toolkit_getmac();
	fprintf(stdout, "%llu\n", mac);
	// create pipe
	int wfd;
	struct connection * readconn = createpipe(&wfd);

	// create timer
	struct cetimer * timer = cetimer_create(10, 1, wfd);
	cetimer_start(timer);

	// create eventhub 
	struct eventhubconf hubconf;
	memset(&hubconf, 0, sizeof(struct eventhubconf));
	memcpy(&hubconf.port, ceconf_getlistenport(),strlen(ceconf_getlistenport())); 
	struct eventhub * hub = eventhub_create(&hubconf);
	// connection to server
	struct connection * serverconn = connectserver();
	if(serverconn){
		eventhub_register(hub,connection_getfd(serverconn));
	}
	if(readconn){
		eventhub_register(hub, connection_getfd(readconn));
	}

	eventhub_start(hub);
}
