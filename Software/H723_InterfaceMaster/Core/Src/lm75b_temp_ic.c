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

#define I2C_ADDRESS (0x48U << 1U)

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

static void lm75b_read_register(const uint8_t reg_addr, uint8_t *data_buff,
                                uint8_t data_size) {
  if (NULL == data_buff) {
    return;
  }
  HAL_I2C_Mem_Read(&hi2c4, I2C_ADDRESS, reg_addr, I2C_MEMADD_SIZE_8BIT,
                   data_buff, data_size, I2C_TIMEOUT_100_MS);
}

/**
 * @brief This function writes data to given memory address.
 * @param reg_addr is the register address writing data to.
 * @param data_buff address of the buffer where the daya writing to.
 * @retval None.
 */

static void lm75b_write_register(const uint8_t reg_addr, uint8_t data) {
  uint8_t tx_buff[2U] = {reg_addr, data};
  HAL_I2C_Master_Transmit(&hi2c4, I2C_ADDRESS, &tx_buff[0U], sizeof(tx_buff),
                          I2C_TIMEOUT_100_MS);
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

/**
 * @brief This function initialize the LM75B temperature sensor.
 * @param None.
 * @retval None.
 */

void lm75b_init(void) {
  /*TODO Add I2C Bus Scanner if device address is not found give error*/
}
