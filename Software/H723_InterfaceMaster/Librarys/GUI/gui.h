/*
 * gui.h
 *
 *  Created on: Oct 19, 2025
 *      Author: MTA
 */

#ifndef GUI_GUI_H_
#define GUI_GUI_H_

typedef enum {
  SPLASH_SCREEN = 0U,
  WORK_MODE_SELECT_SCREEN = 1U,
  COMM_PROTOCOL_SELECT_SCREEN = 2U,
  DATA_WATCH_SCREEN = 3U,
} ScreenName_e;

typedef struct GUI_System_t {
  ScreenName_e active_screen;
  ScreenName_e previous_screen;
  lv_obj_t *p_current_screen_obj;
  lv_obj_t *p_previous_screen_obj;
  lv_timer_t *p_screen_timer;
  lv_timer_cb_t p_screen_timer_CB;
} GUI_System_t;

#endif /* GUI_GUI_H_ */
