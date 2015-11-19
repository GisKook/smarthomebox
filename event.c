
void event_accept(int fd){
	struct connection * c = freeconnlist_getconn();
	connection_init(c, fd, CONNSOCKET);
	connrbtree_insert(c);
}

void event_recvmsg(int fd, unsigned char * buf, int buflen){
	struct connection * c = connrbtree_getconn(fd);
	if(c){
		connection_put(c, buf, buflen);
	}
}

void event_close(int fd){
	struct connection * c = connrbtree_getconn(fd);
	if(c){ 
		connrbtree_del(c);
		freeconnlist_add(c);
	}
}
