#include <string.h>
#include <stdio.h>
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
	struct list_head list;
	struct rb_node node;
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

struct connection * freeconnlist_getconn(){ 
	if(list_empty(&freeconnlisthead)){
		return connection_create();
	}else{
		struct connection *c = list_first_entry(&freeconnlisthead, struct connection, list);
		list_del(&c->list);
		return c;
	}
}

void freeconnlist_add(struct connection * c){ 
	list_add_tail(&c->list, &freeconnlisthead);
}

static struct rb_root connrbtreeroot = RB_ROOT;

struct connection * connrbtree_getconn(int fd){
	struct rb_root *root = &connrbtreeroot;
	struct rb_node *node = root->rb_node;

	struct connection * c;
	while(node){
		c  = rb_entry(node, struct connection, node);
		if(fd > c->fd){
			node = node->rb_right;
		}else if(fd < c->fd){
			node = node->rb_left;
		}else{
			return c;
		}
	}

	return NULL;
}


struct connection * _connrbtree_insert(struct connection *c){
	struct rb_node **newnode = &connrbtreeroot.rb_node, *parent = NULL; 

	struct connection * conn;

	while(*newnode){ 
		conn  = rb_entry(*newnode, struct connection, node);
		parent = *newnode;
		
		if(c->fd < conn->fd){
			newnode = &((*newnode)->rb_left);
		}else if(c->fd > conn->fd){
			newnode = &((*newnode)->rb_right);
		}else{
			fprintf(stdout, "rbtree alread has the fd \n");
			return conn;
		}
	}
	rb_link_node(&c->node, parent, newnode);

	return NULL;
}

void connrbtree_insert(struct connection *c){
	struct connection * ret;

	if(( ret = _connrbtree_insert(c)))
		return;
	rb_insert_color(&c->node, &connrbtreeroot);
}
