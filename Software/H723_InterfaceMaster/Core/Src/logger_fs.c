/*
 * logger_fs.c
 *
 *  Created on: Sep 23, 2025
 *      Author: MTA
 */

#include "logger_fs.h"
#include "lfs.h"
#include "main.h"
#include "w25q_flash_mem.h"

#define LFS_SUCCES 0
#define LFS_FAIL -1
#define LOGGER_FS_BUSY_TIMEOUT_MS 2U

lfs_t lfs;
lfs_file_t data_log_file;
/*
  ==============================================================================
                      ##### STATIC FUNCTIONS #####
  ==============================================================================

  */

static int logger_fs_data_read(const struct lfs_config *c, lfs_block_t block,
                               lfs_off_t off, void *buffer, lfs_size_t size);

static int logger_fs_block_erase(const struct lfs_config *c, lfs_block_t block);

static int logger_fs_page_program(const struct lfs_config *c, lfs_block_t block,
                                  lfs_off_t off, const void *buffer,
                                  lfs_size_t size);

static int logger_fs_sync(const struct lfs_config *c);

const struct lfs_config cfg = {
    .read = logger_fs_data_read,
    .prog = logger_fs_page_program,
    .erase = logger_fs_block_erase,
    .sync = logger_fs_sync,

    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 512,
    .cache_size = 256,
    .lookahead_size = 16,
    .block_cycles = 500,
};

static int logger_fs_data_read(const struct lfs_config *cfg, lfs_block_t block,
                               lfs_off_t off, void *buffer, lfs_size_t size) {

  int logger_status = LFS_SUCCES;
  uint32_t address = (block * cfg->block_size) + off;

  W25Q_State_e status =
      w25q_read_data(address, (uint16_t)size, (uint8_t *)buffer);

  logger_status = (W25Q_OK == status) ? LFS_SUCCES : LFS_FAIL;

  return logger_status;
}

static int logger_fs_block_erase(const struct lfs_config *cfg,
                                 lfs_block_t block) {

  int logger_status = LFS_SUCCES;

  W25Q_State_e status = w25q_sector_erase(block);

  logger_status = (W25Q_OK == status) ? LFS_SUCCES : LFS_FAIL;

  return logger_status;
}

static int logger_fs_page_program(const struct lfs_config *cfg,
                                  lfs_block_t block, lfs_off_t off,
                                  const void *buffer, lfs_size_t size) {

  int logger_status = LFS_SUCCES;
  uint32_t address = (block * cfg->block_size) + off;

  W25Q_State_e status =
      w25q_page_program(address, (uint16_t)size, (uint8_t *)buffer);

  logger_status = (W25Q_OK == status) ? LFS_SUCCES : LFS_FAIL;

  return logger_status;
}

static int logger_fs_sync(const struct lfs_config *cfg) {
  uint8_t busy_bit_status = 0U;
  W25Q_State_e status = W25Q_OK;
  uint32_t start_time = HAL_GetTick();
  do {

    status = w25q_check_busy_bit(&busy_bit_status);

    if (W25Q_OK != status) {
      return -1;
    }
    if ((HAL_GetTick() - start_time) > LOGGER_FS_BUSY_TIMEOUT_MS) {
      return -1;
    }
  } while (1U == busy_bit_status);

  return 0;
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

LFS_Logger_Status_e logger_fs_take_log(const uint8_t *path,
                                       const uint8_t *data_buffer,
                                       const uint32_t size) {

  LFS_Logger_Status_e logger_fs_status = LFS_LOGGER_OK;

  if ((NULL == path) || (NULL == data_buffer) || (0U == size)) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  int status = LFS_ERR_OK;
  int writen_byte = 0;

  status = lfs_file_open(&lfs, &data_log_file, (const char *)path,
                         LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND);

  if (LFS_ERR_OK != status) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  writen_byte = lfs_file_write(&lfs, &data_log_file, data_buffer, size);

  if (writen_byte != size) {
    lfs_file_close(&lfs, &data_log_file);
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  status = lfs_file_close(&lfs, &data_log_file);
  if (LFS_ERR_OK != status) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  return logger_fs_status;
}

LFS_Logger_Status_e logger_fs_delete_file(const uint8_t *path) {

  LFS_Logger_Status_e logger_fs_status = LFS_LOGGER_OK;
  if (NULL == path) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  int status = lfs_remove(&lfs, path);

  if (LFS_ERR_OK != status) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }
  return logger_fs_status;
}

LFS_Logger_Status_e logger_fs_read_file(const uint8_t *path,
                                        const uint32_t size,
                                        uint8_t *data_buffer) {

  LFS_Logger_Status_e logger_fs_status = LFS_LOGGER_OK;
  if ((NULL == path) || (NULL == data_buffer) || (0U == size)) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }
  int status = LFS_ERR_OK;
  int readed_byte = 0;

  status = lfs_file_open(&lfs, &data_log_file, path, LFS_O_RDONLY);

  if (LFS_ERR_OK != status) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  readed_byte = lfs_file_read(&lfs, &data_log_file, data_buffer, size);

  if (readed_byte < 0) {
    lfs_file_close(&lfs, &data_log_file);
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  status = lfs_file_close(&lfs, &data_log_file);

  if (LFS_ERR_OK != status) {
    logger_fs_status = LFS_LOGGER_ERROR;
    return logger_fs_status;
  }

  return logger_fs_status;
}

LFS_Logger_Status_e logger_fs_init(void) {

  LFS_Logger_Status_e logger_fs_status = LFS_LOGGER_OK;
  int status = lfs_mount(&lfs, &cfg);

  if (LFS_ERR_NOENT == status) {
    status = lfs_format(&lfs, &cfg);

    if (LFS_ERR_OK != status) {
      return LFS_LOGGER_ERROR;
    }

    status = lfs_mount(&lfs, &cfg);
  } else {
    /*TODO: File system error give warning and take permission for memory
     * format*/
  }

  logger_fs_status = (LFS_ERR_OK == status) ? LFS_LOGGER_OK : LFS_LOGGER_ERROR;

  return logger_fs_status;
}
