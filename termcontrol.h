struct termioconf{
	char * portname; 
	speed_t speed;
	tcflag_t parity;
};

int openterm(struct termioconf * conf);
