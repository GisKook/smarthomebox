
#define CECHECK "C"
#define HEARTBEAT  "B"
#define CERECONN "R"


struct cetimer; 
struct cetimer * cetimer_create(unsigned int nextvalue, unsigned int interval, int wfd, int reconnwfd);
void cetimer_start(struct cetimer * timer);


