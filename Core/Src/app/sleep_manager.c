/**
  ******************************************************************************
  * @file    sleep_manager.c
  * @brief   Sleep mode manager implementation.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sleep_manager.h"
#include "led.h"
#include "button.h"
#include "systick.h"
#include "stm32f4xx.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define WAKEUP_PIN          GPIO_PIN_0      /* PA0 - User button */
#define WAKEUP_EXTI_LINE    EXTI_Line0
#define ENTER_SLEEP_DELAY_MS 1000           /* Time to show sleep animation */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static sleep_state_t sleep_state = SLEEP_STATE_AWAKE;
static sleep_mode_t sleep_mode = SLEEP_MODE_STOP;
static uint32_t sleep_enter_time = 0;
static bool wakeup_requested = false;

/* Pattern state to restore after wakeup
static struct {
    uint8_t pattern_id;
    uint32_t pattern_step;
    uint32_t pattern_timer;
} saved_pattern_state;
*/
/* Private function prototypes -----------------------------------------------*/
static void enter_sleep_mode(void);
static void enter_stop_mode(void);
static void configure_wakeup_source(void);
static void save_system_state(void);
static void restore_system_state(void);
static void sleep_indication_enter(void);
static void sleep_indication_exit(void);

/* Exported functions --------------------------------------------------------*/

void sleep_manager_init(void) {
    sleep_state = SLEEP_STATE_AWAKE;
    sleep_mode = SLEEP_MODE_STOP;
    wakeup_requested = false;

    // Enable PWR clock (required for low-power modes)
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // Configure wakeup source (PA0 button)
    configure_wakeup_source();
}

void sleep_manager_enter(void) {
    if (sleep_state != SLEEP_STATE_AWAKE) return;

    sleep_state = SLEEP_STATE_ENTERING;

    // 1. Visual indication: Entering sleep
    sleep_indication_enter();

    // 2. Save current system state
    save_system_state();

    // 3. Configure for low-power
    // Disable systick interrupt during sleep
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;

    // Optional: Reduce clock speed or disable peripherals
    // RCC->CFGR &= ~RCC_CFGR_HPRE;  // Set AHB prescaler to 1

    // 4. Enter sleep mode
    enter_sleep_mode();

    // 5. After wakeup (code continues here)
    sleep_state = SLEEP_STATE_WAKING;

    // 6. Restore system
    // Re-enable systick
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

    // Restore system state
    restore_system_state();

    // 7. Visual indication: Waking up
    sleep_indication_exit();

    sleep_state = SLEEP_STATE_AWAKE;
    wakeup_requested = false;
}

void sleep_manager_wake(void) {
    // This function is called from EXTI interrupt handler
    wakeup_requested = true;
}

bool sleep_manager_is_sleeping(void) {
    return (sleep_state == SLEEP_STATE_SLEEPING);
}

sleep_state_t sleep_manager_get_state(void) {
    return sleep_state;
}

sleep_mode_t sleep_manager_get_mode(void) {
    return sleep_mode;
}

void sleep_manager_set_mode(sleep_mode_t mode) {
    if (mode <= SLEEP_MODE_STANDBY) {
        sleep_mode = mode;
    }
}

void sleep_manager_toggle(void) {
    if (sleep_state == SLEEP_STATE_AWAKE) {
        sleep_manager_enter();
    } else {
        // Wakeup is handled by interrupt
        // This just sets flag for main loop to handle
        wakeup_requested = true;
    }
}

/* Private functions ---------------------------------------------------------*/

static void enter_sleep_mode(void) {
    sleep_state = SLEEP_STATE_SLEEPING;

    switch (sleep_mode) {
        case SLEEP_MODE_SLEEP:
            // Cortex-M4 Sleep mode (lightest)
            // Wake on any interrupt
            __WFI();  // Wait For Interrupt
            break;

        case SLEEP_MODE_STOP:
            // STM32 Stop mode (balanced power saving)
            enter_stop_mode();
            break;

        case SLEEP_MODE_STANDBY:
            // STM32 Standby mode (deepest)
            // Note: This causes full reset on wakeup
            // PWR->CR |= PWR_CR_PDDS;  // Enter Standby
            // PWR->CR |= PWR_CR_CWUF;  // Clear wakeup flag
            // SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            // __WFI();
            // Code won't reach here after standby
            break;
    }
}

static void enter_stop_mode(void) {
    // Simple Stop mode configuration for STM32F4

    // Configure Stop mode with voltage regulator in low-power
    PWR->CR |= PWR_CR_LPDS;        // Low-Power Deep Sleep mode
    PWR->CR &= ~PWR_CR_PDDS;       // Enter Stop mode (not Standby)

    // Set SLEEPDEEP bit
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Data synchronization barrier
    __DSB();

    // Wait for interrupt (enters Stop mode)
    __WFI();

    // After wakeup
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
}

static void configure_wakeup_source(void) {
    // PA0 is already configured as EXTI by button driver
    // Ensure rising edge is enabled for wakeup
    EXTI->RTSR |= EXTI_RTSR_TR0;  // Rising edge (button press)

    // For Stop mode, need to unmask EXTI line
    EXTI->IMR |= EXTI_IMR_MR0;

    // Clear any pending EXTI interrupt
    if (EXTI->PR & EXTI_PR_PR0) {
        EXTI->PR = EXTI_PR_PR0;
    }

    // For Standby mode (if using):
    // PWR->CSR |= PWR_CSR_EWUP;  // Enable WKUP pin
}

static void save_system_state(void) {
    // Save pattern manager state (if you have access)
    // In a real implementation, you'd save:
    // - Current pattern ID
    // - Pattern step counter
    // - Pattern timer value
    // - LED states

    // For now, just save timestamp
    sleep_enter_time = systick_get_ticks();

    // Turn off all LEDs
    led_all_off();
}

static void restore_system_state(void) {
    // Restore system to previous state
    // In a real implementation, you'd restore saved state

    // Re-initialize critical peripherals if needed
    // systick_init();  // Already running

    // Small delay to allow system to stabilize
    for (volatile uint32_t i = 0; i < 1000; i++);
}

static void sleep_indication_enter(void) {
    // Visual sequence: Entering sleep

    // 1. All LEDs ON briefly
    led_all_on();
    for (volatile uint32_t i = 0; i < 100000; i++);

    // 2. "Breathing" effect
    for (int breath = 0; breath < 3; breath++) {
        // Fade in
        for (int i = 0; i < 4; i++) {
            led_on(i);
            for (volatile int j = 0; j < 20000; j++);
        }
        // Fade out
        for (int i = 3; i >= 0; i--) {
            led_off(i);
            for (volatile int j = 0; j < 20000; j++);
        }
    }

    // 3. Single LED pulses
    for (int pulse = 0; pulse < 5; pulse++) {
        led_on(LED_GREEN);
        for (volatile int i = 0; i < 10000; i++);
        led_off(LED_GREEN);
        for (volatile int i = 0; i < 40000; i++);
    }

    // 4. All OFF
    led_all_off();
}

static void sleep_indication_exit(void) {
    // Visual sequence: Waking up

    // 1. Quick flash all LEDs
    for (int flash = 0; flash < 3; flash++) {
        led_all_on();
        for (volatile int i = 0; i < 10000; i++);
        led_all_off();
        for (volatile int i = 0; i < 10000; i++);
    }

    // 2. Chase from center outward
    led_on(LED_ORANGE);
    led_on(LED_RED);
    for (volatile int i = 0; i < 50000; i++);
    led_on(LED_GREEN);
    led_on(LED_BLUE);
    for (volatile int i = 0; i < 50000; i++);

    // 3. All ON briefly
    led_all_on();
    for (volatile int i = 0; i < 100000; i++);

    // 4. Return to normal (pattern will resume)
    led_all_off();
}

/******************************** END OF FILE *********************************/
