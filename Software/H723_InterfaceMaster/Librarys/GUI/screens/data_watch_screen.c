/*
 * data_watch_screen.c
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

void data_watch_screen(SystemInstance_t *p_sub_system) {

  if (NULL == p_sub_system) {
    return;
  }

  s_sub_system = p_sub_system;

  s_sub_system->p_GUI->active_screen = DATA_WATCH_SCREEN;
  s_sub_system->p_GUI->p_screen_timer_CB = NULL;

  IM_gui_create_screen(s_sub_system);

  /*--------------------------------------------------*/

  lv_obj_t *textarea =
      lv_textarea_create(s_sub_system->p_GUI->p_current_screen_obj);
  lv_obj_set_size(textarea, 400U, 200U);
  lv_obj_center(textarea);
  lv_obj_set_style_text_font(textarea, &lv_font_montserrat_14, LV_PART_MAIN);

  uint8_t *p_data_buffer = get_comm_protocol_tx_buff();
  lv_textarea_set_text(textarea, (const char *)p_data_buffer);
}
