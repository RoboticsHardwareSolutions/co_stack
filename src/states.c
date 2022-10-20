/*!
** @file   states.c
** @author Edouard TISSERANT and Francis DUPIN
** @date   Tue Jun  5 09:32:32 2007
**
** @brief
**
**
*/

#include "states.h"
#include "data.h"
#include "sysdep.h"
#include "objaccessinternal.h"

/** Prototypes for internals functions */
/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param newCommunicationState                                                                    
**/
void switchCommunicationState(CO_Data* d,
	s_state_communication *newCommunicationState);

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
**                                                                                                 
** @return                                                                                         
**/
e_nodeState getState(CO_Data* d)
{
  return d->nodeState;
}


void tmp_pdo(CO_Data* d){
    UNS16 offset = d->firstIndex->PDO_RCV_MAP;
    UNS16 lastIndex = d->lastIndex->PDO_RCV_MAP;
    UNS8 RPDOn = 0;
    while(offset <= lastIndex) {
        int numMap = 0;
        while (numMap < READ_UNS8(d->objdict, offset, 0)){

            UNS32 mappingParameter = READ_UNS32(d->objdict, offset, numMap + 1);
            UNS16 index = (UNS16) (mappingParameter >> 16);
            UNS32 Size = (UNS32) (mappingParameter & (UNS32) 0x000000FF);     /* Size in bits */
            UNS32 ByteSize = 1 + ((Size - 1) >> 3);
            UNS8 subIndex = (UNS8) (((mappingParameter) >> (UNS8) 8) & (UNS32) 0x000000FF);

            UNS8 dataType;            /* Unused */
            UNS8 tmp[] = { 0, 0, 0, 0, 0, 0, 0, 0 };  /* temporary space to hold bits */

            MSG_WAR (0x300F, "  got mapping parameter : ", mappingParameter);
            //MSG_WAR (0x3050, "    at index : ", ObjDict_Data.objdict[ObjDict_Data.firstIndex->PDO_TRS_MAP].index);
            MSG_WAR (0x3051, "    sub-index : ", numMap + 1);
            int data = 0;
            getODentry (d, index, subIndex, tmp, &ByteSize, &dataType, 0);
            switch(dataType) {
                case int8:
                    data = tmp[0];
                    break;
                case int16:
                    data = tmp[0]|(tmp[1]<<8);
                    break;
                case int32:
                    // data = tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24);
					data = tmp[3]|(tmp[2]<<8)|(tmp[1]<<16)|(tmp[0]<<24);
                    break;
                case uint8:
                    data = tmp[0];
                    break;
                case uint16:
                    // data = tmp[0]|(tmp[1]<<8);
					data = tmp[1]|(tmp[0]<<8);
                    break;
                case uint32:
                    // data = tmp[0]|(tmp[1]<<8)|(tmp[2]<<16)|(tmp[3]<<24);
					data = tmp[3]|(tmp[2]<<8)|(tmp[1]<<16)|(tmp[0]<<24);
                    break;
            }
            numMap++;
        }
        RPDOn++;
        offset++;
    }
}


/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param m                                                                                        
**/
void canDispatch(CO_Data* d, Message *m)
{
	UNS16 cob_id = UNS16_LE(m->cob_id);
	 switch(cob_id >> 7)
	{
		case SYNC:		/* can be a SYNC or a EMCY message */
			if(cob_id == 0x080)	/* SYNC */
			{
				if(d->CurrentCommunicationState.csSYNC)
					proceedSYNC(d);
			} else 		/* EMCY */
				if(d->CurrentCommunicationState.csEmergency)
					proceedEMCY(d, m);
			break;
		case TIME_STAMP:
		case PDO1tx:
		case PDO1rx:
		case PDO2tx:
		case PDO2rx:
		case PDO3tx:
		case PDO3rx:
		case PDO4tx:
		case PDO4rx:
			if (d->CurrentCommunicationState.csPDO) {
                proceedPDO(d, m);
                // tmp_pdo(d);
            }
			break;
		case SDOtx:
		case SDOrx:
			if (d->CurrentCommunicationState.csSDO)
				proceedSDO(d, m);
			break;
		case NODE_GUARD:
			if (d->CurrentCommunicationState.csLifeGuard)
				proceedNODE_GUARD(d, m);
			break;
		case NMT:
			if (*(d->iam_a_slave))
			{
				proceedNMTstateChange(d, m);
			}
            break;
#ifdef CO_ENABLE_LSS
		case LSS:
			if (!d->CurrentCommunicationState.csLSS)break;
			if ((*(d->iam_a_slave)) && cob_id==MLSS_ADRESS)
			{
				proceedLSS_Slave(d,m);
			}
			else if(!(*(d->iam_a_slave)) && cob_id==SLSS_ADRESS)
			{
				proceedLSS_Master(d,m);
			}
			break;
#endif
	}
}

#define StartOrStop(CommType, FuncStart, FuncStop) \
	if(newCommunicationState->CommType && d->CurrentCommunicationState.CommType == 0){\
		MSG_WAR(0x9999,#FuncStart, 9999);\
		d->CurrentCommunicationState.CommType = 1;\
		FuncStart;\
	}else if(!newCommunicationState->CommType && d->CurrentCommunicationState.CommType == 1){\
		MSG_WAR(0x9999,#FuncStop, 9999);\
		d->CurrentCommunicationState.CommType = 0;\
		FuncStop;\
	}
#define None

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param newCommunicationState                                                                    
**/
void switchCommunicationState(CO_Data* d, s_state_communication *newCommunicationState)
{
#ifdef CO_ENABLE_LSS
	StartOrStop(csLSS,	startLSS(d),	stopLSS(d))
#endif
	StartOrStop(csSDO,	None,		resetSDO(d))
	StartOrStop(csSYNC,	startSYNC(d),		stopSYNC(d))
	StartOrStop(csLifeGuard,	lifeGuardInit(d),	lifeGuardStop(d))
	StartOrStop(csEmergency,	emergencyInit(d),	emergencyStop(d))
	StartOrStop(csPDO,	PDOInit(d),	PDOStop(d))
	StartOrStop(csBoot_Up,	None,	slaveSendBootUp(d))
}

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param newState                                                                                 
**                                                                                                 
** @return                                                                                         
**/
UNS8 setState(CO_Data* d, e_nodeState newState)
{
	if(newState != d->nodeState){
		switch( newState ){
			case Initialisation:
			{
				s_state_communication newCommunicationState = {1, 0, 0, 0, 0, 0, 0};
				d->nodeState = Initialisation;
				switchCommunicationState(d, &newCommunicationState);
				/* call user app init callback now. */
				/* d->initialisation MUST NOT CALL SetState */
				(*d->initialisation)(d);
			}

			/* Automatic transition - No break statement ! */
			/* Transition from Initialisation to Pre_operational */
			/* is automatic as defined in DS301. */
			/* App don't have to call SetState(d, Pre_operational) */

			case Pre_operational:
			{

				s_state_communication newCommunicationState = {0, 1, 1, 1, 1, 0, 1};
				d->nodeState = Pre_operational;
				switchCommunicationState(d, &newCommunicationState);
                (*d->preOperational)(d);
			}
			break;

			case Operational:
			if(d->nodeState == Initialisation) return 0xFF;
			{
				s_state_communication newCommunicationState = {0, 1, 1, 1, 1, 1, 0};
				d->nodeState = Operational;
				newState = Operational;
				switchCommunicationState(d, &newCommunicationState);
				(*d->operational)(d);
			}
			break;

			case Stopped:
			if(d->nodeState == Initialisation) return 0xFF;
			{
				s_state_communication newCommunicationState = {0, 0, 0, 0, 1, 0, 1};
				d->nodeState = Stopped;
				newState = Stopped;
				switchCommunicationState(d, &newCommunicationState);
				(*d->stopped)(d);
			}
			break;
			default:
				return 0xFF;

		}/* end switch case */

	}
	/* d->nodeState contains the final state */
	/* may not be the requested state */
    return d->nodeState;
}

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
**                                                                                                 
** @return                                                                                         
**/
UNS8 getNodeId(CO_Data* d)
{
  return *d->bDeviceNodeId;
}

/*!                                                                                                
**                                                                                                 
**                                                                                                 
** @param d                                                                                        
** @param nodeId                                                                                   
**/
void setNodeId(CO_Data* d, UNS8 nodeId)
{
  LOG("setNodeId");
  UNS16 offset = d->firstIndex->SDO_SVR;

#ifdef CO_ENABLE_LSS
  d->lss_transfer.nodeID=nodeId;
  if(nodeId==0xFF){
  	*d->bDeviceNodeId = nodeId;
  	return;
  }
  else
#endif
  if(!(nodeId>0 && nodeId<=127)){
	  MSG_WAR(0x2D01, "Invalid NodeID",nodeId);
	  return;
  }
  LOG("INIT SDO, d->firstIndex->SDO_SVR %u, *d->bDeviceNodeId %08x", d->firstIndex->SDO_SVR, *d->bDeviceNodeId);
  if(offset){
    /* Adjust COB-ID Client->Server (rx) only id already set to default value or id not valid (id==0xFF)*/
    if((READ_UNS32(d->objdict, offset, 1) == ((UNS32)0x600) + *d->bDeviceNodeId)||(*d->bDeviceNodeId==0xFF)){
      /* cob_id_client = 0x600 + nodeId; */
	  LOG("Adjust COB-ID Client->Server (rx) %08x", 0x600 + nodeId);
      WRITE_UNS32(d->objdict, offset, 1, 0x600 + nodeId);
    }
    /* Adjust COB-ID Server -> Client (tx) only id already set to default value or id not valid (id==0xFF)*/
    if((READ_UNS32(d->objdict, offset, 2) == ((UNS32)0x580) + *d->bDeviceNodeId)||(*d->bDeviceNodeId==0xFF)){
      /* cob_id_server = 0x580 + nodeId; */
	  LOG("Adjust COB-ID Server -> Client (tx) %08x", 0x580 + nodeId);
      WRITE_UNS32(d->objdict, offset, 2, 0x580 + nodeId);
    }
  }

  /*
   	Initialize the server(s) SDO parameters
  	Remember that only one SDO server is allowed, defined at index 0x1200

  	Initialize the client(s) SDO parameters
  	Nothing to initialize (no default values required by the DS 401)
  	Initialize the receive PDO communication parameters. Only for 0x1400 to 0x1403
  */
  {
	LOG("INIT RX PDO, d->firstIndex->PDO_RCV %u", d->firstIndex->PDO_RCV);
    UNS8 i = 0;
    offset = d->firstIndex->PDO_RCV;
    UNS16 lastIndex = d->lastIndex->PDO_RCV;
    UNS32 cobID[] = {0x200, 0x300, 0x400, 0x500};
    UNS32 canID;
    UNS32 otherBits;
    if( offset ) {
		while( (offset <= lastIndex) && (i < 4)) {
			canID = READ_UNS32(d->objdict, offset, 1) & 0x1fffffff;
			otherBits = READ_UNS32(d->objdict, offset, 1) & ~0x1fffffff;
			LOG("i=%u, canID=%08x, otherBits=%08x", i , canID, otherBits);
			if((canID == cobID[i] + *d->bDeviceNodeId)||(*d->bDeviceNodeId==0xFF)){
				WRITE_UNS32(d->objdict, offset, 1, (cobID[i] + nodeId) | otherBits);
				LOG("newID%08x", READ_UNS32(d->objdict, offset, 1));
			}
			i ++;
			offset ++;
	  	}
    }
  }
  /* ** Initialize the transmit PDO communication parameters. Only for 0x1800 to 0x1803 */
  {
 	LOG("INIT TX PDO");
    UNS8 i = 0;
    offset = d->firstIndex->PDO_TRS;
    UNS16 lastIndex = d->lastIndex->PDO_TRS;
    UNS32 cobID[] = {0x180, 0x280, 0x380, 0x480};
    UNS32 canID;
    UNS32 otherBits;
    i = 0;
    if( offset ) while ((offset <= lastIndex) && (i < 4)) {
      canID = READ_UNS32(d->objdict, offset, 1) & 0x1fffffff;
      otherBits = READ_UNS32(d->objdict, offset, 1) & ~0x1fffffff;
	  LOG("i=%u, canID=%08x, otherBits=%08x", i , canID, otherBits);
      if((canID == cobID[i] + *d->bDeviceNodeId)||(*d->bDeviceNodeId==0xFF))
	      WRITE_UNS32(d->objdict, offset, 1, (cobID[i] + nodeId) | otherBits);
		  LOG("newID%08x", READ_UNS32(d->objdict, offset, 1));
      i ++;
      offset ++;
    }
  }

	LOG("INIT EMCY *d->bDeviceNodeId %08x", *d->bDeviceNodeId );
  /* Update EMCY COB-ID if already set to default*/
  if((*d->error_cobid == *d->bDeviceNodeId + (UNS32)0x80)||(*d->bDeviceNodeId==0xFF)){
    *d->error_cobid = nodeId + 0x80;
	LOG("NEW error_cobid=%08x", *d->error_cobid);
  }
  
  /* bDeviceNodeId is defined in the object dictionary. */
  *d->bDeviceNodeId = nodeId;
  LOG("NEW bDeviceNodeId=%08x", *d->bDeviceNodeId);
}

void _initialisation(CO_Data* d){(void)d;}
void _preOperational(CO_Data* d){
    if (!(*(d->iam_a_slave)))
    {
        masterSendNMTstateChange (d, 0, NMT_Reset_Node);
    }
}
void _operational(CO_Data* d){(void)d;}
void _stopped(CO_Data* d){(void)d;}
