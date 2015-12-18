
#define CECHECK "C"
#define HEARTBEAT  "B"


struct cetimer; 
struct cetimer * cetimer_create(unsigned int nextvalue, unsigned int interval, int wfd);
void cetimer_start(struct cetimer * timer);


