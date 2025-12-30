/**
  ******************************************************************************
  * @file    pattern_manager.c
  * @brief   LED pattern manager implementation.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pattern_manager.h"
#include "led.h"
#include "systick.h"
#include <stdlib.h>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define CHASE_DELAY_MS       150   /*!< Chase pattern delay */
#define KNIGHT_RIDER_DELAY_MS 80   /*!< Knight Rider delay */
#define BREATHE_CYCLE_MS     3000  /*!< Breathe effect cycle time */
#define TWINKLE_MIN_DELAY_MS  100  /*!< Minimum twinkle delay */
#define TWINKLE_MAX_DELAY_MS  800  /*!< Maximum twinkle delay */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static pattern_t current_pattern = PATTERN_SOLID;
static pattern_state_t pattern_state = PATTERN_STATE_STOPPED;
static uint32_t pattern_timer = 0;
static uint8_t pattern_step = 0;
static bool breathe_direction = true;  /* true = brightening, false = dimming */
static uint8_t breathe_step = 0;

/* Private function prototypes -----------------------------------------------*/
static void execute_solid_pattern(void);
static void execute_blink_slow(void);
static void execute_blink_fast(void);
static void execute_chase_clockwise(void);
static void execute_chase_anticlockwise(void);
static void execute_knight_rider(void);
static void execute_breathe(void);
static void execute_rainbow(void);
static void execute_random_twinkle(void);

/* Exported functions --------------------------------------------------------*/

void pattern_manager_init(void) {
    current_pattern = PATTERN_SOLID;
    pattern_state = PATTERN_STATE_STOPPED;
    pattern_timer = systick_get_ticks();
    pattern_step = 0;
    breathe_direction = true;
    breathe_step = 0;
}

void pattern_manager_set_pattern(pattern_t pattern) {
    if (pattern >= PATTERN_COUNT) return;

    current_pattern = pattern;
    pattern_step = 0;
    pattern_timer = systick_get_ticks();
    breathe_step = 0;
    breathe_direction = true;

    // DO NOT turn LEDs off here
    // led_all_off();  // ← REMOVE THIS LINE

    // Start pattern automatically
    pattern_state = PATTERN_STATE_RUNNING;
}

pattern_t pattern_manager_get_current(void) {
    return current_pattern;
}

pattern_state_t pattern_manager_get_state(void) {
    return pattern_state;
}

void pattern_manager_next(void) {
    pattern_manager_set_pattern((current_pattern + 1) % PATTERN_COUNT);
}

void pattern_manager_prev(void) {
    pattern_manager_set_pattern((current_pattern == 0) ? PATTERN_COUNT - 1 : current_pattern - 1);
}

void pattern_manager_start(void) {
    pattern_state = PATTERN_STATE_RUNNING;
    pattern_timer = systick_get_ticks();
}

void pattern_manager_stop(void) {
    pattern_state = PATTERN_STATE_STOPPED;
    led_all_off();
}

void pattern_manager_pause(void) {
    pattern_state = PATTERN_STATE_PAUSED;
}

void pattern_manager_resume(void) {
    pattern_state = PATTERN_STATE_RUNNING;
}

void pattern_manager_update(void) {
    if (pattern_state != PATTERN_STATE_RUNNING) return;

    switch (current_pattern) {
        case PATTERN_SOLID:
            execute_solid_pattern();
            break;

        case PATTERN_BLINK_SLOW:
            execute_blink_slow();
            break;

        case PATTERN_BLINK_FAST:
            execute_blink_fast();
            break;

        case PATTERN_CHASE_CLOCKWISE:
            execute_chase_clockwise();
            break;

        case PATTERN_CHASE_ANTICLOCKWISE:
            execute_chase_anticlockwise();
            break;

        case PATTERN_KNIGHT_RIDER:
            execute_knight_rider();
            break;

        case PATTERN_BREATHE:
            execute_breathe();
            break;

        case PATTERN_RAINBOW:
            execute_rainbow();
            break;

        case PATTERN_RANDOM_TWINKLE:
            execute_random_twinkle();
            break;

        default:
            // Default to solid pattern
            led_all_on();
            break;
    }
}

/* Private functions ---------------------------------------------------------*/

static void execute_solid_pattern(void) {
    // All LEDs ON
    led_all_on();
}

static void execute_blink_slow(void) {
    // 1Hz blink (500ms ON, 500ms OFF)
    if (systick_delay_elapsed(pattern_timer, 500)) {
        led_all_toggle();
        pattern_timer = systick_get_ticks();
    }
}

static void execute_blink_fast(void) {
    // 4Hz blink (125ms ON, 125ms OFF)
    if (systick_delay_elapsed(pattern_timer, 125)) {
        led_all_toggle();
        pattern_timer = systick_get_ticks();
    }
}

static void execute_chase_clockwise(void) {
    if (systick_delay_elapsed(pattern_timer, CHASE_DELAY_MS)) {
        led_all_off();

        // Turn on current LED in sequence: Green → Orange → Red → Blue
        switch (pattern_step) {
            case 0: led_on(LED_GREEN); break;
            case 1: led_on(LED_ORANGE); break;
            case 2: led_on(LED_RED); break;
            case 3: led_on(LED_BLUE); break;
        }

        pattern_step = (pattern_step + 1) % 4;
        pattern_timer = systick_get_ticks();
    }
}

static void execute_chase_anticlockwise(void) {
    if (systick_delay_elapsed(pattern_timer, CHASE_DELAY_MS)) {
        led_all_off();

        // Reverse direction: Blue → Red → Orange → Green
        switch (pattern_step) {
            case 0: led_on(LED_BLUE); break;
            case 1: led_on(LED_RED); break;
            case 2: led_on(LED_ORANGE); break;
            case 3: led_on(LED_GREEN); break;
        }

        pattern_step = (pattern_step + 1) % 4;
        pattern_timer = systick_get_ticks();
    }
}

static void execute_knight_rider(void) {
    if (systick_delay_elapsed(pattern_timer, KNIGHT_RIDER_DELAY_MS)) {
        static int8_t direction = 1;  // 1 = forward, -1 = backward

        led_all_off();
        led_on(pattern_step);  // Use LED ID directly (0=Green, 1=Orange, etc.)

        pattern_step += direction;

        // Reverse direction at ends
        if (pattern_step == 3 || pattern_step == 0) {
            direction = -direction;
        }

        pattern_timer = systick_get_ticks();
    }
}

static void execute_breathe(void) {
    // Simulated PWM breathing effect
    uint32_t breathe_interval = BREATHE_CYCLE_MS / 10;  // 10 brightness steps

    if (systick_delay_elapsed(pattern_timer, breathe_interval)) {
        // Create breathing pattern by turning LEDs on/off in sequence
        uint8_t pattern = 0;

        if (breathe_direction) {
            // Brightening: turn on more LEDs
            for (int i = 0; i <= breathe_step; i++) {
                pattern |= (1 << i);
            }
        } else {
            // Dimming: turn off LEDs
            for (int i = breathe_step; i < 4; i++) {
                pattern |= (1 << i);
            }
        }

        led_set_pattern(pattern);

        breathe_step++;
        if (breathe_step >= 10) {
            breathe_step = 0;
            breathe_direction = !breathe_direction;
        }

        pattern_timer = systick_get_ticks();
    }
}

static void execute_rainbow(void) {
    // Cycle through color patterns
    if (systick_delay_elapsed(pattern_timer, 500)) {
        uint8_t patterns[] = {
            0b0001,  // Green only
            0b0011,  // Green + Orange
            0b0110,  // Orange + Red
            0b1100,  // Red + Blue
            0b1001,  // Blue + Green
            0b0101,  // Green + Red
            0b1010,  // Orange + Blue
            0b1111   // All on
        };

        led_set_pattern(patterns[pattern_step]);
        pattern_step = (pattern_step + 1) % 8;
        pattern_timer = systick_get_ticks();
    }
}

static void execute_random_twinkle(void) {
    if (systick_delay_elapsed(pattern_timer,
        TWINKLE_MIN_DELAY_MS + (rand() % (TWINKLE_MAX_DELAY_MS - TWINKLE_MIN_DELAY_MS)))) {

        // Turn off all LEDs
        led_all_off();

        // Randomly light 1-2 LEDs
        uint8_t num_leds = 1 + (rand() % 2);
        for (uint8_t i = 0; i < num_leds; i++) {
            led_id_t random_led = rand() % LED_COUNT;
            led_on(random_led);
        }

        pattern_timer = systick_get_ticks();
    }
}

/******************************** END OF FILE *********************************/
