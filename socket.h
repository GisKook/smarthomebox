#ifndef __CE_SOCKET_H_H
#define __CE_SOCKET_H_H

struct connection;
int make_socket_non_blocking (int sfd);
int create_and_bind (char *port);
int openclient(char *addr, char *port);
struct connection * connectserver();
struct connection * createpipe(int * wfd);
int createpipe2(int *wfd);
int readnonblocking(int fd, char * buf, int buflen);
int sendnonblocking(int fd, char * buf, int buflen);

#endif
