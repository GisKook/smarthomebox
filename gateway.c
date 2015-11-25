#include <string.h>
#include "list.h"

#define MAXNAMELEN 255

struct endpoint{
	unsigned char ep;
	unsigned short clusteridcount;
	unsigned short * clusterid;
};

struct device{
	struct list_head list;
	unsigned short deviceid;
	char devicename[MAXNAMELEN];
	unsigned short endpointcount;
	struct endpoint * endpoint;
	unsigned char devicetype;
	unsigned char devicestatus;
	unsigned short company;
};

struct gateway{
	unsigned long long gatewayid;
	char gatewayname[MAXNAMELEN];
	unsigned short devicecount;
	unsigned char boxversion;
	unsigned char protocolversion;
	struct list_head head;
};


static struct gateway gatewayinstance;

struct gateway * getgateway(){
	return &gatewayinstance;
}

void geteway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxverdion, unsigned char protocolversion){ 
	gw->gatewayid = gatewayid;
	memset(gw->gatewayname, 0, MAXNAMELEN);
	memcpy(gw->gatewayname, gatewayname, min(strlen(gatename), MAXNAMELEN-1));
	gw->boxversion = boxversion;
	gw->protocolversion = protocolversion;
	INIT_LIST_HEAD(&gw->head);
}

void gateway_adddevice(struct gateway * gw, struct device * d){ 

}
