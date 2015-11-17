#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "termcontrol.h"

int main(){
	struct termconf * conf = (struct termconf *)malloc(sizeof(struct termconf));
	memset(conf, 0, sizeof(struct termconf));
	conf->portname = strdup("/dev/tnt0");
	conf->speed = 115200;
	conf->parity = 0;

	int fd = openterm(conf); 
	for(;;){
		usleep(1000000);
		fprintf(stdout, ".\n");
		write(fd, "hello", 5);
	}
}
