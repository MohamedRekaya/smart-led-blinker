#include "stm32f4xx.h"
#include "led.h"
#include "button.h"
#include "systick.h"
#include "pattern_manager.h"

int main(void) {
    /* 1. Initialize system (ORDER MATTERS!) */
    systick_init();             /* Must be first for timing */
    led_init();                 /* Initialize LEDs */
    button_init();              /* Initialize button with EXTI */
    pattern_manager_init();     /* Initialize pattern manager */

    /* 2. Enable global interrupts */
    __enable_irq();

    /* 3. Start with pattern 0 (Solid) */
    pattern_manager_set_pattern(PATTERN_SOLID);
    pattern_manager_start();    /* Start pattern execution */

    /* 4. Main loop */
    while (1) {
        /* Update button state machine */
        button_update();

        /* Handle button events */
        button_event_t event = button_get_event();

        switch (event) {
            case BUTTON_EVENT_PRESSED:
                /* Short press: Toggle pattern pause/resume */
                if (pattern_manager_get_state() == PATTERN_STATE_RUNNING) {
                    pattern_manager_pause();
                    led_all_on();  /* Show paused state */
                } else {
                    pattern_manager_resume();
                    led_all_off(); /* Resume pattern */
                }
                break;

            case BUTTON_EVENT_LONG_PRESS:
                /* Long press: Next pattern */
                pattern_manager_next();
                break;

            case BUTTON_EVENT_DOUBLE_CLICK:
                /* Double click: Previous pattern */
                pattern_manager_prev();
                break;

            case BUTTON_EVENT_RELEASED:
                /* Release: Can be used for additional features */
                break;

            default:
                /* No event */
                break;
        }

        /* Update pattern (non-blocking) */
        pattern_manager_update();

        /* Small delay to reduce CPU usage */
        for (volatile int i = 0; i < 1000; i++);
    }
}
