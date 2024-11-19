#ifndef __CO_CAN_H__
#define __CO_CAN_H__

#include "applicfg.h"

/**
 * @brief The CAN message structure
 * @ingroup can
 */
struct Message {
    UNS16 cob_id;    /**< message's ID */
    UNS8 rtr;        /**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
    UNS8 len;        /**< message's length (0 to 8) */
    UNS8 data[8]; /**< message's datas */
};

//typedef struct Message Message;

#define Message_Initializer {0,0,0,{0,0,0,0,0,0,0,0}}

typedef struct Message Message;

typedef UNS8 (*canSend_t)(Message *);

#endif /* __CO_CAN_H__ */
