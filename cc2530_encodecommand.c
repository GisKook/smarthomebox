#include "bytebuffer.h"
#include "cc2530.h"

unsigned char calcFCS(unsigned char *pMsg, unsigned char len)
{
	unsigned char result = 0;
	while (len--)
	{
		result ^= *pMsg++;
	}
	return result;
}
/*
 * ZB_WRITE_CONFIGURATION
 * 4.2.2.1 Description
 * This command is used to write a configuration parameter to the CC2530-ZNP device.
 * 4.2.2.2 Usage
 *
 * SREQ:
 * 1                  * 1           * 1           * 1        * 1   * 1-128
 * Length = 0x03-0x83 * Cmd0 = 0x26 * Cmd1 = 0x05 * ConfigId * Len * Value
 *
 * ConfigId – 1 byte – The identifier for the configuration property
 * Len – 1 byte – Specifies the size of the Value buffer in bytes.
 * Value – 1-128 bytes – The buffer containing the new value of the configuration property
 *
 * SRSP:
 * 1             * 1           * 1           * 1
 * Length = 0x01 * Cmd0 = 0x66 * Cmd1 = 0x05 * Status
 * Status – 1 byte – See 4.7 for a listing of the status values.
 */
unsigned char cc2530_encode_zb_write_configuration(unsigned char * dstbuf, unsigned char configid, unsigned char * value, unsigned char valuelen){ 
	unsigned char * p = dstbuf;
	bytebuffer_writebyte(&p, 0xFE);
	bytebuffer_writebyte(&p, valuelen + 1 + 1); // 1 stands for configid the other 1 for len
	bytebuffer_writebyte(&p, 0x26);
	bytebuffer_writebyte(&p, 0x05); 
	bytebuffer_writebyte(&p, configid);
	bytebuffer_writebyte(&p, valuelen);
	bytebuffer_writebytes(&p, value, valuelen); 
	unsigned char sum = calcFCS(dstbuf+1, p-dstbuf-1);
	bytebuffer_writebyte(&p, sum);

	return p-dstbuf;
}

/*
 * 4.3.2 ZB_START_REQUEST
 * 4.3.2.1 Description
 * This command starts the ZigBee stack in the CC2530-ZNP device. When the ZigBee stack starts, the device reads the programmed configuration parameters and operates accordingly. After the start request process completes, the device is ready to send, receive, and route network traffic.
 * NOTE: Calling this function without following the procedure outlined in section 3.2 could result in unexpected behaviour and not result in the ZNP issuing a ZB_START_CONFIRM.
 *
 * 4.3.2.2 Usage
 * SREQ:
 * 1             * 1           * 1
 * Length = 0x00 * Cmd0 = 0x26 * Cmd1 = 0x00
 * SRSP:
 * 1             * 1           * 1
 * Length = 0x00 * Cmd0 = 0x66 * Cmd1 = 0x00
 */
unsigned char cc2530_encode_zb_start_request(unsigned char * dstbuf){
	unsigned char * p = dstbuf;
	bytebuffer_writebyte(&p, 0xFE);
	bytebuffer_writebyte(&p, 0x00);
	bytebuffer_writebyte(&p, 0x26);
	bytebuffer_writebyte(&p, 0x00);
	unsigned char sum = calcFCS(dstbuf+1, p-dstbuf-1);
	bytebuffer_writebyte(&p, sum);

	return p - dstbuf;
}

/*
 * 4.4.1 AF_REGISTER
 * 4.4.1.1 Description
 * This command enables the host processor to register an application’s endpoint description (and its simple descriptor). Multiple endpoints may be registered with the AF by making multiple calls to AF_REGISTER. This could be useful in the case where the device needs to support multiple application profiles, where each AF_REGISTER call would register a unique endpoint description per application profile.
 * 4.4.1.2 Usage
 *
 * SREQ:
 * 1                  * 1           * 1           * 1        * 2         * 2
 * Length = 0x09-0x49 * Cmd0 = 0x24 * Cmd1 = 0x00 * EndPoint * AppProfId * AppDeviceId
 * 1         * 1          * 1                * 0-32             * 1                 * 0-32
 * AppDevVer * LatencyReq * AppNumInClusters * AppInClusterList * AppNumOutClusters * AppOutClusterList
 *
 * Attributes:
 * Attribute         * Length (byte) * Description
 * EndPoint          * 1             * Specifies the endpoint of this simple descriptor.
 * AppProfId         * 2             * Specifies the profile id of the application
 * AppDeviceId       * 2             * Specifies the device description id for this endpoint
 * AddDevVer         * 1             * Specifies the device version number
 * LatencyReq        * 1             * Specifies latency. For ZigBee the only applicable value is 0x00.
 *                                     0x00-No latency
 *                                     0x01-fast beacons
 *                                     0x02-slow beacons
 * AppNumInClusters  * 1             * the number of Input cluster Ids following in the AppInClusterList
 * AppInClusterList  * 32            * Specifies the list of Input Cluster Ids ( 2bytes each )
 * AppNumOutClusters * 1             * Specifies the number of Output cluster Ids following in the AppOutClusterList
 * AppOutClusterList * 32            * Specifies the list of Output Cluster Ids ( 2bytes each )
 *
 * SRSP:
 * 1             * 1           * 1           * 1
 * Length = 0x01 * Cmd0 = 0x64 * Cmd1 = 0x00 * Status
 * Attributes:
 * Attribute * Length (byte) * Description
 * Status    * 1             * Status is either Success (0) or Failure (1).
 */
unsigned char cc2530_encode_af_register(unsigned char * dstbuf, struct af_register * reg){
	unsigned char *p = dstbuf;
	bytebuffer_writebyte(&p, 0xFE);
	bytebuffer_writebyte(&p, 0x00);
	bytebuffer_writebyte(&p, 0x24);
	bytebuffer_writebyte(&p, 0x00);
	unsigned char * tmp = p;
	bytebuffer_writebyte(&p, reg->endpoint);
	bytebuffer_writeword(&p, reg->appprofid);
	bytebuffer_writeword(&p, reg->appdeviceid);
	bytebuffer_writebyte(&p, reg->appdevversion);
	bytebuffer_writebyte(&p, reg->latencyreq);
	bytebuffer_writebyte(&p, reg->appnuminclusters);
	bytebuffer_writebytes(&p, (unsigned char *)reg->appinclusterlist, reg->appnuminclusters * 2); 
	bytebuffer_writebyte(&p, reg->appnumoutclusters);
	bytebuffer_writebytes(&p, (unsigned char *)reg->appoutclusterlist, reg->appnumoutclusters * 2);
	unsigned char * count = dstbuf + 1;
	bytebuffer_writebyte(&count, p - tmp);

	unsigned char sum = calcFCS(dstbuf+1, p-dstbuf-1);
	bytebuffer_writebyte(&p, sum);

	return p - dstbuf; 
}

/*
 * 4.5.24 ZDO_STARTUP_FROM_APP
 * 4.5.24.1 Description
 * This command starts the device in the network.
 *
 * 4.5.24.2 Usage
 * SREQ:
 * 1             * 1           * 1           * 2
 * Length = 0x01 * Cmd0 = 0x25 * Cmd1 = 0x40 * StartDelay
 *
 * Attributes:
 * Attribute  * Length (byte) * Description
 * StartDelay * 2             * Specifies the time delay before the device starts in milliseconds.
 **
 * SRSP:
 * 1             * 1           * 1           * 1
 * Length = 0x01 * Cmd0 = 0x65 * Cmd1 = 0x40 * Status
 * Attributes:
 * Attribute * Length (byte) * Description
 * Status    * 1             * 0x00 – Restored network state
 *                             0x01 – New network state
 *                             0x02 – Leave and not Started
 */
unsigned char cc2530_encode_zdostartup_from_app(unsigned char * dstbuf,unsigned short startdelay){
	unsigned char *p = dstbuf;
	bytebuffer_writebyte(&p, 0xFE);
	bytebuffer_writebyte(&p, 0x01);
	bytebuffer_writebyte(&p, 0x25);
	bytebuffer_writebyte(&p, 0x40);
	bytebuffer_writewordl(&p, startdelay);

	unsigned char sum = calcFCS(dstbuf+1, p-dstbuf-1);
	bytebuffer_writebyte(&p, sum);

	return p - dstbuf; 
}
