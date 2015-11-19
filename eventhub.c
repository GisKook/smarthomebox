#include <string.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>

#include "socket.h"
#include "connection.h"

#define MAXEVENTS 64

struct eventhubconf{
	char port[8];
};

struct eventhub{
	struct eventhubconf * conf;
	int epollfd;
	int listenfd;
	struct epoll_event * events;
};

struct eventhub * eventhub_create(struct eventhubconf * conf){ 
	struct eventhub * hub = (struct eventhub *)malloc(sizeof(struct eventhub));
	memset(hub, 0, sizeof(struct eventhub));

	hub->events = (struct epoll_enent *)calloc(MAXEVENTS, sizeof(struct epoll_event));
	hub->conf = conf;


	hub->epollfd = epoll_crate1(0);
	if(hub->epollfd == -1){
		fprintf(stdout, "epoll create fail\n");
		free(hub->events);

		return NULL;
	}

	return hub;
}

void eventhub_register(struct eventhub * hub,int fd){
	struct epoll_event event; 
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	int ret = epoll_ctl(hub->epollfd, EPOLL_CTL_ADD, fd, &event);
	if( ret == -1 ){
		fprintf(stdout, "epoll add fail %s \n", strerror(errno));
	}
}

void eventhub_accept(int fd){
	struct connection * c = freeconnlist_getconn();
	connection_init(c, fd, CONNSOCKET);
	connrbtree_insert(c);
}

void eventhub_recvmsg(int fd, unsigned char * buf, int buflen){
	struct connection * c = connrbtree_getconn(fd);
	if(c){
		connection_put(c, buf, buflen);
	}
}

void eventhub_close(int fd){
	struct connection * c = connrbtree_getconn(fd);
	if(c){ 
		connrbtree_del(c);
		freeconnlist_add(c);
	}
}

void eventhub_start(struct eventhub * hub){
	hub->listenfd = create_and_bind(hub->conf->port);
	make_socket_non_blocking(hub->listenfd);
	eventhub_register(hub, hub->listenfd);
	int ret;

	struct epoll_event * events = hub->events;
	for(;;){
		int n, i;

		n = epoll_wait (hub->epollfd, events, MAXEVENTS, -1);
		for (i = 0; i < n; i++) {
			if ((events[i].events & EPOLLERR) ||
			    (events[i].events & EPOLLHUP) ||
			    (!(events[i].events & EPOLLIN))) {
				/* An error has occured on this fd, or the socket is not
				   ready for reading (why were we notified then?) */
				fprintf (stdout, "epoll error\n");
	 			close (events[i].data.fd);
				eventhub_close(events[i].data.fd);
				continue;
			} else if (hub->listenfd == events[i].data.fd) {
				/* We have a notification on the listening socket, which
				   means one or more incoming connections. */
				 for(;;) {
					struct sockaddr in_addr;
					socklen_t in_len;
					int infd;
					char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

					in_len = sizeof(in_addr);
					infd = accept (hub->listenfd, &in_addr, &in_len);
					if (infd == -1) {
						if ((errno == EAGAIN) ||
						    (errno == EWOULDBLOCK)) {
							/* We have processed all incoming
							   connections. */
							break;
						} else {
							fprintf(stdout,  "accept %s \n", strerror(errno));
							break;
						}
					}

					ret = getnameinfo (&in_addr, in_len,
							hbuf, sizeof (hbuf),
							sbuf, sizeof (sbuf),
							NI_NUMERICHOST | NI_NUMERICSERV);
					if (ret== 0) {
						fprintf(stdout, "Accepted connection on descriptor %d (host=%s, port=%s)\n", infd, hbuf, sbuf);
					}

					/* Make the incoming socket non-blocking and add it to the
					   list of fds to monitor. */
					ret = make_socket_non_blocking (infd);
					if (ret == -1)
						abort ();
					eventhub_accept(infd);
					eventhub_register(hub, infd);
				}
				continue;
			} else {
				/* We have data on the fd waiting to be read. Read and
				   display it. We must read whatever data is available
				   completely, as we are running in edge-triggered mode
				   and won't get a notification again for the same
				   data. */
				int done = 0;

				for(;;) {
					ssize_t count;
					char buf[512];

					count = read (events[i].data.fd, buf, sizeof buf);
					if (count == -1) {
						/* If errno == EAGAIN, that means we have read all
						   data. So go back to the main loop. */
						if (errno != EAGAIN)
						{
							perror ("read");
							done = 1;
						}
						break;
					} else if (count == 0) {
						/* End of file. The remote has closed the
						   connection. */
						done = 1;
						break;
					} 

					eventhub_recvmsg(events[i].data.fd, buf, count);

					/* Write the buffer to standard output */
					ret = write (1, buf, count);
					if (ret == -1)
					{
						perror ("write");
						abort ();
					}
				}

				if (done) {
					printf ("Closed connection on descriptor %d\n",
							events[i].data.fd);

					/* Closing the descriptor will make epoll remove it
					   from the set of descriptors which are monitored. */
					close (events[i].data.fd);
					eventhub_close(events[i].data.fd);
				}
			}
		}
	}

}
