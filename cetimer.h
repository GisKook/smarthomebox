
struct cetimer; 
struct cetimer * cetimer_create(unsigned int nextvalue, unsigned int interval);
void cetimer_start(struct cetimer * timer);


