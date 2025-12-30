/**
  ******************************************************************************
  * @file    sleep_manager.h
  * @brief   Sleep mode manager for low-power operation.
  ******************************************************************************
  */
#ifndef SLEEP_MANAGER_H
#define SLEEP_MANAGER_H


#include <stdbool.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  Sleep mode types available.
  */
typedef enum {
    SLEEP_MODE_SLEEP = 0,    /*!< Cortex-M4 Sleep mode (lightest) */
    SLEEP_MODE_STOP,         /*!< STM32 Stop mode (balanced) */
    SLEEP_MODE_STANDBY       /*!< STM32 Standby mode (deepest) */
} sleep_mode_t;

/**
  * @brief  Sleep manager states.
  */
typedef enum {
    SLEEP_STATE_AWAKE = 0,   /*!< System is awake and running */
    SLEEP_STATE_ENTERING,    /*!< System is entering sleep */
    SLEEP_STATE_SLEEPING,    /*!< System is in sleep mode */
    SLEEP_STATE_WAKING       /*!< System is waking up */
} sleep_state_t;

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize sleep manager.
  * @retval None
  */
void sleep_manager_init(void);

/**
  * @brief  Enter sleep mode.
  * @note   Blocks until sleep is entered.
  * @retval None
  */
void sleep_manager_enter(void);

/**
  * @brief  Wake up from sleep mode.
  * @note   Called automatically on wakeup interrupt.
  * @retval None
  */
void sleep_manager_wake(void);

/**
  * @brief  Check if system is sleeping.
  * @retval true if sleeping, false if awake.
  */
bool sleep_manager_is_sleeping(void);

/**
  * @brief  Get current sleep state.
  * @retval Current sleep state.
  */
sleep_state_t sleep_manager_get_state(void);

/**
  * @brief  Get configured sleep mode.
  * @retval Configured sleep mode.
  */
sleep_mode_t sleep_manager_get_mode(void);

/**
  * @brief  Set sleep mode.
  * @param  mode: Sleep mode to use.
  * @retval None
  */
void sleep_manager_set_mode(sleep_mode_t mode);

/**
  * @brief  Toggle sleep state (enter/exit).
  * @retval None
  */
void sleep_manager_toggle(void);


#endif /* SLEEP_MANAGER_H */

/******************************** END OF FILE *********************************/
