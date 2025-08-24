/*
 * ft5426_touch_ic.h
 *
 *  Created on: Aug 24, 2025
 *      Author: MTA
 */

#ifndef INC_FT5426_TOUCH_IC_H_
#define INC_FT5426_TOUCH_IC_H_

typedef enum {
  FT5426_PRESS_DOWN = 0U,
  FT5426_LIFT_UP = 1U,
  FT5426_CONTACT = 2U,
  FT5426_NO_EVENT = 3U,
} FT5426_Event_e;

typedef struct {
  FT5426_Event_e ft5426_event;
  uint16_t x_cord;
  uint16_t y_cord;
} FT5426_TouchData_t;

void ft5426_init(void);

#endif /* INC_FT5426_TOUCH_IC_H_ */
