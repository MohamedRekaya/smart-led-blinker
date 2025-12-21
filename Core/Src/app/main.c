
#include "stm32f4xx.h"
#include "led.h"

// Simple blocking delay for testing
static void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 1600; i++) {
        __NOP();
    }
}

int main(void) {
    led_init();

    // Test only GREEN LED
    led_blink(LED_GREEN, 750, 250);  // 50% duty cycle
    led_blink(LED_ORANGE, 750, 250);

    while (1) {
        led_update_all();
        delay_ms(1);
    }
}
