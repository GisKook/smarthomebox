
#define ILLEGAL          0
#define HALFPACK         255

#define LOGINFEEDBACK    0x8001
#define HEARTFEEDBACK    0x8002
#define REQDEVICELIST    0x8003
#define REQOPERATE       0x8004
#define REQSETDEVICENAME 0x8008
#define REQDELDEVICE     0x800A

#define AFINCOMINGDATA   0x4481
#define ZDOSTATECHANGE   0x45C0
#define SYSRESETIND      0x4180
#define ZBWRITERSP       0x6605
#define AFREGISTERRSP    0x6400
#define ZDOSTARTUPRSP    0x6540
#define AFDATAREQRSP     0x6401

struct connection;
int protocol_check(struct connection * c, unsigned short * messageid);
int znpframe_check(struct connection  *c, unsigned short * messageid);
