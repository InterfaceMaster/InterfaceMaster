/*
 * input_protocol_select_screen.c
 *
 *  Created on: Nov 1, 2025
 *      Author: MTA
 */

#include "communication.h"
#include "defines.h"
#include "gui.h"
#include "gui_helper.h"
#include "image_declarations.h"
#include "lvgl.h"

static SystemInstance_t *s_sub_system;

static void button_event_CB(lv_event_t *e);

static lv_style_t button_cont_style;

static char *button_names[6U] = {"UART", "I2C", "SPI", "CAN", "RS232", "RS485"};

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

  if (target == button_array[0U]) {
    s_sub_system->p_comm_protocol->type = COMM_PROTOCOL_TYPE_UART;
  } else if (target == button_array[1U]) {
    s_sub_system->p_comm_protocol->type = COMM_PROTOCOL_TYPE_I2C;
  } else if (target == button_array[2U]) {
    s_sub_system->p_comm_protocol->type = COMM_PROTOCOL_TYPE_SPI;
  } else if (target == button_array[3U]) {
    s_sub_system->p_comm_protocol->type = COMM_PROTOCOL_TYPE_CAN;
  } else if (target == button_array[4U]) {
    s_sub_system->p_comm_protocol->type = COMM_PROTOCOL_TYPE_RS232;
  } else if (target == button_array[5U]) {
    s_sub_system->p_comm_protocol->type = COMM_PROTOCOL_TYPE_RS485;
  }

  if (WORK_MODE_USB_BRIDGE == s_sub_system->device_work_mode) {
    s_sub_system->p_GUI->active_screen = DATA_WATCH_SCREEN;
    s_sub_system->start_usb_bridge = 1U;
  } else if (WORK_MODE_EXPORT_DATA == s_sub_system->device_work_mode) {
    //	  s_sub_system->p_GUI->active_screen = DATA_INFO
    s_sub_system->start_export_data = 1U;
  }

  IM_gui_change_screen(s_sub_system);
}
