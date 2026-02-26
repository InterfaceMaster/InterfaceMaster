/*
 * fw_meta_data.c
 *
 *  Created on: 26 Şub 2026
 *      Author: MTA
 */

/*
  ==============================================================================
                      ##### USER DEFINE TYPES #####
  ==============================================================================

  */

#include "main.h"

extern uint32_t _app_code_start_addr;
extern uint32_t _app_code_end_addr;

typedef struct __attribute__((packed)) _FirmwareMetadata_t {
  uint32_t u32_magic_number;
  uint8_t u8_sha_256[32U];
  uint32_t u32_app_start_addr;
  uint32_t u32_app_end_addr;
  uint32_t u8_fw_version[2U];
  uint8_t u8_hw_version[2U];
  uint8_t u8_app_type;
  char c_build_date[12U];
  char c_build_time[9U];
  uint32_t u32_resereved[6U];
} FirmwareMetadata_t;

const FirmwareMetadata_t app_metadata
    __attribute__((section(".fw_metadata_section"), used)) = {
        .u32_magic_number = 0x4F594D41U,

        .u8_sha_256 = {0U},

        .u32_app_start_addr = (uint32_t)&_app_code_start_addr,

        .u32_app_end_addr = (uint32_t)&_app_code_end_addr,

        .u8_fw_version = {FW_VERSION_MAJOR, FW_VERSION_MINOR},

        .u8_hw_version = {HW_VERSION_MAJOR, HW_VERSION_MINOR},

        .u8_app_type = APP_TYPE,

        .c_build_date = __DATE__,

        .c_build_time = __TIME__,
};

/**
 * @brief This function writes metadata of firmware to flash.
 * @attention Any modification in this function not recommended.
 * @param None.
 * @retVal Status of operation.
 */

HAL_StatusTypeDef write_fw_metadata_to_flash(void) {
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t flash_dst_addr = app_metadata.u32_app_start_addr;
  uint32_t src_data_ptr = (uint32_t)&app_metadata;

  status = HAL_FLASH_Unlock();
  if (status != HAL_OK)
    return status;

  FLASH_EraseInitTypeDef flash_erase_init = {0};
  uint32_t sector_error = 0U;

  flash_erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;
  flash_erase_init.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  flash_erase_init.Sector = FLASH_SECTOR_1;
  flash_erase_init.NbSectors = 1U;
  flash_erase_init.Banks = FLASH_BANK_1;

  status = HAL_FLASHEx_Erase(&flash_erase_init, &sector_error);

  if (status == HAL_OK) {

    for (uint32_t i = 0U; i < sizeof(FirmwareMetadata_t); i += 32U) {

      status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, flash_dst_addr,
                                 (src_data_ptr + i));

      if (status == HAL_OK) {
        flash_dst_addr += 32U;
      } else {
        break;
      }
    }
  }

  HAL_FLASH_Lock();
  return status;
}
