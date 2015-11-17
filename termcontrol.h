struct termconf{
	char * portname; 
	int speed;
	int parity;
};

int openterm(struct termconf * conf);
