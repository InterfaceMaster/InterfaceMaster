/*
 * w25q_flash_mem.h
 *
 *  Created on: Aug 31, 2025
 *      Author: MTA
 */

#ifndef INC_W25Q_FLASH_MEM_H_
#define INC_W25Q_FLASH_MEM_H_

#include <stdint.h>

typedef enum {
  W25Q_OK = 0U,
  W25Q_ERROR = 1U,
  W25Q_TIMEOUT = 2U,
} W25Q_Status_e;

W25Q_State_e w25q_check_busy_bit(uint8_t *busy_bit_status);
W25Q_State_e w25q_page_program(const uint32_t address, const uint8_t size,
                               const uint8_t *p_data_buff);
W25Q_State_e w25q_sector_erase(const uint32_t address);

W25Q_State_e w25q_read_data(const uint32_t address, const uint16_t size,
                            uint8_t *p_data_buff);
W25Q_State_e w25q_init(void);

#endif /* INC_W25Q_FLASH_MEM_H_ */
