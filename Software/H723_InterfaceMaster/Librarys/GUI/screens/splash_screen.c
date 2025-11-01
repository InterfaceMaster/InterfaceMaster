/*
 * splash_screen.c
 *
 *  Created on: Oct 21, 2025
 *      Author: MTA
 */

#include "defines.h"
#include "gui.h"
#include "gui_helper.h"
#include "image_declarations.h"
#include "lvgl.h"

static SystemInstance_t s_sub_system;

static void logo_button_event_CB(lv_event_t *e);
static void screen_timer_CB(lv_timer_t *timer);

void splash_screen_init(SystemInstance_t *p_sub_system) {

  if (NULL == p_sub_system) {
    return;
  }

  s_sub_system = *p_sub_system;

  s_sub_system.p_GUI->active_screen = WORK_MODE_SELECT_SCREEN;
  s_sub_system.p_GUI->p_screen_timer_CB = screen_timer_CB;

  IM_gui_create_screen(&s_sub_system);

  /*--------------------------------------------------*/

  lv_obj_t *logo_image =
      lv_image_create(s_sub_system.p_GUI->p_current_screen_obj);
  lv_obj_set_size(logo_image, 480U, 272U);
  lv_image_set_src(logo_image, &logo_bit_map);
  lv_obj_center(logo_image);
  lv_obj_add_event_cb(logo_image, logo_button_event_CB, LV_EVENT_RELEASED,
                      NULL);
}

static void logo_button_event_CB(lv_event_t *e) {

  lv_event_code_t code = lv_event_get_code(e);

  if (LV_EVENT_RELEASED == code) {
    s_sub_system.p_GUI->previous_screen = SPLASH_SCREEN;
    s_sub_system.p_GUI->active_screen = WORK_MODE_SELECT_SCREEN;
    IM_gui_change_screen(&s_sub_system);
  }
}
