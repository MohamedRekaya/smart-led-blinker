#include "stm32f4xx.h"
#include "led.h"
#include "board_config.h"

// Simple blocking delay for testing
static void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 1600; i++) {
        __NOP();
    }
}

int main(void) {
	// Initialize LEDs
	    led_init();


	    while (1) {
	        // Test 1: Individual LEDs
	           led_on(LED_GREEN);
	           led_off(LED_ORANGE);
	           led_on(LED_RED);
	           led_off(LED_BLUE);
	           delay_ms(1000);

	           // Test 2: Toggle all
	           led_all_toggle();
	           delay_ms(1000);

	           // Test 3: Pattern
	           led_set_pattern(0b0101);  // Green and Red
	           delay_ms(1000);
	           led_set_pattern(0b1010);  // Orange and Blue
	           delay_ms(1000);
	       }
}
