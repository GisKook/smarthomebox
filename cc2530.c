
/*
 * CC2530ZNP Interface Specification.pdf
3.2  CC2530-ZNP startup procedure
After executing the power-up procedure, the host processor must call some mandatory APIs 
before executing any APIs that invoke ZigBee over-the-air messaging. Not following this 
sequence could result in unexpected behaviour. The recommended startup procedure is as 
follows:
1.  The host processor must use the ZB_WRITE_CONFIGURATION command to configure 
at the minimum the ZCD_NV_LOGICAL_TYPE, ZCD_NV_PAN_ID, and 
ZCD_NV_CHANLIST configuration items. 
2.  If the Simple API is used, the ZB_APP_REGISTER_REQUEST command should be sent 
by the host processor to register the application endpoint. 
3.  The ZB_START_REQUEST command should be sent by the host processor to either 
form a network (if the device is a coordinator) or join a network (if the device is a router or 
end device). 
4.  The host processor should then wait for the ZB_START_CONFIRM command with a 
status of ZB_SUCCESS before performing any other API operations.
5.  If the Simple API is not used after performing step 1, the AF_REGISTER command 
should be sent by the host processor to register the application endpoint. 
6.  The ZDO_STARTUP_FROM_APP command should be sent by the host processor to 
either form a network (if the device is a coordinator) or join a network (if the device is a 
router or end device). 
7.  The host processor should then wait for the ZDO_STATE_CHANGE_IND command with 
a status of DEV_ZB_COORD, DEV_ROUTER, or DEV_END_DEVICE before performing 
any other API operations.
*/
#include <unistd.h>
#include <stdio.h>
#include "bytebuffer.h"
#include "cc2530zcomdef.h"

void _toolkit_printbytes(unsigned char* buf, unsigned int len){
	unsigned int i;
	for (i = 0; i < len; i++)
	{
		fprintf(stdout,"%02X", buf[i]);
	}
	fprintf(stdout,"\n");
}

void cc2530_startup(int fd){ 
	unsigned char buf[255] = {0};
	unsigned char value[128] = {0};
	unsigned char len = cc2530_encode_zb_write_configuration(buf, ZCD_NV_LOGICAL_TYPE, &value, 1); 
	_toolkit_printbytes(buf, len);
	if( write(fd, buf, len) <= 0){
		fprintf(stdout, "set logial error\n");
	};
	value[0] = 0xff;
	value[1] = 0xff;
	len = cc2530_encode_zb_write_configuration(buf, ZCD_NV_PANID, value, 2);
	_toolkit_printbytes(buf, len);
	if( write(fd, buf, len) <= 0){
		fprintf(stdout, "set pan id error\n");
	};
	unsigned char * tmp = value;
	bytebuffer_writedwordl(&tmp, 0x07FFF800);
	len = cc2530_encode_zb_write_configuration(buf, ZCD_NV_CHANLIST, value, 4); 

	_toolkit_printbytes(buf, len);
	if( write(fd, buf, len) <= 0){
		fprintf(stdout, "set pan id error\n");
	};
}

void cc2530_af_register(int fd, struct af_register * af_reg){ 
	unsigned char buf[255] = {0};
	unsigned char len = cc2530_encode_af_register(buf, af_reg);
	_toolkit_printbytes(buf, len);
	if( write(fd, buf, len) <= 0){
		fprintf(stdout, "set pan id error\n");
	};
}

void cc2530_zdo_startup_from_app(int fd, unsigned short startdelay){
	unsigned char buf[255] = {0};
	unsigned char len = cc2530_encode_zdostartup_from_app(buf, startdelay);
	_toolkit_printbytes(buf, len);
	if( write(fd, buf, len) <= 0){
		fprintf(stdout, "set pan id error\n");
	};

}

void cc2530_startrequst(int fd){
	unsigned char buf[255] = {0};
	unsigned char len = cc2530_encode_zb_start_request(buf);
	_toolkit_printbytes(buf, len);
	if( write(fd, buf, len) <= 0){
		fprintf(stdout, "set logial error\n");
	};
}

