#ifndef __CAN_DRIVER_H
#define __CAN_DRIVER_H

#include "def.h"
#include "co_can.h"



int canOpen(CAN_PORT port, uint32_t bitrate, CO_Data *d);

int canReceive(CAN_PORT fd0, Message *m);

#if !defined(RCAN_WINDOWS) && !defined(RCAN_MACOS) && !defined(RCAN_UNIX)
void can_loop(CAN_PORT port, uint32_t bitrate, CO_Data *d);
#endif

int canSend(CAN_PORT fd0, Message const *m);

int canClose();


#define CASE_MSG(fc) case fc: MSG(#fc" ");break;

static inline void print_message(Message const *m) {
    int i;
    UNS8 fc;
    MSG("id:%02x ", m->cob_id & 0x7F);
    fc = m->cob_id >> 7;
    switch (fc) {
        case SYNC:
            if (m->cob_id == 0x080)
                MSG("SYNC ");
            else
                MSG("EMCY ");
            break;
#ifdef CO_ENABLE_LSS
        case LSS:
            if(m->cob_id == 0x7E5)
                MSG("MLSS ");
            else
                MSG("SLSS ");
        break;
#endif
        CASE_MSG(TIME_STAMP)
        CASE_MSG(PDO1tx)
        CASE_MSG(PDO1rx)
        CASE_MSG(PDO2tx)
        CASE_MSG(PDO2rx)
        CASE_MSG(PDO3tx)
        CASE_MSG(PDO3rx)
        CASE_MSG(PDO4tx)
        CASE_MSG(PDO4rx)
        CASE_MSG(SDOtx)
        CASE_MSG(SDOrx)
        CASE_MSG(NODE_GUARD)
        CASE_MSG(NMT)
    }
    if (fc == SDOtx) {
        switch (m->data[0] >> 5) {
            /* scs: server command specifier */
            CASE_MSG(UPLOAD_SEGMENT_RESPONSE)
            CASE_MSG(DOWNLOAD_SEGMENT_RESPONSE)
            CASE_MSG(INITIATE_DOWNLOAD_RESPONSE)
            CASE_MSG(INITIATE_UPLOAD_RESPONSE)
            CASE_MSG(ABORT_TRANSFER_REQUEST)
        }
    } else if (fc == SDOrx) {
        switch (m->data[0] >> 5) {
            /* ccs: client command specifier */
            CASE_MSG(DOWNLOAD_SEGMENT_REQUEST)
            CASE_MSG(INITIATE_DOWNLOAD_REQUEST)
            CASE_MSG(INITIATE_UPLOAD_REQUEST)
            CASE_MSG(UPLOAD_SEGMENT_REQUEST)
            CASE_MSG(ABORT_TRANSFER_REQUEST)
        }
    }
    MSG(" rtr:%d", m->rtr);
    MSG(" len:%d", m->len);
    for (i = 0; i < m->len; i++)
        MSG(" %02x", m->data[i]);
    MSG("\n");
}

#endif


