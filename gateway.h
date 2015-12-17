#ifndef __CE_GATEWAY_H_H
#define __CE_GATEWAY_H_H

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
struct device * device_create(unsigned short deviceid);
void device_addcluster(struct device *d, unsigned char ep, unsigned short clusterid, char * name); 
struct gateway * getgateway();
void geteway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion);
void gateway_adddevice(struct gateway * gw, struct device * d);
void gateway_deldevice(struct gateway * gw, struct device *d);

#endif
