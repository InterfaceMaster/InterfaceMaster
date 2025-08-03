/*
 * tasks.c
 *
 *  Created on: Aug 3, 2025
 *      Author: MTA
 */

#include "main.h"
#include "fdcan.h"
#include "i2c.h"
#include "ltdc.h"
#include "rtc.h"
#include "sdmmc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fmc.h"
#include <stdint.h>

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
  * @attention Any modification to this structure requires analysis the entire system.
  */

typedef struct{
	void (*pTask_CB)(void);
	uint32_t u32_start_time;
	uint32_t u32_end_time;
	uint32_t u32_elapsed_time;
	uint8_t u8_fired;
	uint8_t u8_period;
}TaskConfig_t;

/**
  * @brief  This structure holds the entire system status information and configuration.
  * @details This structure centralizes data related the current state of all tasks and sensors.
  * @attention Any modification to this structure requires analysis the entire system.
  */

typedef struct{

}SystemInstance_t;

/*
  ==============================================================================
                      ##### STATIC VARIABLE DECLARATIONS #####
  ==============================================================================

  */
static TaskConfig_t s_usb_task_config;
/*
  ==============================================================================
                      ##### STATIC FUNCTION IMPLEMENTATIONS #####
  ==============================================================================

  */



/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
  * @brief  Initializes the Interface Master peripherals.
  * @note   This function configures and initializes various peripherals on the STM32H723 based PCB.
  * It should be called once at the start of the application.
  * @param  None.
  * @retval None.
  */
void IM_peripheral_init(void){
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
}


/**
 * @brief Initializes the system task configurations.
 * @param None.
 * @retVal None.
 */
void IM_task_init(void){};


