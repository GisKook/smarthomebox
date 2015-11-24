#include <string.h>
#include <stdio.h>
#include "eventhub.h"
#include "connection.h"
#include "toolkit.h"


#define LISTENPORT "8989"
#define SERVERADDR "192.168.8.42"
#define SERVERPORT "8990"

int main(){ 

	unsigned long long mac = toolkit_getmac();
	fprintf(stdout, "%llu\n", mac);
	struct eventhubconf hubconf;
	memcpy(&hubconf.port, LISTENPORT, sizeof(LISTENPORT));
	struct eventhub * hub = eventhub_create(&hubconf);

	int fd = openclient(SERVERADDR, SERVERPORT);
	struct connection * serverconn = freeconnlist_getconn();
	connection_init(serverconn, fd, CONNSOCKET);
	eventhub_register(hub, fd);
	connrbtree_insert(serverconn);

	eventhub_start(hub);
}
