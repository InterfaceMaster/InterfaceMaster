/*
 * external_storage_fs.h
 *
 *  Created on: Oct 29, 2025
 *      Author: MTA
 */

#ifndef INC_EXTERNAL_STORAGE_FS_H_
#define INC_EXTERNAL_STORAGE_FS_H_

#include <stdint.h>

/**
 * @brief Forward declaration for the Communication Protocol structure.
 *
 * @details Declared as an incomplete type.This structure definition available
 * in "comminucation.h".
 */
typedef struct CommProtocol_t CommProtocol_t;

typedef enum {
  USB_FatFS_OK = 0U,
  USB_FatFS_ERROR = 1U,
} USBH_FatFS_Status_e;

void IM_USB_FatFS_fill_instance(CommProtocol_t *p_comm_protocol);

USBH_FatFS_Status_e IM_USB_FatFS_write_data(void);

#endif /* INC_EXTERNAL_STORAGE_FS_H_ */
