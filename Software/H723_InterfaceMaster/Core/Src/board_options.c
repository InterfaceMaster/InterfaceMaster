/*
 * board_options.c
 *
 *  Created on: Aug 5, 2025
 *      Author: MTA
 */

#include "board_options.h"
#include "main.h"

/**
 * @brief  This function using for reading USB OTG ID pin..
 * system.
 * @retval Type of USB init.
 */
USB_InitType_e get_usb_ID_state(void) {
  GPIO_PinState id_pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10);
  return (id_pin_state == GPIO_PIN_RESET) ? USB_ID_HOST : USB_ID_DEVICE;
};
