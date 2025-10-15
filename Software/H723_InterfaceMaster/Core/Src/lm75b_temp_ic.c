/*
 * lm75b_temp_ic.c
 *
 *  Created on: Sep 29, 2025
 *      Author: MTA
 */
#include "lm75b_temp_ic.h"
#include "i2c.h"
#include "main.h"

#include <stdint.h>

#define I2C_ADDRESS 0x48U

#define TEMPERATURE_REG 0x00U
#define CONFIGURATION_REG 0x01U
#define OVER_TEMP_SHUTDOWN_REG 0x03U
#define HYSTERESIS_REG 0x02U

/*
  ==============================================================================
                      ##### STATIC FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function reads data from given memory address
 * @param reg_addr is the register address to data read from.
 * @param data_buff address of the buffer where the data writing to.
 * @param data size size of the read data.
 * @retval None.
 */

static LM75B_Status_e lm75b_read_register(const uint8_t reg_addr,
                                          uint8_t *data_buff,
                                          uint8_t data_size) {
  HAL_StatusTypeDef status;

  if (NULL == data_buff) {
    return LM75B_ERROR;
  }

  status = HAL_I2C_Mem_Read(&hi2c4, (I2C_ADDRESS << 1U), reg_addr,
                            I2C_MEMADD_SIZE_8BIT, data_buff, data_size,
                            I2C_TIMEOUT_100_MS);
  if (HAL_OK != status) {
    return LM75B_ERROR;
  }

  return LM75B_OK;
}

/**
 * @brief This function writes data to given memory address.
 * @param reg_addr is the register address writing data to.
 * @param data_buff address of the buffer where the daya writing to.
 * @retval None.
 */

static LM75B_Status_e lm75b_write_register(const uint8_t reg_addr,
                                           uint8_t data) {
  HAL_StatusTypeDef status;
  uint8_t tx_buff[2U] = {reg_addr, data};
  status = HAL_I2C_Master_Transmit(&hi2c4, (I2C_ADDRESS << 1U), &tx_buff[0U],
                                   sizeof(tx_buff), I2C_TIMEOUT_100_MS);

  if (HAL_OK != status) {
    return LM75B_ERROR;
  }

  return LM75B_OK;
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function gets the temperature.
 * @param
 * @retval
 */
LM75B_Status_e lm75b_get_ambient_temp(double *buff) {

  LM75B_Status_e status;
  uint8_t temp_buff[2U];
  uint16_t raw_temp = 0U;

  if (NULL == buff) {
    return LM75B_ERROR;
  }

  status = lm75b_read_register(TEMPERATURE_REG, temp_buff, sizeof(temp_buff));

  if (LM75B_ERROR == status) {
    return LM75B_ERROR;
  }

  raw_temp = (uint16_t)((temp_buff[0U] << 8U) | temp_buff[1U]);
  raw_temp >>= 5U;

  *buff = (raw_temp * 0.125F);

  return LM75B_OK;
}
/**
 * @brief This function initialize the LM75B temperature sensor.
 * @param None.
 * @retval None.
 */

LM75B_Status_e lm75b_init(void) {

  I2C_DeviceStatus_e status;
  status = IM_I2C_bus_scanner(I2C_ADDRESS);

  if ((uint8_t)LM75B_OK != status) {
    return LM75B_ERROR;
  }

  return LM75B_OK;
}
