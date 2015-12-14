#include <string.h>
#include <stdio.h>
#include "eventhub.h"
#include "connection.h"
#include "toolkit.h"
#include "cetimer.h"
#include "socket.h"
#include "ceconf.h"
#include "connection.h"
#include "termcontrol.h"
#include "reconn.h"

int main(){ 
	unsigned long long mac = toolkit_getmac();
	fprintf(stdout, "%llu\n", mac);

	// create pipe for timer to main
	int wfd;
	struct connection * readconn = createpipe(&wfd);

	// create pipe for timer to reconnect
	int reconnrfd, reconnwfd;
	reconnrfd = createpipe2(&reconnwfd);

	// create timer
	struct cetimer * timer = cetimer_create(10, 1, wfd, reconnwfd);
	cetimer_start(timer);
	

	// create eventhub 
	struct eventhubconf hubconf;
	memset(&hubconf, 0, sizeof(struct eventhubconf));
	memcpy(&hubconf.port, ceconf_getlistenport(),strlen(ceconf_getlistenport())); 
	struct eventhub * hub = eventhub_create(&hubconf);

	// start reconn thread
	reconn_start(reconnrfd, hub);
	
	// connection to server
	//struct connection * serverconn = connectserver();
	//if(serverconn){
	//	eventhub_register(hub,connection_getfd(serverconn));
	//}

	// add cmd pipe
	if(readconn){
		eventhub_register(hub, connection_getfd(readconn));
	}

	// conn to serial port
	//struct connection * connserial = connserialport();
	//if(connserial){ 
	//	eventhub_register(hub, connection_getfd(connserial));
	//}

	eventhub_start(hub);
}
