testterm:./testterm.c ./termcontrol.c
	- rm $@
	gcc -g -o $@ $^

testevent:./main.c eventhub.c event.c connection.c kfifo.c rbtree.c socket.c protocol.c toolkit.c cetimer.c ceconf.c termcontrol.c parseserver.c gateway.c bussinessdata.c
	- rm $@
	gcc -g -o $@ $^

