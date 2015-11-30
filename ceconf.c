
static struct ceconf{
	char * listenport;
	char * serveraddr;
	char * serverport;
}s_conf = {
	"8989",
	"192.168.8.42",
	"8990",
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


