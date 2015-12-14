#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "connection.h"
#include "ceconf.h"

int
make_socket_non_blocking (int sfd) {
	int flags, s;

	flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1) {
		perror ("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
	if (s == -1) {
		perror ("fcntl");
		return -1;
	}

	return 0;
}

int
create_and_bind (char *port) {
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC;     /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE;     /* All interfaces */

	s = getaddrinfo (NULL, port, &hints, &result);
	if (s != 0) {
		fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
		return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0) {
			/* We managed to bind successfully! */
			break;
		}

		close (sfd);
	}

	if (rp == NULL) {
		fprintf (stderr, "Could not bind\n");
		return -1;
	}

	freeaddrinfo (result);

	return sfd;
}

int openclient(char *addr, char *port) {
	typedef struct sockaddr SA;
	int sockfd, fd, nbyte;
	struct sockaddr_in servaddr;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stdout,"fail to connection to server\n");
		return -1;
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_port = htons(atoi(port));
	servaddr.sin_addr.s_addr = inet_addr(addr);

	if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
		fprintf(stdout, "fail to connect\n");
		close(sockfd);

		return -1;
	}

	make_socket_non_blocking(sockfd);

	return(sockfd);
}

struct connection * connectserver(){
	int fd = openclient(ceconf_getserveraddr(), ceconf_getserverport());
	struct connection * serverconn = NULL;
	if(fd != -1){
		serverconn = freeconnlist_getconn();
		connection_init(serverconn, fd, CONNSOCKETSERVER);
		connrbtree_insert(serverconn);
	}

	return serverconn;
}


struct connection * createpipe(int * wfd){
	int fdsig[2];
	if(pipe2(fdsig,O_CLOEXEC | O_NONBLOCK) == -1){
		fprintf(stderr,"create pipe error.%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		*wfd = -1;

		return NULL;
	}

	*wfd = fdsig[1]; 
	make_socket_non_blocking(fdsig[0]);

	struct connection * conn = freeconnlist_getconn();
	connection_init(conn, fdsig[0], CONNSOCKETCMD);
	connrbtree_insert(conn);

	return conn;
}

int sendnonblocking(int fd, char * buf, int buflen){ 
	int n;
	for(;;){
		n = write(fd, buf, buflen);
		if (n == -1) {
			if(errno == EINTR) continue;
			else if(errno == EAGAIN) break;
			else {
				assert(0);
				break;
			}
		} else if (n != buflen) {
			assert(0);
			break;
		} else { break; }
	}

	return n;
}

int readnonblocking(int fd, char * buf, int buflen){
	int n;
	for(;;){
		n = read(fd, buf, buflen);
		if (n == -1) {
			if(errno == EINTR) continue;
			else if(errno == EAGAIN) break;
			else {
				assert(0);
				break;
			}
		} else if ((size_t)n != buflen) {
			assert(0);
			break;
		} else { break; }
	}

	return n;
}

