
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "connection.h"

typedef void (*sighandler_t)(int);

void checkstatus(int i){ 
	struct list_head * head = connlist_get();
	if(list_empty(head)){
	fprintf(stdout, "empty \n");
	}else{
	
	fprintf(stdout, "check \n");
	}
}

struct cetimer{ 
	struct itimerval timer;
	sighandler_t handler;
	struct timeval tnexttime;
	struct timeval tinterval;
};

struct cetimer * cetimer_create(unsigned int nextvalue, unsigned int interval){
	struct cetimer * timer = (struct cetimer *)malloc(sizeof(struct cetimer));
	timer->tnexttime.tv_sec = nextvalue;
	timer->tinterval.tv_sec = interval;
	timer->timer.it_interval = timer->tinterval;
	timer->timer.it_value = timer->tnexttime;
	setitimer(ITIMER_REAL, &timer->timer, 0);
	timer->handler = checkstatus;

	return timer;
}

void cetimer_start(struct cetimer * timer){
	signal(SIGALRM, timer->handler);
}


