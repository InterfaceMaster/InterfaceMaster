/*
 * splash_screen.c
 *
 *  Created on: Oct 19, 2025
 *      Author: MTA
 */

#include "defines.h"
#include "gui.h"
#include "gui_helper.h"
#include "image_declarations.h"
#include "lvgl.h"

static SystemInstance_t *s_sub_system;

static void bridge_mode_button_event_CB(lv_event_t *e);
static void data_export_mode_button_event_CB(lv_event_t *e);

void work_mode_screen_init(SystemInstance_t *p_sub_system) {

  if (NULL == p_sub_system) {
    return;
  }

  s_sub_system = p_sub_system;

  s_sub_system->p_GUI->active_screen = WORK_MODE_SELECT_SCREEN;
  s_sub_system->p_GUI->p_screen_timer_CB = NULL;

  IM_gui_create_screen(s_sub_system);

  /*--------------------------------------------------*/

  lv_obj_t *bridge_mode_button =
      lv_obj_create(s_sub_system->p_GUI->p_current_screen_obj);
  lv_obj_set_size(bridge_mode_button, 180U, 220U);
  lv_obj_align(bridge_mode_button, LV_ALIGN_LEFT_MID, 60U, -110U);
  lv_obj_add_event_cb(bridge_mode_button, bridge_mode_button_event_CB,
                      LV_EVENT_RELEASED, NULL);

  lv_obj_t *bridge_mode_button_label = lv_label_create(bridge_mode_button);
  lv_obj_set_size(bridge_mode_button_label, 170U, 50U);
  lv_obj_set_style_text_font(bridge_mode_button_label, &lv_font_montserrat_14,
                             LV_PART_MAIN);

  lv_obj_center(bridge_mode_button_label);

  /*--------------------------------------------------*/

  lv_obj_t *data_export_mode_button =
      lv_button_create(s_sub_system->p_GUI->p_current_screen_obj);
  lv_obj_set_size(data_export_mode_button, 180U, 220U);
  lv_obj_add_event_cb(data_export_mode_button, data_export_mode_button_event_CB,
                      LV_EVENT_RELEASED, NULL);
  lv_obj_align(data_export_mode_button, LV_ALIGN_RIGHT_MID, -240U, -110U);

  lv_obj_t *data_export_mode_button_label =
      lv_label_create(data_export_mode_button);
  lv_obj_set_size(data_export_mode_button_label, 170U, 50U);
  lv_obj_set_style_text_font(data_export_mode_button_label,
                             &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_center(data_export_mode_button_label);

  /*--------------------------------------------------*/
}

static void bridge_mode_button_event_CB(lv_event_t *e) {

  lv_event_code_t code = lv_event_get_code(e);

  if (LV_EVENT_RELEASED == code) {
    s_sub_system->device_work_mode = WORK_MODE_USB_BRIDGE;
    s_sub_system->p_GUI->active_screen = COMM_PROTOCOL_SELECT_SCREEN;
    IM_gui_change_screen(s_sub_system);
  }
}

static void data_export_mode_button_event_CB(lv_event_t *e) {

  lv_event_code_t code = lv_event_get_code(e);

  if (LV_EVENT_RELEASED == code) {
    s_sub_system->device_work_mode = WORK_MODE_EXPORT_DATA;
    s_sub_system->p_GUI->active_screen = COMM_PROTOCOL_SELECT_SCREEN;
    IM_gui_change_screen(s_sub_system);
  }
}
