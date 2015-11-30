
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "connection.h"


typedef void (*sighandler_t)(int);
struct cetimer{ 
	struct itimerval timer;
	sighandler_t handler;
	struct timeval tnexttime;
	struct timeval tinterval;
	int wfd;
};

static struct cetimer * s_timer = NULL;

void checkstatus(int i){ 
	struct list_head * head = connlist_get();
	if(connlist_checkserver()){
		fprintf(stdout, "have server %d\n", s_timer->wfd);
	}else{ 
		for(;;){
			int n = write(s_timer->wfd,"A",1);
			if(n < 0){ 
				fprintf(stdout, "%s\n", strerror(errno));
			}
			break;
		}
	}
}

struct cetimer * cetimer_create(unsigned int nextvalue, unsigned int interval, int wfd){
	if(s_timer == NULL){
		struct cetimer * timer = (struct cetimer *)malloc(sizeof(struct cetimer));
		timer->tnexttime.tv_sec = nextvalue;
		timer->tinterval.tv_sec = interval;
		timer->timer.it_interval = timer->tinterval;
		timer->timer.it_value = timer->tnexttime;
		setitimer(ITIMER_REAL, &timer->timer, 0);
		timer->handler = checkstatus;
		timer->wfd = wfd;

		s_timer = timer;
	}

	return s_timer;
}

void cetimer_start(struct cetimer * timer){
	signal(SIGALRM, timer->handler);
}


