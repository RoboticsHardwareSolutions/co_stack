# co_stack

CANopen stack in ANSI 

To customize co_stack, you can create a file co_config_custom.h and set custom settings. For example, do not use dynamic allocation or change the timeout for waiting for an SDO response. On UNIX, Apple or Win, you must define the CO_ARCH Cmake option. For Bare Metal projects, you should only have co_config_custom.h:
```c
#ifndef _CO_CONFIG_CUSTOM_H_
#define _CO_CONFIG_CUSTOM_H_

#define SDO_TIMEOUT_MS 100
#define NO_SDO_DYNAMIC_BUFFER_ALLOCATION

#endif /* _CO_CONFIG_CUSTOM_H_ */
```

All custom options (Examples):

```c
#define SDO_MAX_LENGTH_TRANSFER 32
#define SDO_BLOCK_SIZE 16
#define SDO_MAX_SIMULTANEOUS_TRANSFERS 4
#define NMT_MAX_NODE_ID 128
#define SDO_TIMEOUT_MS 3000 // timeout for waiting for an SDO response
#define MAX_NB_TIMER 32
#define EMCY_MAX_ERRORS 8
#define LSS_TIMEOUT_MS 1000
#define LSS_FS_TIMEOUT_MS 100
#define NO_SDO_DYNAMIC_BUFFER_ALLOCATION // If you don't want to use dynamic allocation
#define SDO_DYNAMIC_BUFFER_ALLOCATION_SIZE 256 // Change max buffer allocation

// If you need logs:  and define LOG(...) macros, e.g. printf(__VA_ARGS__)
#define CO_STACK_DEBUG_ENABLE               /* Enable debug mode */

/***** FOR STANDARD LIB *****/
#include "stdio.h"                          /* include you log output lib */
/* Define CO_LOG, CO_LOG_WAR and CO_LOG_ERR macros */
#define CO_LOG(...) printf(__VA_ARGS__)     /* e.g. printf(__VA_ARGS__) */
#define CO_LOG_WAR(...) CO_LOG(__VA_ARGS__)
#define CO_LOG_ERR(...) CO_LOG(__VA_ARGS__)

/***** OR FOR RLOG LIB *****/
#include "rlog.h"
#define CO_LOG(...) RLOG_INFO(__VA_ARGS__)
#define CO_LOG_WAR(...) RLOG_WARNING(__VA_ARGS__)
#define CO_LOG_ERR(...) RLOG_ERROR(__VA_ARGS__)
```
