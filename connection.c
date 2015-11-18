#include "list.h"
#include "kfifo.h"
#include "rbtree.h"

#define CONNSERIALPORT 1
#define CONNSOCKET 2
#define MAXBUFLEN 1024

struct connection{
	int fd;
	unsigned char type;
	struct kfifo * rawfifo;
};

struct connection * connection_create(){
	struct connection * conn = (struct connection*) malloc(sizeof(struct connection));
	memset(conn, 0, sizeof(struct connection));
	conn->rawfifo = kfifo_init(1024);

	return conn;
}

void connection_destroy(struct connection * c){ 
	kfifo_free(c->rawfifo);
	free(c);
}

void connection_init(struct connection * c, int fd, unsigned char type){
	c->fd = fd;
	c->type = type;
}

static LIST_HEAD(freeconnlisthead);
struct freeconnlist{
	struct list_head list;
	struct connection * connection;
};

struct connection * freeconnlist_getconn(){ 
	if(list_empty(freeconnlisthead)){
		return connection_create();
	}else{
		struct freeconnlist* list_first_entry(&freeconnlisthead, struct freeconnlist, list);
		struct connection * c = freeconnlist->connection;
		free(freeconnlist);
		return c;
	}
}

void freeconnlist_add(struct connection * c){ 
	struct freeconnlist * conn = (struct freeconnlist *)malloc(sizeof(struct freeconnlist));
	memset(conn, 0, sizeof(struct freeconnlist));
	conn->connection = c;
	list_add_tail(&conn->list, &freeconnlisthead);
}

static struct rb_root connrbtreeroot = RB_ROOT;

struct connrbtreenode{
	struct rb_node node;
	struct connection * connection;
};

struct connection * connrbtree_getconn(int fd){
	struct rb_root *root = &connrbtreeroot;
	struct rb_node *node = root->rb_node;

	struct connrbtreenode * connnode;
	while(node){
		connnode = rb_entry(node, struct connrbtreenode, node);
		if(fd > connnode->connection->fd){
			node = node->rb_right;
		}else if(fd < connnode->connection->fd){
			node = node->rb_left;
		}else{
			return node;
		}
	}

	return NULL;
}


struct connrbtreenode * _connrbtree_insert(struct connrbtreenode *c){
	struct rb_node **newnode = &connrbtreeroot.rb_node, *parent = NULL; 

	struct connrbtreenode * crbn;

	while(*newnode){ 
		crbn = rb_entry(*newnode, struct connrbtreenode, node);
		parent = *newnode;
		
		if(c->fd < crbn->connection->fd){
			newnode = &((*newnode)->rb_left);
		}else if(c->fd > crbn->connection->fd){
			newnode = &((*newnode)->rb_right);
		}else{
			return crbn;
		}
	}
	rb_link_node(&c->node, parent, newnode);

	return NULL;
}

void connrbtree_insert(struct connection *c){
	struct connrbtreenode * connrbtreenode = (struct  connrbtreenode *)malloc(sizeof(struct connrbtreenode));
	memset(connrbtreenode, 0, sizeof(struct connrbtreenode));
	connrbtreenode->connection = c;
	
	struct connrbtreenode * ret;

	if(( ret = _connrbtree_insert(connrbtreenode)))
		return;
	rb_insert_color(connrbtreenode->node, &connrbtreeroot);
}
