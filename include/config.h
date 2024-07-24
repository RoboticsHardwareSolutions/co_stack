#ifndef _CONFIG_H_
#define _CONFIG_H_

#define MAX_CAN_BUS_ID 1
#define SDO_MAX_LENGTH_TRANSFER 32
#define SDO_BLOCK_SIZE 16
#define SDO_MAX_SIMULTANEOUS_TRANSFERS 4
#define NMT_MAX_NODE_ID 128
#define SDO_TIMEOUT_MS 250
#define MAX_NB_TIMER 32
/* CANOPEN_BIG_ENDIAN is not defined */
/* US_TO_TIMEVAL_FACTOR is not defined */
/* TIMEVAL is not defined */
/* TIMEVAL_MAX is not defined */
/* RTCAN_SOCKET is not defined */
#define EMCY_MAX_ERRORS 8
#define LSS_TIMEOUT_MS 1000
#define LSS_FS_TIMEOUT_MS 100
#define SDO_DYNAMIC_BUFFER_ALLOCATION
#define SDO_DYNAMIC_BUFFER_ALLOCATION_SIZE 256
/* SDO_DYNAMIC_BUFFER_ALLOCATION is not defined */
/* SDO_DYNAMIC_BUFFER_ALLOCATION_SIZE is not defined */

#define REPEAT_SDO_MAX_SIMULTANEOUS_TRANSFERS_TIMES(repeat)\
repeat repeat repeat repeat
#define REPEAT_NMT_MAX_NODE_ID_TIMES(repeat)\
repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat
#define REPEAT_EMCY_MAX_ERRORS_TIMES(repeat)\
repeat repeat repeat repeat repeat repeat repeat repeat
#endif /* _CONFIG_H_ */
