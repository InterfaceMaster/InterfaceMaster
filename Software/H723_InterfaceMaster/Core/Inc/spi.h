/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    spi.h
 * @brief   This file contains all the function prototypes for
 *          the spi.c file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;

extern SPI_HandleTypeDef hspi2;

/* USER CODE BEGIN Private defines */

typedef enum {
  SLAVE_IDLE = 0U,
  SLAVE_ENABLE = 1U,
  SLAVE_DISABLE = 2U,
} SPI1_CS_State_e;

/* USER CODE END Private defines */

void MX_SPI1_Init(void);
void MX_SPI2_Init(void);

/* USER CODE BEGIN Prototypes */
void IM_spi1_cs_state(const SPI1_CS_State_e cs_state);
HAL_StatusTypeDef IM_spi_send_command(const uint8_t spi_ch, const uint8_t reg);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
