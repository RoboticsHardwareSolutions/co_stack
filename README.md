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

// If you need logs:
#define CO_STACK_ENABLE_DEBUG

// If you need custom log (CO_STACK_ENABLE_DEBUG must be defined):
#define CO_STACK_ENABLE_CUSTOM_LOG
/***** EXAMPLE FOR RLOG LIB *****/
#include "rlog.h"
#define CO_LOG(...) RLOG_INFO(__VA_ARGS__)
#define CO_LOG_WAR(...) RLOG_WARNING(__VA_ARGS__)
#define CO_LOG_ERR(...) RLOG_ERROR(__VA_ARGS__)
```
