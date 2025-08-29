/*
 * tft.c
 *
 *  Created on: Aug 18, 2025
 *      Author: MTA
 */

#include "lvgl.h"

#include "ft5426_touch_ic.h"
#include "main.h"
#include "stdint.h"

#define LCD_FRAME_BUFFER_ADDRESS 0xCC000000

#define DISPLAY_WIDTH 480U
#define DISPLAY_HEIGHT 272U
#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB888))
#define BUFF_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT * BYTES_PER_PIXEL)

uint8_t draw_buff_0[BUFF_SIZE] __attribute__((used, section(".sdram_data")));

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
static void display_flush_cb(lv_display_t *disp, const lv_area_t *area,
                             lv_color_t *color_p) {

  lv_display_flush_ready(disp);
}

/**
 * @brief This function gets the current touch point on the screen
 * @param input_device pointer of input_device handler.
 * @param data pinter of input device data.
 * @retval None.
 */

static void touchpad_read_cb(lv_indev_t *input_device, lv_indev_data_t *data) {
  FT5426_TouchData_t touch_data = {0U};
  ft5426_get_touch_data(&touch_data);
  data->point.x = (uint32_t)touch_data.x_cord;
  data->point.y = (uint32_t)touch_data.y_cord;
  if (FT5426_NO_EVENT != touch_data.ft5426_event) {
    data->state = LV_INDEV_STATE_PRESSED;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

/**
 * @brief This function initialize the touch for lvgl.
 * @param None.
 * @retval None.
 *
 */

static void touch_init(void) {
  lv_indev_t *input_device = lv_indev_create();
  lv_indev_set_type(input_device, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(input_device, touchpad_read_cb);
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
  HAL_LTDC_SetAddress(&hltdc, (uint32_t)draw_buff_0, 0);
  lv_display_t *display = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lv_display_set_flush_cb(display, (lv_display_flush_cb_t)display_flush_cb);
  lv_display_set_buffers(display, draw_buff_0, NULL, sizeof(draw_buff_0),
                         LV_DISPLAY_RENDER_MODE_FULL);
  touch_init();
}
