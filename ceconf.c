
static struct ceconf{
	char * listenport;
	char * serveraddr;
	char * serverport;

	char * serialport;
	int speed;
	int parity;

	int timeout;
}s_conf = {
	"8989",
	"192.168.8.42",
	"8990",
	"/dev/ttyO1",
	115200,
	0,
	60,
};

char * ceconf_getlistenport(){
	return s_conf.listenport;
}

char * ceconf_getserveraddr(){
	return s_conf.serveraddr;
}

char * ceconf_getserverport(){
	return s_conf.serverport;
}

char * ceconf_getserialport(){
	return s_conf.serialport;
}

int ceconf_getspeed(){
	return s_conf.speed;
}

int ceconf_getparity(){
	return s_conf.parity;
}

int ceconf_gettimeout(){
	return s_conf.timeout;
}
