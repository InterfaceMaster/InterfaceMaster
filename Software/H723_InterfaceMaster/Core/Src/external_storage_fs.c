/*
 * external_storage_fs.c
 *
 *  Created on: Oct 29, 2025
 *      Author: MTA
 */

#include "external_storage_fs.h"
#include "fatfs.h"
#include "tasks.h"

/*
  ==============================================================================
                      ##### STATIC VARIABLE DECLARATIONS #####
  ==============================================================================

  */

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
 *@brief This function writes data to the external flash disk.
 *@param p_data Pointer of to be written data.
 *@param size Amaount of to be written data.
 *@retVal status Status of write operation.
 */

USBH_FatFS_Status_e IM_USB_FatFS_write_data(const uint8_t *p_data,
                                            const uint32_t size) {

  if ((NULL == p_data) || (0U == size)) {
    return USB_FatFS_ERROR;
  }

  FRESULT status = FR_OK;

  status =
      f_open(&s_uart_data_file, "UART_DATA.txt ", FA_CREATE_ALWAYS | FA_WRITE);

  if (FR_OK != status) {
    return USB_FatFS_ERROR;
  }

  status =
      f_open(&s_i2c_data_file, "I2C_DATA.txt", FA_CREATE_ALWAYS | FA_WRITE);

  if (FR_OK != status) {
    return USB_FatFS_ERROR;
  }

  status =
      f_open(&s_spi_data_file, "SPI_DATA.txt", FA_CREATE_ALWAYS | FA_WRITE);

  if (FR_OK != status) {
    return USB_FatFS_ERROR;
  }

  status =
      f_open(&s_can_data_file, "CAN_DATA.txt", FA_CREATE_ALWAYS | FA_WRITE);

  if (FR_OK != status) {
    return USB_FatFS_ERROR;
  }
  return USB_FatFS_OK;
}
