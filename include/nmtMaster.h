/** @defgroup networkmanagement Network Management
 *  @ingroup userapi
 */
/** @defgroup nmtmaster NMT Master
 *  @brief NMT master provides mechanisms that control and monitor the state of nodes and their behavior in the network.
 *  @ingroup networkmanagement
 */
 
#ifndef __nmtMaster_h__
#define __nmtMaster_h__

#include <applicfg.h>

//typedef struct struct_CO_Data CO_Data;

/** 
 * @ingroup nmtmaster
 * @brief Transmit a NMT message on the network to the slave whose nodeId is node ID.
 * 
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Id of the slave node
 * @param cs The order of state changement \n\n
 * 
 * Allowed states :
 *  - cs =  NMT_Start_Node               // Put the node in operational mode             
 *  - cs =	NMT_Stop_Node		         // Put the node in stopped mode
 *  - cs =	NMT_Enter_PreOperational     // Put the node in pre_operational mode  
 *  - cs =  NMT_Reset_Node		         // Put the node in initialization mode 
 *  - cs =  NMT_Reset_Comunication	     // Put the node in initialization mode
 *  
 * The mode is changed according to the slave state machine mode :
 *  - initialisation  ---> pre-operational (Automatic transition)
 *  - pre-operational <--> operational
 *  - pre-operational <--> stopped
 *  - pre-operational, operational, stopped -> initialisation
 * \n\n
 * @return errorcode
 *                   - 0 if the NMT message was send
 *                   - 1 if an error occurs 
 */
UNS8 masterSendNMTstateChange (CO_Data* d, UNS8 nodeId, UNS8 cs);

/**
 * @ingroup nmtmaster 
 * @brief Transmit a NodeGuard message on the network to the slave whose nodeId is node ID
 * 
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Id of the slave node
 * @return
 *         - 0 is returned if the NodeGuard message was send.
 *         - 1 is returned if an error occurs.
 */
UNS8 masterSendNMTnodeguard (CO_Data* d, UNS8 nodeId);

/** 
 * @ingroup nmtmaster
 * @brief Ask the state of the slave node whose nodeId is node Id.
 * 
 * To ask states of all nodes on the network (NMT broadcast), nodeId must be equal to 0
 * @param *d Pointer to a CAN object data structure
 * @param nodeId Id of the slave node
 */
UNS8 masterRequestNodeState (CO_Data* d, UNS8 nodeId);


#endif /* __nmtMaster_h__ */
