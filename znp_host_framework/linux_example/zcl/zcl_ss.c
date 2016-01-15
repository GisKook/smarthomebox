#include "zcl_ss.h" 
#include "zcl.h"

int zclss_handlespecificcommands( struct zclincomingmsg * msg){

}

int zclss_handleincoming( struct zclincomingmsg * zclincomingmsg){ 
	int result = -1;
#if defined ( INTER_PAN )
	//todo
#endif
	if (zcl_ClusterCmd(zclincomingmsg->zclframehdr.control.type)){ 
		if(zclincomingmsg->zclframehdr.control.manuspecific == 0){ 

		}else{
		       	// We don't support any manufacturer specific command -- ignore it.  
			result = -1;
		}
	}else{
		result = -1;
	}

	return result;
}
