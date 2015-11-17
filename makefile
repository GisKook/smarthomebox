testterm:./testterm.c ./termcontrol.c
	- rm $@
	gcc -g -o $@ $^

