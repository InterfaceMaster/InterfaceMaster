/*
 * input_protocol_select_screen.c
 *
 *  Created on: Nov 1, 2025
 *      Author: MTA
 */

#include "gui_helper.h"
#include "image_declarations.h"
#include "lvgl.h"

static SystemInstance_t *s_sub_system;

static void button_event_CB(lv_event_t *e);

static lv_style_t button_cont_style;

static uint8_t *button_names[6U] = {"UART", "I2C",   "SPI",
                                    "CAN",  "RS232", "RS485"};

static lv_obj_t *button_array[6U];

void input_protocol_select_screen(SystemInstance_t *p_sub_system) {

  if (NULL == p_sub_system) {
    return;
  }

  s_sub_system = p_sub_system;

  s_sub_system->p_GUI->active_screen = COMM_PROTOCOL_SELECT_SCREEN;
  s_sub_system->p_GUI->p_screen_timer = NULL;

  IM_gui_create_screen(s_sub_system);

  /*--------------------------------------------------*/
  lv_style_init(&button_cont_style);
  lv_style_set_flex_flow(&button_cont_style, LV_FLEX_FLOW_ROW_WRAP);
  lv_style_set_flex_main_place(&button_cont_style, LV_FLEX_ALIGN_SPACE_EVENLY);
  lv_style_set_layout(&button_cont_style, LV_LAYOUT_FLEX);

  lv_obj_t *button_cont =
      lv_obj_create(s_sub_system->p_GUI->p_current_screen_obj);
  lv_obj_set_size(button_cont, 400U, 200U);
  lv_obj_center(button_cont);
  lv_obj_add_style(button_cont, &button_cont_style, LV_PART_MAIN);

  for (uint8_t i = 0U; i < 7U; ++i) {
    button_array[i] = lv_obj_create(button_cont);
    lv_obj_set_size(button_array[i], 70U, LV_SIZE_CONTENT);
    lv_obj_add_flag(button_array[i], LV_OBJ_FLAG_CLICKABLE);

    lv_obj_t *button_label = lv_label_create(button_array[i]);
    lv_label_set_text_static(button_label, button_names[i]);
    lv_obj_center(button_label);
  }
  lv_obj_add_event_cb(button_cont, button_event_CB, LV_EVENT_PRESSED, NULL);
}

static void button_event_CB(lv_event_t *e) {

  lv_obj_t *target = lv_event_get_target_obj(e);
  lv_obj_t *cont = lv_event_get_current_target_obj(e);

  if (target == cont) {
    return;
  }

  for (uint8_t i = 0U; i < 7U; ++i) {
    if (target == button_array[i]) {
      s_sub_system->p_comm_protocol->type = i + 1U;
      return;
    }
  }

  s_sub_system->p_GUI->p_previous_screen_obj = COMM_PROTOCOL_SELECT_SCREEN;
  s_sub_system->p_GUI->p_current_screen_obj = DATA_WATCH_SCREEN;

  IM_gui_change_screen(s_sub_system);
}
