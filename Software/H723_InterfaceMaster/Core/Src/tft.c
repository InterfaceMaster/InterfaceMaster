/*
 * tft.c
 *
 *  Created on: Aug 18, 2025
 *      Author: MTA
 */

#include "lvgl.h"

#include "stdint.h"

#define DISPLAY_WIDTH 480U
#define DISPLAY_HEIGHT 272U
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB888))
#define BUFF_SIZE (DISPLAY_WIDTH * 10 * BYTES_PER_PIXEL)

static uint8_t draw_buff_0[BUFF_SIZE];
static uint8_t draw_buff_1[BUFF_SIZE];

void tft_init(void) {
  lv_init();

  lv_display_t *display = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lv_display_set_flush_cb(display, display_flush_cb);
  lv_display_set_buffers(display, draw_buff_0, draw_buff_1, sizeof(draw_buff_0),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
}
