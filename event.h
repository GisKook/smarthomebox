struct eventhub;
void event_accept(int fd);
void event_recvmsg(struct eventhub *, int fd, unsigned char * buf, int buflen);
void event_close(int fd);
