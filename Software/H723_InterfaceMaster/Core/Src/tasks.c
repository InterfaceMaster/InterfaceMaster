/**
 * Copyright (c) 2025 InterfaceMaster organization.
 * This work is licensed under a Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License. To view a
 * copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/
 */

/*
 * tasks.c
 *
 *  Created on: Aug 3, 2025
 *      Author: MTA
 */

#include <stdint.h>

#include "adc.h"
#include "bdma.h"
#include "board_options.h"
#include "communication.h"
#include "dma.h"
#include "fdcan.h"
#include "fmc.h"
#include "gpio.h"
#include "i2c.h"
#include "lm75b_temp_ic.h"
#include "ltdc.h"
#include "main.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tasks.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "usb_host.h"

#include "ft5426_touch_ic.h"
#include "lvgl.h"
#include "tft.h"
/*
  ==============================================================================
                      ##### STATIC FUNCTION PROTOTYPE #####
  ==============================================================================

  */

static void s_gui_task_CB(void);
static void s_usb_task_CB(void);

/**
 * @brief  This structure define task config parameters.
 * @details This structure holds periodic execution of task and time tracking.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef struct {
  void (*pTask_CB)(void);
  uint32_t u32_start_time;
  uint32_t u32_last_run_time;
  uint32_t u32_elapsed_time;
  uint32_t u32_period;
} TaskConfig_t;

/*
  ==============================================================================
                      ##### STATIC VARIABLE DECLARATIONS #####
  ==============================================================================

  */
static uint8_t s_u8_comm_rx_data_buff_0[MAX_COMM_PROTOCOL_SIZE] = {0U};
static uint8_t s_u8_comm_rx_data_buff_1[MAX_COMM_PROTOCOL_SIZE] = {0U};
static uint8_t s_u8_comm_tx_data_buff[MAX_COMM_PROTOCOL_SIZE] = {0U};

static SystemInstance_t s_gSystem = {0U};
static TaskConfig_t s_tasks_config[3U] = {0U};
static uint8_t s_u8_usb_task_period_ms = 10U;
static uint8_t s_u8_gui_task_period_ms = 100U;
static uint8_t s_u8_comm_protocol_task_ms = 5U;

/*
  ==============================================================================
                      ##### STATIC FUNCTION IMPLEMENTATIONS #####
  ==============================================================================

  */

/**
 *@brief This function is communication protocol task.
 *@param None.
 *@retVal None.
 */

static void s_comm_protocol_task_CB(void) {}

/**
 *@brief This function is UI task callback.
 *@param None.
 *@retVal None.
 */

static void s_gui_task_CB(void) { lv_task_handler(); }

/**
 *@brief This function is USB task callback.
 *@param None.
 *@retVal None.
 */
static void s_usb_task_CB(void) { USB_send_data(); }

/**
 * @brief  This function initializes the MCU USB Device or USB Host based on ID
 * 		   pin.
 * @param None.
 * @retval None.
 */

static void USB_init(void) {
  USB_InitType_e usb_init_type = get_usb_ID_state();

  switch (usb_init_type) {
  case USB_ID_DEVICE:
    MX_USB_DEVICE_Init();
    break;
  case USB_ID_HOST:
    MX_USB_HOST_Init();
    break;
  default:
    MX_USB_DEVICE_Init();
    break;
  }
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief  This function initializes the Interface Master peripherals.
 * @note   This function configures and initializes various peripherals on
 * the STM32H723 based PCB. It should be called once at the start of the
 * application.
 * @param None.
 * @retval None.
 */
void IM_peripheral_init(void) {
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_BDMA_Init();
  MX_ADC3_Init();
  MX_UART5_Init();
  MX_FDCAN1_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_I2C4_Init();
  MX_LTDC_Init();
  MX_RTC_Init();
  MX_SDMMC2_SD_Init();
  MX_SPI2_Init();
  MX_UART7_Init();
  MX_USART2_UART_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();

  USB_init();
}

/**
 * @bried Initialize the system librarys.
 * @param None.
 * @retval None.
 * */

void IM_system_init(void) {
  logger_fs_init();

  lm75b_init();
  ft5426_init();
  tft_init();
  lv_init();

  s_gSystem.comm_protocol.p_rx_buff_0 = &s_u8_comm_rx_data_buff_0[0U];
  s_gSystem.comm_protocol.p_rx_buff_1 = &s_u8_comm_rx_data_buff_1[0U];

  s_gSystem.comm_protocol.p_tx_buff = &s_u8_comm_tx_data_buff[0U];

  s_gSystem.comm_protocol.active_buff = COMM_ACTIVE_BUFF_0;
  s_gSystem.comm_protocol.type = COMM_PROTOCOL_TYPE_UART;
  s_gSystem.comm_protocol.rx_status = COMM_STATUS_IDLE;
  s_gSystem.comm_protocol.tx_status = COMM_STATUS_IDLE;

  init_comm_protocol_handler(&s_gSystem.comm_protocol);
}

/**
 * @brief Initializes the system task configurations.
 * @param None.
 * @retval None.
 */
void IM_task_init(void) {

  s_tasks_config[0U].pTask_CB = &s_usb_task_CB;
  s_tasks_config[0U].u32_period = s_u8_usb_task_period_ms;

  s_tasks_config[1U].pTask_CB = &s_gui_task_CB;
  s_tasks_config[1U].u32_period = s_u8_gui_task_period_ms;

  s_tasks_config[2U].pTask_CB = &s_comm_protocol_task_CB;
  s_tasks_config[2U].u32_period = s_u8_comm_protocol_task_ms;
};

/**
 * @brief Initializes the system task configurations.
 * @param None.
 * @retval None.
 */
void IM_run_tasks(void) {

  uint32_t u32_current_time = HAL_GetTick();
  uint8_t task_amaount = sizeof(s_tasks_config) / sizeof(s_tasks_config[0U]);

  for (uint8_t i = 0U; i < task_amaount; ++i) {
    if (u32_current_time - s_tasks_config[i].u32_last_run_time >
        s_tasks_config[i].u32_period) {
      s_tasks_config[i].u32_last_run_time = u32_current_time;
      s_tasks_config[i].u32_start_time = HAL_GetTick();
      s_tasks_config[i].pTask_CB();
      s_tasks_config[i].u32_elapsed_time =
          HAL_GetTick() - s_tasks_config[i].u32_start_time;
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2) {
    lv_tick_inc(10U);
  }
}
