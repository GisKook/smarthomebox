#ifndef __CC2530_ENCODICOMMAND_H_H
#define __CC2530_ENCODICOMMAND_H_H

struct af_register;
unsigned char cc2530_encode_zb_write_configuration(unsigned char * dstbuf, unsigned char configid, unsigned char * value, unsigned char valuelen);
unsigned char cc2530_encode_zb_start_request(unsigned char * dstbuf);
unsigned char cc2530_encode_af_register(unsigned char * dstbuf, struct af_register * reg);
unsigned char cc2530_encode_zdostartup_from_app(unsigned char * dstbuf,unsigned short startdelay);

#endif
