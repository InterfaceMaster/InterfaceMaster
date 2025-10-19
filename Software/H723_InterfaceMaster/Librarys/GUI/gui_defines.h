/*
 * gui_defines.h
 *
 *  Created on: Oct 19, 2025
 *      Author: MTA
 */

#ifndef GUI_GUI_DEFINES_H_
#define GUI_GUI_DEFINES_H_

#include "lvgl.h"

typedef enum {
  SPLASH_SCREEN = 0U,
  WORK_MODE_SELECT_SCREEN = 1U,
  COMM_PROTOCOL_SELECT_SCREEN = 2U,
  DATA_WATCH_SCREEN = 3U,
} ActiveScreen_e;

typedef struct GUI_System_t {
  ActiveScreen_e active_screen;
  lv_obj_t *p_current_screen_obj;
  lv_obj_t *p_previous_screen_obj;
  lv_timer_t *p_screen_timer;
  lv_timer_cb_t p_screen_timer_CB;
} GUI_System_t;

#endif /* GUI_GUI_DEFINES_H_ */
