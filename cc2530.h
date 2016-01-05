#ifndef __CC2530_H_H_
#define __CC2530_H_H_

struct af_register{
	unsigned char endpoint;
	unsigned short appprofid;
	unsigned short appdeviceid;
	unsigned char appdevversion;
	unsigned char latencyreq;
	unsigned char appnuminclusters;
	unsigned short appinclusterlist[16];
	unsigned char appnumoutclusters;
	unsigned short appoutclusterlist[16];
};

void cc2530_startup(int fd);
void cc2530_startrequst(int fd);
void cc2530_af_register(int fd, struct af_register * af_reg);
void cc2530_zdo_startup_from_app(int fd, unsigned short startdelay);
#endif
