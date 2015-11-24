
#define CONNSERIALPORT 1
#define CONNSOCKET 2
struct connection;

void connection_init(struct connection * c, int fd, unsigned char type);
void connection_put(struct connection * c, unsigned char * buf, unsigned int buflen);
unsigned int connection_readbuf_getlen(struct connection *c);
int connection_readbuf_getahead(struct connection *c, unsigned char * buf, unsigned int buflen);
void connection_readbuf_pop(struct connection *c);

struct connection * freeconnlist_getconn(); 
void freeconnlist_add(struct connection * c); 

struct connection * connrbtree_getconn(int fd);
void connrbtree_insert(struct connection *c);
void connrbtree_del(struct connection * c); 
