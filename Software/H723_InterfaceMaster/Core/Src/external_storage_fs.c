/*
 * external_storage_fs.c
 *
 *  Created on: Oct 29, 2025
 *      Author: MTA
 */

#include "external_storage_fs.h"
#include "communication.h"
#include "fatfs.h"
#include "logger_fs.h"
#include "tasks.h"

/*
  ==============================================================================
                      ##### STATIC VARIABLE DECLARATIONS #####
  ==============================================================================

  */

uint8_t s_u8_logger_fs_rx_data_buff[65U * 1024U]
    __attribute__((used, section(".sdram_data")));

static CommProtocol_t *s_p_comm_protocol;

static FIL s_uart_data_file;
static FIL s_spi_data_file;
static FIL s_i2c_data_file;
static FIL s_can_data_file;

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 *@brief This function fills the buffer with related data.
 *@param p_data Pointer of to be written data.
 *@param size Amaount of to be written data.
 *@retVal status Status of write operation.
 */

void IM_USB_FatFS_fill_instance(CommProtocol_t *p_comm_protocol) {

  if (NULL != p_comm_protocol) {
    return;
  }

  s_p_comm_protocol = p_comm_protocol;
}

/**
 *@brief This function writes data to the external flash disk.
 *@param None.
 *@retVal None.
 */

USBH_FatFS_Status_e IM_USB_FatFS_write_data(void) {

  LFS_Logger_Status_e logger_fs_status = LFS_LOGGER_OK;

  FRESULT fat_fs_status = FR_OK;

  UINT file_size = 0U;
  UINT writed_byte_size = 0U;

  logger_fs_status =
      logger_fs_get_file_size(s_p_comm_protocol, (uint32_t *)&file_size);

  if (LFS_LOGGER_OK != logger_fs_status) {
    return USB_FatFS_ERROR;
  }

  logger_fs_status = logger_fs_read_file(s_p_comm_protocol, file_size,
                                         &s_u8_logger_fs_rx_data_buff[0U]);

  if (LFS_LOGGER_OK != logger_fs_status) {
    return USB_FatFS_ERROR;
  }

  switch (s_p_comm_protocol->type) {

  case COMM_PROTOCOL_TYPE_UART:

    fat_fs_status = f_open(&s_uart_data_file, "UART_DATA.txt ",
                           FA_CREATE_ALWAYS | FA_WRITE);

    if (FR_OK != fat_fs_status) {
      f_close(&s_uart_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_write(&s_uart_data_file, &s_u8_logger_fs_rx_data_buff[0U],
                            file_size, &writed_byte_size);

    if ((FR_OK != fat_fs_status) || (file_size != writed_byte_size)) {
      f_close(&s_uart_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_close(&s_uart_data_file);

    if (FR_OK != fat_fs_status) {
      return USB_FatFS_ERROR;
    }

    break;

  case COMM_PROTOCOL_TYPE_I2C:

    fat_fs_status =
        f_open(&s_i2c_data_file, "I2C_DATA.txt", FA_CREATE_ALWAYS | FA_WRITE);

    if (FR_OK != fat_fs_status) {
      f_close(&s_i2c_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_write(&s_i2c_data_file, &s_u8_logger_fs_rx_data_buff[0U],
                            file_size, &writed_byte_size);

    if ((FR_OK != fat_fs_status) || (file_size != writed_byte_size)) {
      f_close(&s_i2c_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_close(&s_i2c_data_file);

    if (FR_OK != fat_fs_status) {
      return USB_FatFS_ERROR;
    }

    break;

  case COMM_PROTOCOL_TYPE_SPI:

    fat_fs_status =
        f_open(&s_spi_data_file, "SPI_DATA.txt", FA_CREATE_ALWAYS | FA_WRITE);

    if (FR_OK != fat_fs_status) {
      f_close(&s_spi_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_write(&s_spi_data_file, &s_u8_logger_fs_rx_data_buff[0U],
                            file_size, &writed_byte_size);

    if ((FR_OK != fat_fs_status) || (file_size != writed_byte_size)) {
      f_close(&s_spi_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_close(&s_spi_data_file);

    if (FR_OK != fat_fs_status) {
      return USB_FatFS_ERROR;
    }

    break;

  case COMM_PROTOCOL_TYPE_CAN:

    fat_fs_status =
        f_open(&s_can_data_file, "CAN_DATA.txt", FA_CREATE_ALWAYS | FA_WRITE);

    if (FR_OK != fat_fs_status) {
      f_close(&s_can_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_write(&s_can_data_file, &s_u8_logger_fs_rx_data_buff[0U],
                            file_size, &writed_byte_size);

    if ((FR_OK != fat_fs_status) || (file_size != writed_byte_size)) {
      f_close(&s_can_data_file);
      return USB_FatFS_ERROR;
    }

    fat_fs_status = f_close(&s_can_data_file);

    if (FR_OK != fat_fs_status) {
      return USB_FatFS_ERROR;
    }

    break;
  default:
    /*TODO Add error state*/
    break;
  }

  return USB_FatFS_OK;
}
