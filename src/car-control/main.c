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
    double steering;
    pthread_mutex_t lock;
};

static void drive_handler(const lcm_recv_buf_t *rbuf, const char * channel,
           const drive_t * msg, void * user)
{
    state_t *state = user;
    pthread_mutex_lock(&state->lock);
    state->steering = msg->steering;
    pthread_mutex_unlock(&state->lock);
}

void * on_control(void* user)
{
    state_t *state= user;
    while(1)
    {
        double steering;
        pthread_mutex_lock(&state->lock);
        steering = state->steering;
        pthread_mutex_unlock(&state->lock);
        printf("Steering:%f\n", steering);
        // Set PWM Frequency = 19.2Mhz / 16 / 1000 = 600Hz
        // pwm_set_range(0, 1000);
        pwm_set_range(0, 1000*12); // 50HZ

        // Set PWM duty cycle as 800 / 1000 = 80%
        // pwm_set_data(0, 800);
        int leftMost = 100*6;
        int rightMost = 100*12;
        pwm_set_data(0, (uint32_t)(100*6 + (1+steering)*100*3));
        usleep(100e3);
    }
    return NULL;
}
int main(int argc, char ** argv)
{
    if (rpi_init() == -1) {
        printf("initialization failed");
        return -1;
    }

    state_t *state = calloc(1, sizeof(state_t));
    state->lcm = lcm_create(LCM_URL);
    if(!state->lcm)
        exit(-1);

    rpi_gpio_fsel(PIN_12, RPI_GPIO_FSEL_ALT5);
    if(pwm_init(RPI_PWM_CHANNEL_0, 1, 1)) {
        printf("Initializing PWM fails \n");
        exit(-1);
    }
    pwm_set_clock(32);

    pthread_mutex_init(&state->lock, NULL);
    drive_t_subscribe(state->lcm, DRIVE_T_CHANNEL, &drive_handler, state);
    pthread_t control_thread;
    pthread_create(&control_thread, NULL, on_control, state);

    while(1) {
        lcm_handle(state->lcm);
    }

    lcm_destroy(state->lcm);
    return 0;
}
