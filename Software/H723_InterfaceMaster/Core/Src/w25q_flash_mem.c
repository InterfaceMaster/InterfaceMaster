/*
 * w25q_flash_mem.c
 *
 *  Created on: Aug 31, 2025
 *      Author: MTA
 *
 *   @note The Nor-Flash Part Number: W25Q16JVSSIQ TR
 *   @note The related link:
 * https://www.digikey.com/en/products/detail/winbond-electronics/W25Q16JVSSIQ-TR/6193775
 */
#include "w25q_flash_mem.h"
#include "main.h"
#include "spi.h"
#include <stdint.h>

#define W25Q_BUSY_TIMEOUT_MS 5U
#define W25Q_SPI_CH 1U

#define WRITE_ENABLE 0x06U
#define WRITE_DISABLE 0x04U

#define STATUS_REG_1 0x05U
#define PAGE_PROGRAM 0x02U
#define READ_DATA_REG 0x03U
#define SECTOR_ERASE 0x20U

/*
  ==============================================================================
                      ##### STATIC FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function enables the write operation.
 * @param None.
 * @retval Transmit state.
 */
static W25Q_Status_e w25q_write_enable(void) {
  HAL_StatusTypeDef status = W25Q_ERROR;
  status = IM_spi_send_command(W25Q_SPI_CH, WRITE_ENABLE);
  if (HAL_OK != status) {
    return W25Q_ERROR;
  }
  return W25Q_OK;
}

/**
 * @brief This function disables the write operation.
 * @param None.
 * @retval Transmit state.
 */

static W25Q_Status_e w25q_write_disable(void) {

  HAL_StatusTypeDef status = W25Q_ERROR;
  status = IM_spi_send_command(W25Q_SPI_CH, WRITE_DISABLE);

  if (HAL_OK != status) {
    return W25Q_ERROR;
  }
  return W25Q_OK;
}

/**
 * @brief This function reads the status register.
 * @param None.
 * @retval Read state.
 */

static W25Q_Status_e w25q_read_status_reg_1(uint8_t *p_data_buff) {

  HAL_StatusTypeDef status = W25Q_ERROR;
  uint8_t temp = STATUS_REG_1;

  IM_spi1_cs_state(SLAVE_ENABLE);
  status = HAL_SPI_Transmit(&hspi1, &temp, sizeof(temp), HAL_MAX_DELAY);

  if (HAL_OK != status) {
    return W25Q_ERROR;
  }

  status = HAL_SPI_Receive(&hspi1, p_data_buff, 1U, HAL_MAX_DELAY);

  if (HAL_OK != status) {
    return W25Q_ERROR;
  }
  IM_spi1_cs_state(SLAVE_DISABLE);
  return W25Q_OK;
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief Checks the BUSY bit of the W25Q flash memory's Status Register-1.
 * @param busy_bit_status A pointer to an 8-bit variable where the status bit
 * stored.
 * @retval Operation state.
 */
W25Q_Status_e w25q_check_busy_bit(uint8_t *busy_bit_status) {

  W25Q_Status_e status = W25Q_OK;
  uint8_t status_reg_data = 0U;

  status = w25q_read_status_reg_1(&status_reg_data);

  if (W25Q_OK != status) {
    return W25Q_ERROR;
  }

  *busy_bit_status = status_reg_data & 0x01U;

  return W25Q_OK;
}

/**
 * @brief This functions programs page of data.
 * @param address 24-bit address the memory location.
 * @param size Number of data bytes to be programmed. Must be <= 256.
 * @param p_data_buff Pointer the buffer containing data be writen
 * @retval Status of page program operation.
 */

W25Q_Status_e w25q_page_program(const uint32_t address, const uint8_t size,
                                const uint8_t *p_data_buff) {

  uint8_t busy_bit_status = 0u;
  HAL_StatusTypeDef status = HAL_OK;

  uint8_t tx_buff[4U] = {0U};

  tx_buff[0U] = PAGE_PROGRAM;
  tx_buff[1U] = (address >> 16U) & 0xFFU;
  tx_buff[2U] = (address >> 8U) & 0xFFU;
  tx_buff[3U] = address & 0xFFU;

  status = w25q_write_enable();

  if (HAL_OK != status) {
    return W25Q_ERROR;
  }

  IM_spi1_cs_state(SLAVE_ENABLE);
  status =
      HAL_SPI_Transmit(&hspi1, &tx_buff[0U], sizeof(tx_buff), HAL_MAX_DELAY);

  if (HAL_OK != status) {
    IM_spi1_cs_state(SLAVE_DISABLE);
    return W25Q_ERROR;
  }

  status = HAL_SPI_Transmit(&hspi1, p_data_buff, size, HAL_MAX_DELAY);

  if (HAL_OK != status) {
    IM_spi1_cs_state(SLAVE_DISABLE);
    return W25Q_ERROR;
  }

  IM_spi1_cs_state(SLAVE_DISABLE);

  uint32_t start_time = HAL_GetTick();

  do {
    status = w25q_check_busy_bit(&busy_bit_status);

    if (HAL_OK != status) {
      return W25Q_ERROR;
    }
    if ((HAL_GetTick() - start_time) > W25Q_BUSY_TIMEOUT_MS) {
      return W25Q_TIMEOUT;
    }

  } while (1U == busy_bit_status);

  return W25Q_OK;
}

/**
 * @brief This function delete the sector of flash memory.
 * @param address 24-bit address the memory location.
 * @param size Number of data byte to delete.
 * @retval Status of sector erase operation.
 */
W25Q_Status_e w25q_sector_erase(const uint32_t address) {

  uint8_t busy_bit_status = 0U;
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t tx_buff[4U] = {0U};

  tx_buff[0U] = SECTOR_ERASE;
  tx_buff[1U] = (address >> 16U) & 0xFFU;
  tx_buff[2U] = (address >> 8U) & 0xFFU;
  tx_buff[3U] = address & 0xFFU;

  status = w25q_write_enable();

  if (HAL_OK != status) {
    return W25Q_ERROR;
  }

  IM_spi1_cs_state(SLAVE_ENABLE);

  status =
      HAL_SPI_Transmit(&hspi1, &tx_buff[0U], sizeof(tx_buff), HAL_MAX_DELAY);

  if (HAL_OK != status) {
    return W25Q_ERROR;
  }

  IM_spi1_cs_state(SLAVE_DISABLE);

  uint32_t start_time = HAL_GetTick();

  do {
    status = w25q_check_busy_bit(&busy_bit_status);

    if (HAL_OK != status) {
      return W25Q_ERROR;
    }
    if ((HAL_GetTick() - start_time) > W25Q_BUSY_TIMEOUT_MS) {
      return W25Q_TIMEOUT;
    }
  } while (1U == busy_bit_status);

  return W25Q_OK;
}

/**
 * @brief This function reads data from a given address.
 * @param addres 24-bit address from which the starting read.
 * @param size Size of reading byte.
 * @param p_data_buff Pointer to the buffer data will be storage.
 * @retval None.
 */

W25Q_Status_e w25q_read_data(const uint32_t address, const uint16_t size,
                             uint8_t *p_data_buff) {
  HAL_StatusTypeDef status = HAL_OK;

  uint8_t tx_buff[4U] = {0U};

  tx_buff[0U] = READ_DATA_REG;
  tx_buff[1U] = (address >> 16U) & 0xFFU;
  tx_buff[2U] = (address >> 8U) & 0xFFU;
  tx_buff[3U] = address & 0xFFU;

  IM_spi1_cs_state(SLAVE_ENABLE);

  status =
      HAL_SPI_Transmit(&hspi1, &tx_buff[0U], sizeof(tx_buff), HAL_MAX_DELAY);
  if (HAL_OK != status) {
    return W25Q_ERROR;
  }

  status = HAL_SPI_Receive(&hspi1, p_data_buff, size, HAL_MAX_DELAY);
  if (HAL_OK != status) {
    return W25Q_ERROR;
  }

  IM_spi1_cs_state(SLAVE_DISABLE);

  return W25Q_OK;
}

/**
 * @brief This function initialize the nor memory.
 * @param  None
 * @retval Status of initialization.
 */

W25Q_Status_e w25q_init(void) {
  uint8_t busy_bit_status = 0U;
  W25Q_Status_e status = W25Q_OK;
  uint32_t start_time = HAL_GetTick();

  do {
    status = w25q_check_busy_bit(&busy_bit_status);

    if (W25Q_OK != status) {
      return W25Q_ERROR;
    }

    if ((HAL_GetTick() - start_time) > W25Q_BUSY_TIMEOUT_MS) {
      /*TODO: Give error*/
      return W25Q_TIMEOUT;
    }

  } while (1U == busy_bit_status);

  return W25Q_OK;
}
