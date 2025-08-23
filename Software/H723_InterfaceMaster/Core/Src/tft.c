/*
 * tft.c
 *
 *  Created on: Aug 18, 2025
 *      Author: MTA
 */

#include "lvgl.h"

#include "main.h"
#include "stdint.h"

#define DISPLAY_WIDTH 480U
#define DISPLAY_HEIGHT 272U
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB888))
#define BUFF_SIZE (DISPLAY_WIDTH * 10 * BYTES_PER_PIXEL)

static uint8_t draw_buff_0[BUFF_SIZE] __attribute__((section(".sdram_data")));
static uint8_t draw_buff_1[BUFF_SIZE] __attribute__((section(".sdram_data")));

/*
  ==============================================================================
                      ##### STATIC FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function is called by LVGL when a display refresh is required.
 * @note For LTDC, this callback's primary role is to inform LVGL that the
 * drawing is complete, as the hardware display controller handles the actual
 * buffer-to-screen transfer.
 * @param disp A pointer to the display object.
 * @param area The area that has been invalidated and needs to be redrawn.
 * @param color_p A pointer to the pixel data to be drawn.
 */
void display_flush_cb(lv_display_t *disp, const lv_area_t *area,
                      lv_color_t *color_p) {
  lv_display_flush_ready(disp);
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function initialize LCD screen.
 * @param None.
 * @retval None.
 * */
void tft_init(void) {

  lv_display_t *display = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lv_display_set_flush_cb(display, display_flush_cb);
  lv_display_set_buffers(display, draw_buff_0, draw_buff_1, sizeof(draw_buff_0),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
}
