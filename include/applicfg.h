#ifndef __APPLICFG_H
#define __APPLICFG_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "logs.h"

/*  Define the architecture : little_endian or big_endian
 -----------------------------------------------------
 Test :
 UNS32 v = 0x1234ABCD;
 char *data = &v;

 Result for a little_endian architecture :
 data[0] = 0xCD;
 data[1] = 0xAB;
 data[2] = 0x34;
 data[3] = 0x12;

 Result for a big_endian architecture :
 data[0] = 0x12;
 data[1] = 0x34;
 data[2] = 0xAB;
 data[3] = 0xCD;
 */

/* Integers */
#define INTEGER8 int8_t
#define INTEGER16 int16_t
#define INTEGER24 int32_t
#define INTEGER32 int32_t
#define INTEGER40 int64_t
#define INTEGER48 int64_t
#define INTEGER56 int64_t
#define INTEGER64 int64_t

/* Unsigned integers */
#define UNS8   uint8_t
#define UNS16  uint16_t
#define UNS32  uint32_t
#define UNS24  uint32_t
#define UNS40  uint64_t
#define UNS48  uint64_t
#define UNS56  uint64_t
#define UNS64  uint64_t

/* Reals */
#define REAL32 float
#define REAL64 double
#ifdef CO_STACK_DEBUG_ENABLE
/* Definition of error and warning macros */
/* -------------------------------------- */
    #define MSG(...) LOG(__VA_ARGS__)
    /* Definition of MSG_ERR */
    /* --------------------- */
    #define MSG_WAR(num, str, val) LOG_WAR("MSG_WAR %u %s %x\n", num, str, val)
    #define MSG_ERR(num, str, val) LOG_ERR("MSG_ERR %u %s %x\n", num, str, val)
    #define MSG_TIME(...)          LOG(__VA_ARGS__)

#else

    #define MSG(...)
    #define MSG_WAR(num, str, val)
    #define MSG_ERR(num, str, val)
    #define MSG_TIME(...)
#endif //CO_STACK_DEBUG_ENABLE


#if defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW__)
#define dFUNCTION __PRETTY_FUNCTION__
#else
#ifdef _MSC_VER
       #define dFUNCTION __FUNCSIG__
   #else
       #define dFUNCTION __FUNCTION__
   #endif
#endif
//typedef void *CAN_HANDLE;
//TODO check need this fucking handle or not


typedef void *CAN_PORT;

#endif
