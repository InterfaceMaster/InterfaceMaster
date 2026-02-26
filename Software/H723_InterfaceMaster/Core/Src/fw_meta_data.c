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
