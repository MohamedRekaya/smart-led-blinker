#include "stm32f4xx.h"
#include "systick.h"
#include "led.h"



// Full test with all features
int main(void) {
    systick_init();
    led_init();

    // All 4 LEDs with different patterns
    led_blink(LED_GREEN,  100, 400);
    led_blink(LED_ORANGE, 200, 300);
    led_blink(LED_RED,    300, 200);
    led_blink(LED_BLUE,   400, 100);

    // Add non-blocking print every 1s
    uint32_t last_print = 0;

    while (1) {
        led_update_all();

        if (systick_delay_elapsed(last_print, 1000)) {
            // Debug output here
            last_print = systick_get_ticks();
        }
    }
}
