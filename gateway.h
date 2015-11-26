struct device;
struct gateway;

struct device * device_create(unsigned short deviceid);
void device_addcluster(struct device *d, unsigned char ep, unsigned short clusterid, char * name); 
struct gateway * getgateway();
void geteway_init(struct gateway * gw,unsigned long long gatewayid, char * gatewayname, unsigned char boxversion, unsigned char protocolversion);
void gateway_adddevice(struct gateway * gw, struct device * d);
void gateway_deldevice(struct gateway * gw, struct device *d);
