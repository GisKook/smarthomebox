
#include "list.h"

#define CONNSERIALPORT 1
#define CONNSOCKETCLIENT 2
#define CONNSOCKETSERVER 3
#define CONNSOCKETCMD 4

struct connection;

void connection_init(struct connection * c, int fd, unsigned char type);
void connection_put(struct connection * c, unsigned char * buf, unsigned int buflen);
unsigned int connection_get(struct connection* c, char * buffer, unsigned int msglen);
unsigned int connection_readbuf_getlen(struct connection *c);
int connection_readbuf_getahead(struct connection *c, unsigned char * buf, unsigned int buflen);
void connection_readbuf_pop(struct connection *c);
int connection_getfd(struct connection * c);
unsigned char connection_gettype(struct connection * c);

struct connection * freeconnlist_getconn(); 
void freeconnlist_add(struct connection * c); 

struct list_head * connlist_get();
int connlist_check(unsigned char conntype); 

struct connection * connrbtree_getconn(int fd);
void connrbtree_insert(struct connection *c);
void connrbtree_del(struct connection * c); 
