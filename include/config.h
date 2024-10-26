#ifndef _CONFIG_H_
#    define _CONFIG_H_

#    define CO_ARCH_CUSTOM 0        // User creates its own co_config_custom.h
#    define CO_ARCH_UNIX 1          // Linux, BSD, Mac, ...
#    define CO_ARCH_WIN32 2         // Windows
#    define CO_ARCH_ESP32 3         // ESP32
#    define CO_ARCH_ESP8266 4       // ESP8266
#    define CO_ARCH_FREERTOS 5      // FreeRTOS
#    define CO_ARCH_AZURERTOS 6     // MS Azure RTOS
#    define CO_ARCH_ZEPHYR 7        // Zephyr RTOS
#    define CO_ARCH_NEWLIB 8        // Bare metal ARM
#    define CO_ARCH_CMSIS_RTOS1 9   // CMSIS-RTOS API v1 (Keil RTX)
#    define CO_ARCH_TIRTOS 10       // Texas Semi TI-RTOS
#    define CO_ARCH_RP2040 11       // Raspberry Pi RP2040
#    define CO_ARCH_ARMCC 12        // Keil MDK-Core with Configuration Wizard
#    define CO_ARCH_CMSIS_RTOS2 13  // CMSIS-RTOS API v2 (Keil RTX5, FreeRTOS)
#    define CO_ARCH_RTTHREAD 14     // RT-Thread RTOS

#    if !defined(CO_ARCH)
#        if defined(__unix__) || defined(__APPLE__)
#            define CO_ARCH CO_ARCH_UNIX
#        elif defined(_WIN32)
#            define CO_ARCH CO_ARCH_WIN32
#        elif defined(__ZEPHYR__)
#            define CO_ARCH CO_ARCH_ZEPHYR
#        elif defined(ESP_PLATFORM)
#            define CO_ARCH CO_ARCH_ESP32
#        elif defined(__ARMCC_VERSION)
#            define CO_ARCH CO_ARCH_ARMCC
#        endif
#    endif  // !defined(CO_ARCH)

// if the user did not specify an CO_ARCH, or specified a custom one, OR
// we guessed a known IDE, pull the customized config (Configuration Wizard)
#    if !defined(CO_ARCH) || (CO_ARCH == CO_ARCH_CUSTOM)
#        include "co_config_custom.h"  // keep this include
#    endif

#    define MAX_CAN_BUS_ID 1

#    ifndef SDO_MAX_LENGTH_TRANSFER
#        define SDO_MAX_LENGTH_TRANSFER 32
#    endif

#    ifndef SDO_BLOCK_SIZE
#        define SDO_BLOCK_SIZE 16
#    endif

#    ifndef SDO_MAX_SIMULTANEOUS_TRANSFERS
#        define SDO_MAX_SIMULTANEOUS_TRANSFERS 4
#    endif

#    ifndef NMT_MAX_NODE_ID
#        define NMT_MAX_NODE_ID 128
#    endif

#    ifndef SDO_TIMEOUT_MS
#        define SDO_TIMEOUT_MS 3000
#    endif

#    ifndef MAX_NB_TIMER
#        define MAX_NB_TIMER 32
#    endif

#    ifndef EMCY_MAX_ERRORS
#        define EMCY_MAX_ERRORS 8
#    endif

#    ifndef LSS_TIMEOUT_MS
#        define LSS_TIMEOUT_MS 1000
#    endif

#    ifndef LSS_FS_TIMEOUT_MS
#        define LSS_FS_TIMEOUT_MS 100
#    endif

#    ifndef NO_SDO_DYNAMIC_BUFFER_ALLOCATION
#        define SDO_DYNAMIC_BUFFER_ALLOCATION
#        ifndef SDO_DYNAMIC_BUFFER_ALLOCATION_SIZE
#            define SDO_DYNAMIC_BUFFER_ALLOCATION_SIZE 256
#        endif
#    endif

/* CANOPEN_BIG_ENDIAN is not defined */
/* US_TO_TIMEVAL_FACTOR is not defined */
/* TIMEVAL is not defined */
/* TIMEVAL_MAX is not defined */
/* RTCAN_SOCKET is not defined */

#    define REPEAT_SDO_MAX_SIMULTANEOUS_TRANSFERS_TIMES(repeat) repeat repeat repeat repeat
#    define REPEAT_NMT_MAX_NODE_ID_TIMES(repeat)                                                                       \
        repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat       \
            repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat   \
                repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat      \
                    repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat  \
                        repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat     \
                            repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat \
                                repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat    \
                                    repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat       \
                                        repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat   \
                                            repeat repeat repeat repeat repeat repeat repeat repeat repeat repeat
#    define REPEAT_EMCY_MAX_ERRORS_TIMES(repeat) repeat repeat repeat repeat repeat repeat repeat repeat
#endif /* _CONFIG_H_ */

#ifdef CO_STACK_ENABLE_DEBUG
#    ifdef CO_STACK_ENABLE_CUSTOM_LOG
#        define MSG(...) CO_LOG(__VA_ARGS__)
#        define MSG_WAR(num, str, val) CO_LOG_WAR("MSG_WAR %x %s %x\n", num, str, val)
#        define MSG_ERR(num, str, val) CO_LOG_ERR("MSG_ERR %x %s %x\n", num, str, val)
#    else
#        include "stdio.h"
#        define MSG(...) printf(__VA_ARGS__)
#        define MSG_WAR(num, str, val) MSG("MSG_WAR %x %s %x\n", num, str, val)
#        define MSG_ERR(num, str, val) MSG("MSG_ERR %x %s %x\n", num, str, val)
#    endif
#else
#    define MSG(...)
#    define MSG_WAR(num, str, val)
#    define MSG_ERR(num, str, val)
#endif  // CO_STACK_DEBUG_ENABLE
