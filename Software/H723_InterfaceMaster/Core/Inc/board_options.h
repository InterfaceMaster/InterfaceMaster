/*
 * board_options.h
 *
 *  Created on: Aug 5, 2025
 *      Author: MTA
 */

#ifndef INC_BOARD_OPTIONS_H_
#define INC_BOARD_OPTIONS_H_

typedef enum {
  USB_ID_NONE = 0U,
  USB_ID_DEVICE = 1U,
  USB_ID_HOST = 2U,
} USB_InitType_e;

USB_InitType_e get_usb_ID_state(void);

#endif /* INC_BOARD_OPTIONS_H_ */
