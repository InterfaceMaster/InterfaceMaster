/*
 * gui_helper.h
 *
 *  Created on: Oct 19, 2025
 *      Author: MTA
 */

#ifndef GUI_HELPER_GUI_HELPER_H_
#define GUI_HELPER_GUI_HELPER_H_

#include "defines.h"

void IM_gui_create_screen(SystemInstance_t *p_sub_system);
void IM_gui_change_screen(SystemInstance_t *p_sub_system);

void splash_screen_init(SystemInstance_t *p_sub_system);
void work_mode_screen_init(SystemInstance_t *p_sub_system);

#endif /* GUI_HELPER_GUI_HELPER_H_ */
