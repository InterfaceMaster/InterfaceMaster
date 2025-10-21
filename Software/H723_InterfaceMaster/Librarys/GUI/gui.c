/*
 * gui.c
 *
 *  Created on: Oct 19, 2025
 *      Author: MTA
 */

#include "gui_helper.h"
#include "lvgl.h"
#include "tasks.h"

/**
 *@brief This function initilize the gui.
 *@param p_sub_system Pointer of system instance structure object.
 *@retVal None.
 */

void gui_init(SystemInstance_t *p_sub_system) {
  splash_screen_init(p_sub_system);
}
