/**
  ******************************************************************************
  * @file    pattern_manager.h
  * @brief   LED pattern manager for Smart LED Blinker.
  ******************************************************************************
  */
#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H



/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  LED pattern types.
  */
typedef enum {
    PATTERN_SOLID = 0,          /*!< All LEDs steady ON or OFF */
    PATTERN_BLINK_SLOW,         /*!< Slow blink (1Hz) */
    PATTERN_BLINK_FAST,         /*!< Fast blink (4Hz) */
    PATTERN_CHASE_CLOCKWISE,    /*!< Chase pattern clockwise */
    PATTERN_CHASE_ANTICLOCKWISE,/*!< Chase pattern anti-clockwise */
    PATTERN_KNIGHT_RIDER,       /*!< Knight Rider scanner effect */
    PATTERN_BREATHE,            /*!< Breathing effect (simulated PWM) */
    PATTERN_RAINBOW,            /*!< Color cycle (uses all LEDs) */
    PATTERN_RANDOM_TWINKLE,     /*!< Random LED twinkling */
    PATTERN_COUNT               /*!< Total number of patterns */
} pattern_t;

/**
  * @brief  Pattern manager states.
  */
typedef enum {
    PATTERN_STATE_STOPPED = 0,  /*!< Pattern not running */
    PATTERN_STATE_RUNNING,      /*!< Pattern running */
    PATTERN_STATE_PAUSED        /*!< Pattern paused */
} pattern_state_t;

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize pattern manager.
  * @retval None
  */
void pattern_manager_init(void);

/**
  * @brief  Set specific pattern.
  * @param  pattern: Pattern to set.
  * @retval None
  */
void pattern_manager_set_pattern(pattern_t pattern);

/**
  * @brief  Get current pattern.
  * @retval Current pattern.
  */
pattern_t pattern_manager_get_current(void);

/**
  * @brief  Get pattern state.
  * @retval Current pattern state.
  */
pattern_state_t pattern_manager_get_state(void);

/**
  * @brief  Switch to next pattern (circular).
  * @retval None
  */
void pattern_manager_next(void);

/**
  * @brief  Switch to previous pattern (circular).
  * @retval None
  */
void pattern_manager_prev(void);

/**
  * @brief  Start pattern execution.
  * @retval None
  */
void pattern_manager_start(void);

/**
  * @brief  Stop pattern execution.
  * @retval None
  */
void pattern_manager_stop(void);

/**
  * @brief  Pause pattern execution.
  * @retval None
  */
void pattern_manager_pause(void);

/**
  * @brief  Resume pattern execution.
  * @retval None
  */
void pattern_manager_resume(void);

/**
  * @brief  Update pattern (call in main loop).
  * @note   Handles timing and LED updates for current pattern.
  * @retval None
  */
void pattern_manager_update(void);



#endif /* PATTERN_MANAGER_H */

/******************************** END OF FILE *********************************/
