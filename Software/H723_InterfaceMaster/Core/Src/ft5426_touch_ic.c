/*
 * ft5426_touch_ic.c
 *
 *  Created on: Aug 24, 2025
 *      Author: MTA
 */
#include "ft5426_touch_ic.h"
#include "i2c.h"
#include "main.h"

#include <stdint.h>

#define I2C_ADDRESS (0x38U << 1U)

#define WORKING_MODE_REG 0X00U
#define NUMBER_OF_TOUCH_POINT 0x02U

#define FT5426_EVENT_FLAG_MASK 0XC0U

/*
  ==============================================================================
                      ##### STATIC FUNCTIONS #####
  ==============================================================================

  */
/**
 *  @brief This function reads data from given memory address.
 * @param reg_addr is the register address to data read from.
 * @param data_buff address of the buffer where the data writing to.
 * @param data_size amaount of the read data.
 * @retval None.
 */

static void ft5426_read_register(const uint8_t reg_addr, uint8_t *data_buff,
                                 uint8_t data_size) {
  if (data_buff == NULL) {
    return;
  }
  HAL_I2C_Mem_Read(&hi2c4, (uint16_t)I2C_ADDRESS, (uint16_t)reg_addr,
                   I2C_MEMADD_SIZE_8BIT, data_buff, data_size,
                   I2C_TIMEOUT_100_MS);
}

/**
 *  @brief This function writes data to given memory address.
 * @param reg_addr is the register address writing data to.
 * @param data_buff address of the buffer where the data writing to.
 * @retval None.
 */

static void ft5426_write_register(const uint8_t reg_addr, uint8_t data) {

  uint8_t tx_buff[2U] = {reg_addr, data};

  HAL_I2C_Master_Transmit(&hi2c4, I2C_ADDRESS, &tx_buff[0U], sizeof(tx_buff),
                          I2C_TIMEOUT_100_MS);
}

/**
 * @brief This function sets the device working mode to work mode.
 * @param None.
 * @retval None.
 */

static void ft5426_set_working_mode(void) {
  ft5426_write_register(WORKING_MODE_REG, 0U);
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief Reads touch point coordinates and event from the FT5426 controller.
 * @param  touch_data Pointer to the struct where the touch data will be stored.
 * @retval None.
 * @note   This function reads a 5-byte burst from the FT5426. The data contains
 * the touch status and the X and Y coordinates of the first touch point.
 */

void ft5426_get_touch_data(FT5426_TouchData_t *touch_data) {
  uint8_t touch_points = 0U;
  uint8_t data_buff[5U];
  uint8_t data_size = sizeof(data_buff) / sizeof(data_buff[0U]);

  ft5426_read_register(NUMBER_OF_TOUCH_POINT, data_buff, data_size);

  touch_points = data_buff[0U] & 0X0FU;
  if (touch_points >= 1U) {
    touch_data->ft5426_event = (data_buff[1U] & 0xC0U) >> 6U;
    touch_data->x_cord = ((data_buff[1U] & 0x0FU) << 8U) | data_buff[2U];
    touch_data->y_cord = ((data_buff[3U] & 0x0FU) << 8U) | data_buff[4U];
  }
}

/**
 * @brief This function initialize the FT5426 touch ic.
 * @param None.
 * @retval None.
 */

void ft5426_init(void) {
  /*TODO Add bus scanner. If IC address note found give error.*/
  ft5426_set_working_mode();
}
