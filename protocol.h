
#define ILLEGAL          0
#define HALFPACK         255

#define LOGINFEEDBACK    0x8001
#define HEARTFEEDBACK    0x8002
#define REQDEVICELIST    0x8003
#define REQOPERATE       0x8004
#define REQSETDEVICENAME 0x8008
#define REQDELDEVICE     0x800A

struct connection;
int protocol_check(struct connection * c, unsigned short * messageid);
