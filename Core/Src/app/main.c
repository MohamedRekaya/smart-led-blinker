#include "stm32f4xx.h"
#include "led.h"
#include "systick.h"
#include "button.h"


int main(void) {
    /* 1. Initialize system */
    systick_init();     /* Must be first for timing */
    led_init();         /* Initialize LEDs */
    button_init();      /* Initialize button with EXTI */

    /* 2. Initial state */
    led_on(LED_GREEN);  /* Start with green ON */
    led_off(LED_ORANGE);
    led_off(LED_RED);
    led_off(LED_BLUE);

    /* 3. Main loop */
    while (1) {
        /* Update button state machine */
        button_update();

        /* Check for button events */
        button_event_t event = button_get_event();

        /* Handle events */
        switch (event) {
            case BUTTON_EVENT_PRESSED:
                /* Button pressed - toggle orange LED */
                led_toggle(LED_ORANGE);
                break;

            case BUTTON_EVENT_RELEASED:
                /* Button released - toggle red LED */
                led_toggle(LED_RED);
                break;

            case BUTTON_EVENT_LONG_PRESS:
                /* Long press (2s) - toggle blue LED */
                led_toggle(LED_BLUE);
                break;

            case BUTTON_EVENT_DOUBLE_CLICK:
                /* Double click - toggle all LEDs */
                led_all_toggle();
                break;

            case BUTTON_EVENT_NONE:
            default:
                /* No event */
                break;
        }

        /* Update any blinking LEDs (if using blink functions) */
        // led_update_all();

        /* Optional: Small sleep to reduce CPU usage */
        // __WFI();
    }
}
