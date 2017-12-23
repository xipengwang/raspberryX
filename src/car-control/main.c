#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>

#include "lcm/lcm.h"
#include "lcmtypes/drive_t.h"

#include "common/lcm_channels.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"

typedef struct state state_t;
struct state {
    lcm_t *lcm;
    pthread_mutex_t lock;
};

static void drive_handler(const lcm_recv_buf_t *rbuf, const char * channel,
           const drive_t * msg, void * user)
{
    printf("Received message on channel \"%s\":\n", channel);
    printf("  timestamp   = %"PRId64"\n", msg->utime);
    printf("  Throttle   = %f\n", msg->throttle);
}

int
main(int argc, char ** argv)
{
    state_t state;
    state.lcm = lcm_create(LCM_URL);
    if(!state.lcm)
        exit(-1);

    pthread_mutex_init(&state.lock, NULL);

    drive_t_subscribe(state.lcm, DRIVE_T_CHANNEL, &drive_handler, NULL);

    while(1) {
        lcm_handle(state.lcm);
    }

    lcm_destroy(state.lcm);
    return 0;
}
