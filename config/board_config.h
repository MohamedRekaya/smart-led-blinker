#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm32f4xx.h"
#include "gpio_pins.h"

// LED identifiers for Discovery board
typedef enum {
    LED_GREEN = 0,  // PD12
    LED_ORANGE,     // PD13
    LED_RED,        // PD14
    LED_BLUE,       // PD15
    LED_COUNT,
    LED_ALL = 0xFF  // Special value for all LEDs
} led_id_t;

// LED GPIO Configuration
#define LED_GPIO_PORT        GPIOD

#define LED_GPIO_CLK_ENABLE()  do {        \
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;   \
} while(0)

// LED Pin Definitions
// ===============================
// STM32F4 Discovery LEDs
// ===============================
#define LED_GREEN_PIN_NUM   GPIO_PIN_12_NUM
#define LED_GREEN_PIN_MSK   GPIO_PIN_12

#define LED_ORANGE_PIN_NUM  GPIO_PIN_13_NUM
#define LED_ORANGE_PIN_MSK  GPIO_PIN_13

#define LED_RED_PIN_NUM     GPIO_PIN_14_NUM
#define LED_RED_PIN_MSK     GPIO_PIN_14

#define LED_BLUE_PIN_NUM    GPIO_PIN_15_NUM
#define LED_BLUE_PIN_MSK    GPIO_PIN_15


#define LED_ALL_PINS     (LED_GREEN_PIN_MSK | LED_ORANGE_PIN_MSK | LED_RED_PIN_MSK | LED_BLUE_PIN_MSK)

// User button (PA0)
#define BUTTON_GPIO_PORT     GPIOA
#define BUTTON_GPIO_PIN      GPIO_PIN_0
#define BUTTON_GPIO_CLK_ENABLE() do { \
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; \
} while(0)

// Timing
#define SYSTEM_TICK_MS    1    // SysTick period in ms

#endif
