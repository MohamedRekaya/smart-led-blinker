/**
  ******************************************************************************
  * @file    interrupts.c
  * @brief   Interrupt handler for button EXTI.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "button.h"

/**
  * @brief  EXTI0 interrupt handler (PA0 button).
  */
void EXTI0_IRQHandler(void) {
    button_exti_handler();
}
