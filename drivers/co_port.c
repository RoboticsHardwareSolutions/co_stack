#include "canfestival.h"
#include "timer.h"
#include "data.h"
#include "co_port.h"
#include "rcounter.h"
#include "rcan.h"
#include "stm32g4xx_hal.h"
#include "led.h"

TIMEVAL last_counter_val = 0;
TIMEVAL elapsed_time = 0;
static CO_Data *co_data = NULL;
rcan can;
uint32_t current_bitrate = 0;


void co_tim_cb(void) {
    last_counter_val = 0;
    elapsed_time = 0;
    TimeDispatch();
}

void initTimer(void) {
    rcounter_register_overflow_cb(co_tim_cb);
    if (!rcounter_start(_10us)) {
        printf("rcounter error start");
    }

}

void setTimer(TIMEVAL value) {
    uint32_t timer = rcounter_get_tick();
    elapsed_time += timer - last_counter_val;
    last_counter_val = 65535 - value;
    rcounter_set_tick(65535 - value);
}


TIMEVAL getElapsedTime(void) {
    uint32_t timer = rcounter_get_tick();
    if (timer < last_counter_val)
        timer += 65535;
    TIMEVAL elapsed = timer - last_counter_val + elapsed_time;
    return elapsed;
}


unsigned char canInit(CO_Data *d, uint32_t bitrate) {
    co_data = d;
    current_bitrate = bitrate;
    if (!rcan_start(&can, FDCAN1_BASE, bitrate)) {
        printf("rcan not start\r\n");
    }
    return 1;
}

unsigned char canSend(CAN_PORT notused, Message *m) {
    rcan_frame frame = {0};
    frame.id = m->cob_id;
    frame.type = std;
    frame.len = m->len;
    frame.rtr = m->rtr;
    memcpy(frame.payload, m->data, m->len);
    return rcan_send(&can, &frame);
}


void can_loop(void) {
    Message rxm = {0};
    rcan_frame frame;

    if (!rcan_is_ok(&can)) {
        led_red_on();
        rcan_stop(&can);
        rcan_start(&can, FDCAN1_BASE, current_bitrate);
        led_red_off();
    }

    if (rcan_receive(&can, &frame)) {
        if (frame.type != std)
            return;
        rxm.cob_id = frame.id;
        rxm.rtr = frame.rtr;
        rxm.len = frame.len;
        memcpy(rxm.data, frame.payload, frame.len);
        canDispatch(co_data, &rxm);
    }
}


