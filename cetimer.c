
#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "connection.h"
#include "cetimer.h"

typedef void (*sighandler_t)(int);
struct cetimer{ 
	struct itimerval timer;
	sighandler_t handler;
	struct timeval tnexttime;
	struct timeval tinterval;
	int wfd;
};

static struct cetimer * s_timer = NULL;
//static struct cetimer * heart_timer

void reconnect(unsigned char conntype){
	struct list_head * head = connlist_get();
	int n = 0;
	if(!connlist_check(conntype)){
		for(;;){
			if(conntype == CONNSOCKETSERVER){
				n = write(s_timer->wfd,CERECONNSOCKET,1);
			}else if(conntype == CONNSERIALPORT){
				n = write(s_timer->wfd, CERECONNSERIAL, 1);
			}

			if(n < 0){ 
				fprintf(stdout, "%s\n", strerror(errno));
			}
			break;
		}
	}
}

void checkstatus(int i){ 
	struct list_head * head = connlist_get();
	if(!connlist_check(CONNSOCKETSERVER)){
		for(;;){
			int n = write(s_timer->wfd,CERECONNSOCKET,1);
			if(n < 0){ 
				fprintf(stdout, "%s\n", strerror(errno));
			}
			break;
		}
	}
	
	if(!connlist_check(CONNSERIALPORT)){
		for(;;){
			int n = write(s_timer->wfd,CERECONNSERIAL,1);
			if(n < 0){ 
				fprintf(stdout, "%s\n", strerror(errno));
			}
			break;
		}

	} 
	time_t t = time(NULL);
         if(t%60==0){
                  for(;;){
                          int n = write(s_timer->wfd,HEARTBEAT,1);
                          if(n < 0){
                                  fprintf(stdout, "%s\n", strerror(errno));
                          }
                          break;
                  }
  
          }



	connlist_checkstatus(t);
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
/*
struct cetimer * cetimer_create(unsigned int nextvalue, unsigned int interval, int wfd){
        if(heart_timer == NULL){
                struct cetimer * timer = (struct cetimer *)malloc(sizeof(struct cetimer));
                timer->tnexttime.tv_sec = nextvalue;
                timer->tinterval.tv_sec = interval;
                timer->timer.it_interval = timer->tinterval;
                timer->timer.it_value = timer->tnexttime;
                setitimer(ITIMER_VIRTUAL, &timer->timer, 0);
                timer->handler = ;
                timer->wfd = wfd;
  
                heart_timer = timer;
          }
  
          return heart_timer;
  }

void cetimer_start(struct cetimer * timer){
         signal(SIGVTALRM, timer->handler);
}
*/


void cetimer_start(struct cetimer * timer){
	signal(SIGALRM, timer->handler);
}


