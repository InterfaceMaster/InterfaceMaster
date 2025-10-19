/*
 * logger_fs.h
 *
 *  Created on: Sep 23, 2025
 *      Author: MTA
 */

#ifndef INC_LOGGER_FS_H_
#define INC_LOGGER_FS_H_

#define MAX_RDWR_DATA_SIZE 64U
#define LOGS_FILE_PATH "system/logs"
#define SYSTEM_BOTT_FILE_PATH "system/boot_data"

#include "lfs.h"
#include <stdint.h>

/**
 * @brief Forward declaration for the Communication Protocol structure.
 *
 * @details Declared as an incomplete type.This structure definition available
 * in "comminucation.h".
 */
typedef struct CommProtocol_t CommProtocol_t;

typedef enum {
  LFS_LOGGER_OK = 0U,
  LFS_LOGGER_ERROR = 1U,

} LFS_Logger_Status_e;

LFS_Logger_Status_e logger_fs_delete_file(const uint8_t *path);

LFS_Logger_Status_e logger_fs_read_file(lfs_dir_t *p_dir,
                                        const uint8_t *p_dir_name,
                                        const uint8_t *p_file_name,
                                        const uint32_t size,
                                        uint8_t *data_buffer);
LFS_Logger_Status_e
logger_fs_write_received_data(CommProtocol_t *p_comm_protocol);
LFS_Logger_Status_e logger_fs_init(void);

#endif /* INC_LOGGER_FS_H_ */
