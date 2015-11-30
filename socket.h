struct connection;
int make_socket_non_blocking (int sfd);
int create_and_bind (char *port);
int openclient(char *addr, char *port);
struct connection * connectserver();
struct connection * createpipe(int * wfd);
