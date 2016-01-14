#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <termios.h>
#include <string.h>
#include "cc2530.h"
#include "cluster.h"
#include "endianness.h"

struct termconf{
	char * portname; 
	int speed;
	int parity;
};

int set_term_attribs(int fd, struct termconf * conf);
void set_blocking (int fd, int should_block);

int openterm(struct termconf * conf){
	int fd = open(conf->portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0){
		fprintf(stdout, "error %d opending %s: %s\n", errno, conf->portname, strerror(errno));
		return -1;
	}
	set_term_attribs(fd, conf);
	set_blocking(fd, 1);

	return fd;
}

void
set_blocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof(struct termios) );
	if (tcgetattr (fd, &tty) != 0)
	{
		fprintf (stdout, "error from tggetattr %s \n", strerror(errno));
		return;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0){
		fprintf (stdout, "error setting term attributes %s\n", strerror(errno));
	}

}

int set_term_attribs(int fd, struct termconf * conf){
	struct termios tty;
	memset (&tty, 0, sizeof(struct termios) );
	if (tcgetattr (fd, &tty) != 0)
	{
		fprintf(stdout, "error from tcgetattr %s \n", strerror(errno));
		return -1;
	}

	cfsetospeed (&tty, conf->speed);
	cfsetispeed (&tty, conf->speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
	// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
	// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= conf->parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		fprintf(stdout, "error from tcsetattr %s \n", strerror(errno));
		return -1;
	}

	return 0;
}

void toolkit_printbytes(unsigned char* buf, unsigned int len){
	unsigned int i;
	for (i = 0; i < len; i++)
	{
		fprintf(stdout,"%02X", buf[i]);
	}
	fprintf(stdout,"\n");
}

int main(){
	struct termconf * conf = (struct termconf *)malloc(sizeof(struct termconf));
	memset(conf, 0, sizeof(struct termconf));
	conf->portname = strdup("/dev/ttyUSB0");
	//conf->portname = strdup("/dev/tnt0");
	conf->speed = 115200;
	conf->parity = 0;

	int fd = openterm(conf); 
	if(fd<0){
		return;
	}
	
	cc2530_startup(fd);
	struct af_register reg;
	reg.endpoint = 11;
	reg.appprofid = 0x0104;
	reg.appdeviceid = 12;
	reg.appdevversion = 13;
	reg.latencyreq = 0x00;
	reg.appnuminclusters = 10;
	reg.appinclusterlist[0] = (ONOFF);
	reg.appinclusterlist[1] = (ALARMS);
	reg.appinclusterlist[2] = (POLL_CONTROL);
	reg.appinclusterlist[3] = (DOOR_LOCK);
	reg.appinclusterlist[4] = (WINDOW_COVERING);
	reg.appinclusterlist[5] = (IAS_ZONE);
	reg.appinclusterlist[6] = (IAS_ACE);
	reg.appinclusterlist[7] = (IAS_WD);
	reg.appinclusterlist[8] = (BASIC);
	reg.appinclusterlist[9] = (IDENTIFY);
	reg.appnumoutclusters = 10;
	reg.appoutclusterlist[0] = (ONOFF);
	reg.appoutclusterlist[1] = (ALARMS);
	reg.appoutclusterlist[2] = (POLL_CONTROL);
	reg.appoutclusterlist[3] = (DOOR_LOCK);
	reg.appoutclusterlist[4] = (WINDOW_COVERING);
	reg.appoutclusterlist[5] = (IAS_ZONE);
	reg.appoutclusterlist[6] = (IAS_ACE);
	reg.appoutclusterlist[7] = (IAS_WD);
	reg.appoutclusterlist[8] = (BASIC);
	reg.appoutclusterlist[9] = (IDENTIFY);
	cc2530_af_register(fd, &reg);
	//cc2530_startrequst(fd);
	cc2530_zdo_startup_from_app(fd, 100);
	int count = 0;
	char buf[1024];
	for(;;){
		count = read (fd, buf, sizeof (buf));
		toolkit_printbytes(buf, count);
	}
}
