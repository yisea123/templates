/*
    RExOS - embedded RTOS
    Copyright (c) 2011-2019, RExOS team
    All rights reserved.

    author: RL (jam_roma@yahoo.com)
*/

#include "button.h"
#include "app_private.h"
#include "../../rexos/userspace/nrf/nrf_driver.h"
#include "../../rexos/userspace/gpio.h"
#include "../../rexos/userspace/stdio.h"
#include "../../rexos/userspace/conv.h"
#include "../../rexos/userspace/pin.h"
#include "../../rexos/userspace/irq.h"
#include "../../rexos/midware/pinboard.h"

void button_init(APP* app)
{
#if (APP_DEBUG_BUTTON)
#endif // APP_DEBUG_BUTTON
    /* PINBOARD */
    app->button.pinboard = process_create(&__PINBOARD);
    app->button.pressed = false;
    app->button.timer_double = timer_create(APP_TIMER_BUTTON_DOUBLE_PRESS, HAL_PINBOARD);
    app->button.timer_long = timer_create(APP_TIMER_BUTTON_LONG_PRESS, HAL_PINBOARD);
    /* BUTTONS */
    ack(app->button.pinboard, HAL_REQ(HAL_PINBOARD, IPC_OPEN), BUTTON_PIN, BUTTON_FLAGS, 0);
}

static inline void button_press(APP* app)
{
    printf("BUTTON: press\n");
}

static inline void button_double_press(APP* app)
{
    printf("BUTTON: double press\n");
}

static inline void button_long_press(APP* app)
{
    printf("BUTTON: long press\n");
}

static inline void button_down(APP *app, uint8_t button)
{
    switch(button)
    {
        case BUTTON_PIN:
            printf("BUTTON: down\n");
            if(!app->button.pressed)
            {
                timer_start_ms(app->button.timer_long, B_PRESS_LONG_MS);
                app->button.pressed = true;
                return;
            }

            timer_stop(app->button.timer_double, APP_TIMER_BUTTON_DOUBLE_PRESS, HAL_PINBOARD);
            timer_stop(app->button.timer_long, APP_TIMER_BUTTON_LONG_PRESS, HAL_PINBOARD);
            app->button.pressed = false;
            button_double_press(app);
            break;
        default:
            break;
    }
}

static inline void button_up(APP *app, uint8_t button)
{
    switch(button)
    {
        case BUTTON_PIN:
            printf("BUTTON: up\n");
            if(app->button.pressed)
                timer_start_ms(app->button.timer_double, B_PRESS_MS);

            timer_stop(app->button.timer_long, APP_TIMER_BUTTON_LONG_PRESS, HAL_PINBOARD);
            break;
        default:
            break;
    }
}

static inline void button_timeout(APP* app, IPC* ipc)
{
    switch (ipc->param1)
    {
        case APP_TIMER_BUTTON_LONG_PRESS:
            timer_stop(app->button.timer_double, APP_TIMER_BUTTON_DOUBLE_PRESS, HAL_PINBOARD);
            app->button.pressed = false;
            button_long_press(app);
            return;

        case APP_TIMER_BUTTON_DOUBLE_PRESS:
            app->button.pressed = false;
            button_press(app);
            return;
    }
    error(ERROR_NOT_SUPPORTED);
}

void button_request(APP* app, IPC* ipc)
{
    switch (HAL_ITEM(ipc->cmd))
    {
        case PINBOARD_KEY_DOWN:
            button_down(app, ipc->param1);
            break;

        case PINBOARD_KEY_UP:
            button_up(app, ipc->param1);
            break;

        case IPC_TIMEOUT:
            button_timeout(app, ipc);
            break;

        default:
            error(ERROR_NOT_SUPPORTED);
    }
}