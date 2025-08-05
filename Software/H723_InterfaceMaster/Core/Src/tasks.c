/*
 * tasks.c
 *
 *  Created on: Aug 3, 2025
 *      Author: MTA
 */

#include <stdint.h>

#include "board_options.h"
#include "communication.h"
#include "tasks.h"

#include "fdcan.h"
#include "fmc.h"
#include "gpio.h"
#include "i2c.h"
#include "ltdc.h"
#include "main.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "usb_host.h"

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
  uint16_t u32_start_time;
  uint16_t u32_last_run_time;
  uint16_t u32_elapsed_time;
  uint32_t u32_period;
  uint8_t u8_fired;

} TaskConfig_t __attribute__((aligned(32U)));

/**
 * @brief  This structure holds the entire system status information and
 * configuration.
 * @details This structure centralizes data related the current state of all
 * tasks and sensors.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef struct {
  CommProtocol_t comm_protocol;
} SystemInstance_t;

/*
  ==============================================================================
                      ##### STATIC VARIABLE DECLARATIONS #####
  ==============================================================================

  */
static TaskConfig_t s_usb_task_config;
static uint8_t s_u8_usb_task_period_ms = 10U;

/*
  ==============================================================================
                      ##### STATIC FUNCTION IMPLEMENTATIONS #####
  ==============================================================================

  */

/**
 * @brief  This function initializes the MCU USB Device or USB Host based on ID
 * 		   pin.
 * @param None.
 * @retval None.
 */

static void usb_init(void) {
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
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();
  usb_init();
}

/**
 * @brief Initializes the system task configurations.
 * @param None.
 * @retval None.
 */
void IM_task_init(void) {

  s_usb_task_config.pTask_CB = &s_usb_task_CB;
  s_usb_task_config.u8_period = s_u8_usb_task_period_ms;
  s_usb_task_config.u8_fired = 1U;
};

/**
 * @brief Initializes the system task configurations.
 * @param None.
 * @retval None.
 */

void run_tasks(void) {

  uint32_t u32_current_time = HAL_GetTick();

  if (u32_current_time - s_usb_task_config.u32_last_run_time >
      s_usb_task_config.u32_period) {
    s_usb_task_config.u32_last_run_time = u32_current_time;
    s_usb_task_config.u32_start_time = HAL_GetTick();
    s_usb_task_config.pTask_CB();
    s_usb_task_config.u32_elapsed_time =
        HAL_GetTick() - s_usb_task_config.u32_start_time;
  }
}
