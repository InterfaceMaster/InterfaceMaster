/*
 * splash_screen.c
 *
 *  Created on: Oct 19, 2025
 *      Author: MTA
 */

#include <stdint.h>

#include "gui_helper.h"
#include "lvgl.h"

static SystemInstance_t s_sub_system;

static lv_timer_cb_t screen_timer_CB(lv_timer_t *timer);

static lv_event_cb_t bridge_mode_button_event_CB(lv_event_t *e);
static lv_event_cb_t data_export_mode_button_event_CB(lv_event_t *e);

void splash_screen_init(SystemInstance_t *p_sub_system) {

  if (NULL == p_sub_system) {
    return;
  }

  s_sub_system = *p_sub_system;

  p_sub_system->p_GUI->active_screen = SPLASH_SCREEN;
  p_sub_system->p_GUI->p_screen_timer_CB = screen_timer_CB;

  IM_gui_create_screen(p_sub_system->p_GUI);

  /*--------------------------------------------------*/

  lv_obj_t *bridge_mode_button =
      lv_obj_create(p_sub_system->p_GUI->active_screen);
  lv_obj_set_size(bridge_mode_button, 180U, 220U);
  lv_obj_align(bridge_mode_button, LV_ALIGN_CENTER, -20U, 0U);
  lv_obj_add_event_cb(bridge_mode_button, bridge_mode_button_event_CB,
                      LV_EVENT_CLICKED, NULL);

  lv_obj_t *bridge_mode_button_label = lv_label_create(bridge_mode_button);
  lv_obj_set_size(bridge_mode_button_label, 170U, 50U);
  lv_obj_set_style_text_font(bridge_mode_button_label,
                             &lv_font_benchmark_montserrat_14_aligned,
                             LV_PART_MAIN);
  lv_obj_center(bridge_mode_button_label);

  /*--------------------------------------------------*/

  lv_obj_t *data_export_mode_button =
      lv_button_create(p_sub_system->p_GUI->p_current_screen_obj);
  lv_obj_set_size(data_export_mode_button, 180U, 220U);
  lv_obj_add_event_cb(data_export_mode_button, data_export_mode_button_event_CB,
                      LV_EVENT_CLICKED, NULL);
  lv_obj_align(data_export_mode_button, LV_ALIGN_CENTER, 20U, 0U);

  lv_obj_t *data_export_mode_button_label =
      lv_label_create(data_export_mode_button);
  lv_obj_set_size(data_export_mode_button_label, 170U, 50U);
  lv_obj_set_style_text_font(data_export_mode_button_label,
                             &lv_font_benchmark_montserrat_14_aligned,
                             LV_PART_MAIN);

  /*--------------------------------------------------*/

  lv_obj_center(data_export_mode_button_label);
}

static lv_event_cb_t bridge_mode_button_event_CB(lv_event_t *e) {

  lv_event_code_t code = lv_event_get_code(e);
}

static lv_event_cb_t data_export_mode_button_event_CB(lv_event_t *e) {

  lv_event_code_t code = lv_event_get_code(e);
}
