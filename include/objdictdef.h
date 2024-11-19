#ifndef __OBJDICTDEF_H
#define __OBJDICTDEF_H

#include "applicfg.h"
#include "objacces.h"
#include "sysdep.h"

/************************* CONSTANTES **********************************/
/** this are static defined datatypes taken fCODE the canopen standard. They
 *  are located at index 0x0001 to 0x001B. As described in the standard, they
 *  are in the object dictionary for definition purpose only. a device does not
 *  to support all of this datatypes.
 */
#define boolean         0x01
#define int8            0x02
#define int16           0x03
#define int32           0x04
#define uint8           0x05
#define uint16          0x06
#define uint32          0x07
#define real32          0x08
#define visible_string  0x09
#define octet_string    0x0A
#define unicode_string  0x0B
#define time_of_day     0x0C
#define time_difference 0x0D

#define domain          0x0F
#define int24           0x10
#define real64          0x11
#define int40           0x12
#define int48           0x13
#define int56           0x14
#define int64           0x15
#define uint24          0x16

#define uint40          0x18
#define uint48          0x19
#define uint56          0x1A
#define uint64          0x1B

#define pdo_communication_parameter 0x20
#define pdo_mapping                 0x21
#define sdo_parameter               0x22
#define identity                    0x23

/* CanFestival is using 0x24 to 0xFF to define some types containing a 
 value range (See how it works in objdict.c)
 */


/** Each entry of the object dictionary can be READONLY (RO), READ/WRITE (RW),
 *  WRITE-ONLY (WO)
 */
#define RW     0x00  
#define WO     0x01
#define RO     0x02
#define CONST  0x03

#define TO_BE_SAVE  0x04
#define DCF_TO_SEND 0x08

#ifdef __AVR__
#define CONSTSTORE __flash
#else
#define CONSTSTORE
#endif

/************************ STRUCTURES ****************************/
/** This are some structs which are neccessary for creating the entries
 *  of the object dictionary.
 */
typedef struct td_subindex
{
    const UNS8              bAccessType;
    const UNS8              bDataType; /* Defines of what datatype the entry is */
    UNS32             size;      /* The size (in Byte) of the variable */
    union {
        void*          pObject;   /* This is the pointer of the Variable */
        const CONSTSTORE void* const pObjectConst;
    };
} subindex;

/** Struct for creating entries in the communictaion profile
 */
typedef struct td_indextable
{
    const CONSTSTORE subindex* const  pSubindex;   /* Pointer to the subindex */
    const UNS8   bSubCount;   /* the count of valid entries for this subindex
                         * This count here defines how many memory has been
                         * allocated. this memory does not have to be used.
                         */
    const UNS16   index;
} indextable;

typedef struct s_quick_index{
	UNS16 SDO_SVR;
	UNS16 SDO_CLT;
	UNS16 PDO_RCV;
	UNS16 PDO_RCV_MAP;
	UNS16 PDO_TRS;
	UNS16 PDO_TRS_MAP;
}quick_index;

/************************** MACROS *********************************/

//typedef struct struct_CO_Data CO_Data;
typedef UNS32 (*ODCallback_t)(CO_Data* d, UNS16 wIndex, UNS8 bSubindex);
typedef const CONSTSTORE indextable * (*scanIndexOD_t)(UNS16 wIndex, UNS32 * errorCode, ODCallback_t **Callback);

/* CANopen usefull helpers */
#define GET_NODE_ID(m)         (UNS16_LE(m.cob_id) & 0x7f)
#define GET_FUNCTION_CODE(m)   (UNS16_LE(m.cob_id) >> 7)

#endif /* __objdictdef_h__ */
