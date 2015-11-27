#include "endianness.h"

static inline void bytebuffer_readbyte(const unsigned char** s, unsigned char* d){
	*d = (unsigned char)**s; (*s)++;
}

static inline void bytebuffer_readword(const unsigned char** s, unsigned short* d){
	*d = 0;
	((unsigned char *)d)[0] = ((unsigned char *)*s)[0];
	((unsigned char *)d)[1] = ((unsigned char *)*s)[1];
	if(!ISBIGENDIAN){
		*d = swap16(*d);
	}
	*s += 2;
}

static inline void bytebuffer_readdword(const unsigned char** s, unsigned int* d){
	*d = 0;
	((unsigned char *)d)[0] = ((unsigned char *)*s)[0];
	((unsigned char *)d)[1] = ((unsigned char *)*s)[1];
	((unsigned char *)d)[2] = ((unsigned char *)*s)[2];
	((unsigned char *)d)[3] = ((unsigned char *)*s)[3];
	if (!ISBIGENDIAN){
		*d = swap32(*d);
	}

	*s += 4;
}

static inline void bytebuffer_readquadword(const unsigned char ** s, unsigned long long *d){
	*d = 0;
	((unsigned char *)d)[0] = ((unsigned char *)*s)[0];
	((unsigned char *)d)[1] = ((unsigned char *)*s)[1];
	((unsigned char *)d)[2] = ((unsigned char *)*s)[2];
	((unsigned char *)d)[3] = ((unsigned char *)*s)[3];
	((unsigned char *)d)[4] = ((unsigned char *)*s)[4];
	((unsigned char *)d)[5] = ((unsigned char *)*s)[5];
	((unsigned char *)d)[6] = ((unsigned char *)*s)[6];
	((unsigned char *)d)[7] = ((unsigned char *)*s)[7];
	if(!ISBIGENDIAN){
		*d = swap64(*d);
	}

	*s+=8;
}

static inline void bytebuffer_readbytes( const unsigned char ** s, char * str, int len){
	int i;
	for(i = 0; i < len; i++) {
		str[i] = ((char *)*s)[i];
	}

	*s += len;
}

static inline unsigned short bytebuffer_getword(unsigned char * value){
	unsigned short result = 0;
	((unsigned char *)&result)[0] = value[0];
	((unsigned char *)&result)[1] = value[1];
	if (!ISBIGENDIAN){
		return swap16(result);
	}

	return result;
}

static inline unsigned long long bytebuffer_getquadword(unsigned char * value){
	unsigned long long result = 0;

	((unsigned char *)&result)[0] = value[0];
	((unsigned char *)&result)[1] = value[1];
	((unsigned char *)&result)[2] = value[2];
	((unsigned char *)&result)[3] = value[3];
	((unsigned char *)&result)[4] = value[4];
	((unsigned char *)&result)[5] = value[5];
	((unsigned char *)&result)[6] = value[6];
	((unsigned char *)&result)[7] = value[7];
	if (!ISBIGENDIAN){
		result = swap64(result);
	}

	return result;
}
