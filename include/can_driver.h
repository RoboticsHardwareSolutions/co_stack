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


#define _P(fc) case fc: MSG(#fc" ");break;

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
        _P(TIME_STAMP)
        _P(PDO1tx)
        _P(PDO1rx)
        _P(PDO2tx)
        _P(PDO2rx)
        _P(PDO3tx)
        _P(PDO3rx)
        _P(PDO4tx)
        _P(PDO4rx)
        _P(SDOtx)
        _P(SDOrx)
        _P(NODE_GUARD)
        _P(NMT)
    }
    if (fc == SDOtx) {
        switch (m->data[0] >> 5) {
            /* scs: server command specifier */
            _P(UPLOAD_SEGMENT_RESPONSE)
            _P(DOWNLOAD_SEGMENT_RESPONSE)
            _P(INITIATE_DOWNLOAD_RESPONSE)
            _P(INITIATE_UPLOAD_RESPONSE)
            _P(ABORT_TRANSFER_REQUEST)
        }
    } else if (fc == SDOrx) {
        switch (m->data[0] >> 5) {
            /* ccs: client command specifier */
            _P(DOWNLOAD_SEGMENT_REQUEST)
            _P(INITIATE_DOWNLOAD_REQUEST)
            _P(INITIATE_UPLOAD_REQUEST)
            _P(UPLOAD_SEGMENT_REQUEST)
            _P(ABORT_TRANSFER_REQUEST)
        }
    }
    MSG(" rtr:%d", m->rtr);
    MSG(" len:%d", m->len);
    for (i = 0; i < m->len; i++)
        MSG(" %02x", m->data[i]);
    MSG("\n");
}

#endif


