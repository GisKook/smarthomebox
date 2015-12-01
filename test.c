#include<stdio.h>

#include "bytebuffer.h"

int main(){ 
	unsigned char test[10] = {0x08, 0x01, 0x02,0x03, 0x04, 0x05, 0x06,0x07,0x08,0x09};
	char * testa = (char *)test;
	unsigned char a = 0;
	unsigned int aa = 1;
	bytebuffer_readbyte(&testa, &a);
	unsigned short value = 0;
	bytebuffer_readword(&testa, &value);
	unsigned short value1 = *((unsigned short*)(test+1));
	fprintf(stdout, "%d %d\n", value, value1);

	return 0;
}
