/*
 * gui_helper.c
 *
 *  Created on: Oct 19, 2025
 *      Author: MTA
 */

#include "gui_helper.h"
#include "gui.h"
#include "lvgl.h"
#include "tasks.h"

#include <stdint.h>

#define SCREEN_TIMER_PERIOD 100U /*ms*/

static const uint16_t tft_hor_res = 480U;
static const uint16_t tft_ver_res = 272U;
/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 *@brief This function create new screen and load it to display.
 *@param p_gui Pointer of gui system scruture.
 *@retVal None.
 */

void IM_gui_create_screen(SystemInstance_t *p_sub_system) {

  if (NULL == p_sub_system) {
    return;
  }
  /*If previous screen is avaliable delete first then create new screen.*/
  if (NULL != p_sub_system->p_GUI->p_previous_screen_obj) {
    /*Check timer and delete*/
    if (NULL != p_sub_system->p_GUI->p_screen_timer) {
      lv_timer_delete(p_sub_system->p_GUI->p_screen_timer);
    }
    /*After timer delete, delete screen too.*/
    lv_obj_delete(p_sub_system->p_GUI->p_previous_screen_obj);
  }

  p_sub_system->p_GUI->p_current_screen_obj = lv_obj_create(lv_screen_active());
  lv_obj_set_size(p_sub_system->p_GUI->p_current_screen_obj,
                  (int32_t)tft_ver_res, (int32_t)tft_hor_res);

  lv_screen_load(p_sub_system->p_GUI->p_current_screen_obj);

  if (NULL != p_sub_system->p_GUI->p_screen_timer_CB) {
    p_sub_system->p_GUI->p_screen_timer = lv_timer_create(
        p_sub_system->p_GUI->p_screen_timer_CB, SCREEN_TIMER_PERIOD, NULL);
  }

  /*New screen also the previous screen after load the screen.*/
  if (NULL != p_sub_system->p_GUI->p_current_screen_obj) {
    p_sub_system->p_GUI->p_previous_screen_obj =
        p_sub_system->p_GUI->p_current_screen_obj;
  }
}

/**
 *@brief This function changes the screen.
 *@param p_gui Pointer of gui system scruture.
 *@retVal None.
 */

void IM_gui_change_screen(SystemInstance_t *p_sub_system) {

  if (NULL == p_sub_system) {
    return;
  }

  switch (p_sub_system->p_GUI->active_screen) {
  case SPLASH_SCREEN:
    splash_screen_init(p_sub_system);
    break;
  case WORK_MODE_SELECT_SCREEN:
    work_mode_screen_init(p_sub_system);
    break;
  case COMM_PROTOCOL_SELECT_SCREEN:
    input_protocol_select_screen(p_sub_system);
    break;
  case DATA_WATCH_SCREEN:
    data_watch_screen(p_sub_system);
    break;
  default:

    break;
  }
}
