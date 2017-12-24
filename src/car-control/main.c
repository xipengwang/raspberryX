#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>

#include "lcm/lcm.h"
#include "lcmtypes/drive_t.h"

#include "common/lcm_channels.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"

#define pin_redA PIN_03
#define pin_redB PIN_05
#define pin_yellowA PIN_08
#define pin_yellowB PIN_10

typedef struct state state_t;
struct state {
    lcm_t *lcm;
    double throttle;
    pthread_mutex_t lock;
};

static void drive_handler(const lcm_recv_buf_t *rbuf, const char * channel,
           const drive_t * msg, void * user)
{
    state_t *state = user;
    pthread_mutex_lock(&state->lock);
    state->throttle = msg->throttle;
    pthread_mutex_unlock(&state->lock);
}

void * on_control(void* user)
{
    state_t *state= user;
    while(1)
    {
        double throttle;
        pthread_mutex_lock(&state->lock);
        throttle = state->throttle;
        pthread_mutex_unlock(&state->lock);
        if(throttle < -0.5) {
            rpi_gpio_write(pin_yellowA, HIGH);
            rpi_gpio_write(pin_yellowB, HIGH);
            rpi_gpio_write(pin_redA, LOW);
            rpi_gpio_write(pin_redB, LOW);
        } else if(throttle < 0){
            rpi_gpio_write(pin_yellowA, HIGH);
            rpi_gpio_write(pin_yellowB, LOW);
            rpi_gpio_write(pin_redA, LOW);
            rpi_gpio_write(pin_redB, LOW);
        } else if(throttle > 0.5) {
            rpi_gpio_write(pin_yellowA, LOW);
            rpi_gpio_write(pin_yellowB, LOW);
            rpi_gpio_write(pin_redA, HIGH);
            rpi_gpio_write(pin_redB, HIGH);
        } else if(throttle > 0) {
            rpi_gpio_write(pin_yellowA, LOW);
            rpi_gpio_write(pin_yellowB, LOW);
            rpi_gpio_write(pin_redA, HIGH);
            rpi_gpio_write(pin_redB, LOW);
        } else {
            rpi_gpio_write(pin_yellowA, LOW);
            rpi_gpio_write(pin_yellowB, LOW);
            rpi_gpio_write(pin_redA, LOW);
            rpi_gpio_write(pin_redB, LOW);
        }
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
    rpi_gpio_fsel(pin_redA, RPI_GPIO_FSEL_OUT);
    rpi_gpio_fsel(pin_redB, RPI_GPIO_FSEL_OUT);
    rpi_gpio_fsel(pin_yellowA, RPI_GPIO_FSEL_OUT);
    rpi_gpio_fsel(pin_yellowB, RPI_GPIO_FSEL_OUT);
    rpi_gpio_write(pin_redA, HIGH);
    rpi_gpio_write(pin_redB, HIGH);
    rpi_gpio_write(pin_yellowA, HIGH);
    rpi_gpio_write(pin_yellowB, HIGH);
    usleep(2e6);
    rpi_gpio_write(pin_redA, LOW);
    rpi_gpio_write(pin_redB, LOW);
    rpi_gpio_write(pin_yellowA, LOW);
    rpi_gpio_write(pin_yellowB, LOW);

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
