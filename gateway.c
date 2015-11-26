#include <stdlib.h>
#include <string.h>
#include "list.h"

#define MAXNAMELEN 255
#define MAXCLUSTER 36

struct endpoint{
	struct list_head list;
	unsigned char ep;
	unsigned short clusteridcount;
	unsigned short clusterid[MAXCLUSTER];
};

struct device{
	struct list_head list;
	unsigned short deviceid;
	char devicename[MAXNAMELEN];
	unsigned short endpointcount;
	struct list_head eplisthead;
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

struct endpoint * _endpiont_create(unsigned char ep, unsigned short clusterid){ 
	struct endpoint * endpoint = (struct endpoint *)malloc(sizeof(struct endpoint));
	memset(endpoint, 0, sizeof(struct endpoint));
	INIT_LIST_HEAD(&endpoint->list);
	endpoint->ep = ep;
	endpoint->clusterid[endpoint->clusteridcount] = clusterid;
	endpoint->clusteridcount++; 

	return endpoint;
}

struct device * device_create(unsigned short deviceid){
	struct device * device = (struct device *)malloc(sizeof(struct device)); 
	memset(device, 0, sizeof(struct device));
	INIT_LIST_HEAD(&device->list);
	INIT_LIST_HEAD(&device->eplisthead);
	device->deviceid = deviceid;

	return device; 
}

struct endpoint * device_getep(struct device * d, unsigned short ep){
	struct list_head *pos, *n;
	struct endpoint * endpoint;
	list_for_each_safe(pos, n, &d->eplisthead){ 
		endpoint = list_entry(pos, struct endpoint, list);
		if (endpoint->ep == ep){
			return endpoint;
		}
	}

	return NULL;
}

void device_addcluster(struct device *d, unsigned char ep, unsigned short clusterid, char * name){ 
	if(strlen(d->devicename) == 0){
		memcpy(d->devicename, name,min(strlen(name), MAXNAMELEN-1));
	}
	struct endpoint * endpoint = NULL;
	if((endpoint = device_getep(d, ep))){
		endpoint->clusterid[endpoint->clusteridcount] = clusterid;
		endpoint->clusteridcount++;
	}else{ 
		endpoint = _endpiont_create(ep, clusterid);
		list_add_tail(&d->eplisthead, &endpoint->list);
	}
}

static struct gateway gatewayinstance;

struct gateway * getgateway(){
	return &gatewayinstance;
}

void geteway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion){ 
	gw->gatewayid = gatewayid;
	memset(gw->gatewayname, 0, MAXNAMELEN);
	memcpy(gw->gatewayname, gatewayname, min(strlen(gatewayname), MAXNAMELEN-1));
	gw->boxversion = boxversion;
	gw->protocolversion = protocolversion;
	INIT_LIST_HEAD(&gw->head);
}

void gateway_adddevice(struct gateway * gw, struct device * d){
	list_add_tail(&d->list, &gw->head);
}

void gateway_deldevice(struct gateway * gw, struct device *d){
	list_del(&d->list);
	struct list_head *pos, *n;
	struct endpoint * endpoint;
	list_for_each_safe(pos, n, &d->eplisthead){ 
		endpoint = list_entry(pos, struct endpoint, list);
		list_del(pos);
		free(endpoint);
	}

	free(d);
}
