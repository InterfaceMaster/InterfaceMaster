/*
 * logger_fs.c
 *
 *  Created on: Sep 23, 2025
 *      Author: MTA
 *
 * @note Advantage of little fs is wear leveling.
 * https://os.mbed.com/blog/entry/littlefs-high-integrity-embedded-fs/
 */

#include "logger_fs.h"
#include "communication.h"
#include "lfs.h"
#include "main.h"
#include "w25q_flash_mem.h"

#define LFS_SUCCES 0
#define LFS_FAIL -1
#define LOGGER_FS_BUSY_TIMEOUT_MS 2U

#define UART_DATA_LOG_DIR "UART_DATA_DIR"
#define SPI_DATA_LOG_DIR "SPI_DATA_DIR"
#define I2C_DATA_LOG_DIR "I2C_DATA_DIR"
#define CAN_DATA_LOG_DIR "CAN_DATA_DIR"

#define UART_DATA_LOG_FILE "UART_DATA_FILE"
#define SPI_DATA_LOG_FILE "SPI_DATA_FILE"
#define I2C_DATA_LOG_FILE "I2C_DATA_FILE"
#define CAN_DATA_LOG_FILE "CAN_DATA_FILE"

/*
  ==============================================================================
                      ##### STATIC VARIABLES #####
  ==============================================================================

  */

static lfs_t s_logger_file_system;
static lfs_file_t s_data_log_file;

static lfs_dir_t s_uart_data_dir;
static lfs_dir_t s_spi_data_dir;
static lfs_dir_t s_i2c_data_dir;
static lfs_dir_t s_can_data_dir;

/*
  ==============================================================================
                      ##### STATIC FUNCTIONS #####
  ==============================================================================

  */

static int s_logger_fs_data_read(const struct lfs_config *c, lfs_block_t block,
                                 lfs_off_t off, void *buffer, lfs_size_t size);

static int s_logger_fs_block_erase(const struct lfs_config *c,
                                   lfs_block_t block);

static int s_logger_fs_page_program(const struct lfs_config *c,
                                    lfs_block_t block, lfs_off_t off,
                                    const void *buffer, lfs_size_t size);

static int s_logger_fs_sync(const struct lfs_config *c);

const struct lfs_config config = {
    .read = s_logger_fs_data_read,
    .prog = s_logger_fs_page_program,
    .erase = s_logger_fs_block_erase,
    .sync = s_logger_fs_sync,

    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 512,
    .cache_size = 256,
    .lookahead_size = 16,
    .block_cycles = 500,
};

static int s_logger_fs_data_read(const struct lfs_config *cfg,
                                 lfs_block_t block, lfs_off_t off, void *buffer,
                                 lfs_size_t size) {

  int logger_status = LFS_SUCCES;
  uint32_t address = (block * cfg->block_size) + off;

  W25Q_Status_e status =
      w25q_read_data(address, (uint16_t)size, (uint8_t *)buffer);

  logger_status = (W25Q_OK == status) ? LFS_SUCCES : LFS_FAIL;

  return logger_status;
}

static int s_logger_fs_block_erase(const struct lfs_config *cfg,
                                   lfs_block_t block) {

  int logger_status = LFS_SUCCES;

  W25Q_Status_e status = w25q_sector_erase(block);

  logger_status = (W25Q_OK == status) ? LFS_SUCCES : LFS_FAIL;

  return logger_status;
}

static int s_logger_fs_page_program(const struct lfs_config *cfg,
                                    lfs_block_t block, lfs_off_t off,
                                    const void *buffer, lfs_size_t size) {

  int logger_status = LFS_SUCCES;
  uint32_t address = (block * cfg->block_size) + off;

  W25Q_Status_e status =
      w25q_page_program(address, (uint16_t)size, (uint8_t *)buffer);

  logger_status = (W25Q_OK == status) ? LFS_SUCCES : LFS_FAIL;

  return logger_status;
}

static int s_logger_fs_sync(const struct lfs_config *cfg) {
  uint8_t busy_bit_status = 0U;
  W25Q_Status_e status = W25Q_OK;
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

/**
 * @brief This function writes the related data to given path.
 * @param  p_dir Address of directory handler.
 * @param  p_dir_name Address of directory name string.
 * @param  p_file Address of file name string.
 * @param  p_data_buffer Address of data to be write.
 * @retval Status of write success.
 */

static LFS_Logger_Status_e s_logger_fs_write_file(lfs_dir_t *p_dir,
                                                  const uint8_t *p_dir_name,
                                                  const uint8_t *p_file_name,
                                                  const uint8_t *p_data_buffer,
                                                  const uint32_t size) {

  if ((NULL == p_file_name) || (NULL == p_data_buffer) || (0U == size)) {
    return LFS_LOGGER_ERROR;
  }

  int status = LFS_ERR_OK;
  int writen_byte = 0;

  status = lfs_dir_open(&s_logger_file_system, p_dir, (const char *)p_dir_name);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  status = lfs_file_open(&s_logger_file_system, &s_data_log_file,
                         (const char *)p_file_name,
                         LFS_O_RDWR | LFS_O_CREAT | LFS_O_APPEND);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  writen_byte = lfs_file_write(&s_logger_file_system, &s_data_log_file,
                               p_data_buffer, size);

  if (writen_byte != size) {
    lfs_file_close(&s_logger_file_system, &s_data_log_file);
    return LFS_LOGGER_ERROR;
  }

  status = lfs_file_close(&s_logger_file_system, &s_data_log_file);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  status = lfs_dir_close(&s_logger_file_system, p_dir);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  return LFS_LOGGER_OK;
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function deletes the file.
 * @param  p_path Address of directory path string.
 * @retval Status of file delete success.
 */

LFS_Logger_Status_e logger_fs_delete_file(const uint8_t *p_path) {

  if (NULL == p_path) {
    return LFS_LOGGER_ERROR;
  }

  int status = lfs_remove(&s_logger_file_system, (const char *)p_path);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }
  return LFS_LOGGER_OK;
}

/**
 * @brief This function reads the file.
 * @param  p_dir Address of directory handler.
 * @param  p_dir_name Address of directory name string.
 * @param  p_file_name Address of file name string.
 * @param  p_data_buffer Address of buffer data to be write.
 * @retval Status of read success.
 */

LFS_Logger_Status_e logger_fs_read_file(lfs_dir_t *p_dir,
                                        const uint8_t *p_dir_name,
                                        const uint8_t *p_file_name,
                                        const uint32_t size,
                                        uint8_t *data_buffer) {

  if ((NULL == p_file_name) || (NULL == data_buffer) || (0U == size)) {
    return LFS_LOGGER_ERROR;
  }
  int status = LFS_ERR_OK;
  int readed_byte = 0;

  status = lfs_dir_open(&s_logger_file_system, p_dir, (const char *)p_dir_name);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  status = lfs_file_open(&s_logger_file_system, &s_data_log_file,
                         (const char *)p_file_name, LFS_O_RDONLY);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  readed_byte =
      lfs_file_read(&s_logger_file_system, &s_data_log_file, data_buffer, size);

  if (readed_byte < 0) {
    lfs_file_close(&s_logger_file_system, &s_data_log_file);
    return LFS_LOGGER_ERROR;
  }

  status = lfs_file_close(&s_logger_file_system, &s_data_log_file);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  status = lfs_dir_close(&s_logger_file_system, p_dir);
  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  return LFS_LOGGER_OK;
}

/**
 *@brief This function writes received data to NOR flash.
 *@param comm_protocol Communication protocol handler pointer.
 *@retVal status Status of write operation.

 */

LFS_Logger_Status_e
logger_fs_write_received_data(CommProtocol_t *p_comm_protocol) {

  LFS_Logger_Status_e status = LFS_LOGGER_OK;

  if (NULL == p_comm_protocol) {
    return LFS_LOGGER_ERROR;
  }

  flush_comm_prtocol_tx_buff();

  switch (p_comm_protocol->type) {
  case COMM_PROTOCOL_TYPE_UART:
    status = s_logger_fs_write_file(
        &s_uart_data_dir, UART_DATA_LOG_DIR, UART_DATA_LOG_FILE,
        &p_comm_protocol->p_tx_buff[0U], MAX_USB_PROTOCOL_SIZE);
    break;
  case COMM_PROTOCOL_TYPE_I2C:
    status = s_logger_fs_write_file(
        &s_i2c_data_dir, I2C_DATA_LOG_DIR, I2C_DATA_LOG_FILE,
        &p_comm_protocol->p_tx_buff[0U], MAX_USB_PROTOCOL_SIZE);
    break;
  case COMM_PROTOCOL_TYPE_SPI:
    status = s_logger_fs_write_file(
        &s_uart_data_dir, SPI_DATA_LOG_DIR, SPI_DATA_LOG_FILE,
        &p_comm_protocol->p_tx_buff[0U], MAX_USB_PROTOCOL_SIZE);
    break;
  case COMM_PROTOCOL_TYPE_CAN:
    status = s_logger_fs_write_file(
        &s_can_data_dir, CAN_DATA_LOG_DIR, CAN_DATA_LOG_FILE,
        &p_comm_protocol->p_tx_buff[0U], MAX_USB_PROTOCOL_SIZE);
    break;
  default:
    return LFS_LOGGER_ERROR;
    break;
  }

  if (LFS_LOGGER_ERROR = status) {
    return LFS_LOGGER_ERROR;
  }
  return LFS_LOGGER_OK;
}

/**
 * @brief This function initialize the logger file system.
 * @param  None.
 * @retval Status of initialization success.
 */

LFS_Logger_Status_e logger_fs_init(void) {

  W25Q_Status_e w25q_status = W25Q_OK;

  w25q_status = w25q_init();

  if (W25Q_OK != w25q_status) {
    return LFS_LOGGER_ERROR;
  }

  int status = lfs_mount(&s_logger_file_system, &config);

  if (LFS_ERR_NOENT == status) {
    status = lfs_format(&s_logger_file_system, &config);

    if (LFS_ERR_OK != status) {
      return LFS_LOGGER_ERROR;
    }

    status = lfs_mount(&s_logger_file_system, &config);
  } else {
    /*TODO: File system error give warning and take permission for memory
     * format*/
  }

  status = lfs_mkdir(&s_logger_file_system, UART_DATA_LOG_DIR);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  status = lfs_mkdir(&s_logger_file_system, SPI_DATA_LOG_DIR);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  status = lfs_mkdir(&s_logger_file_system, I2C_DATA_LOG_DIR);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  status = lfs_mkdir(&s_logger_file_system, CAN_DATA_DIR);

  if (LFS_ERR_OK != status) {
    return LFS_LOGGER_ERROR;
  }

  return LFS_LOGGER_OK;
}
