
#define CONNSERIALPORT 1
#define CONNSOCKET 2
struct connection;

void connection_init(struct connection * c, int fd, unsigned char type);
void connection_put(struct connection * c, unsigned char * buf, unsigned int buflen);

struct connection * freeconnlist_getconn(); 
void freeconnlist_add(struct connection * c); 

struct connection * connrbtree_getconn(int fd);
void connrbtree_insert(struct connection *c);
void connrbtree_del(struct connection * c); 
