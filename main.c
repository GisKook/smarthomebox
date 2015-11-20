#include <string.h>
#include "eventhub.h"
#define LISTENPORT "8989"

int main(){ 
	struct eventhubconf hubconf;
	memcpy(&hubconf.port, LISTENPORT, sizeof(LISTENPORT));
	struct eventhub * hub = eventhub_create(&hubconf);
	eventhub_start(hub);
}
