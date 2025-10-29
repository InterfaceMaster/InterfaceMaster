/*
 * external_storage_fs.h
 *
 *  Created on: Oct 29, 2025
 *      Author: MTA
 */

#ifndef INC_EXTERNAL_STORAGE_FS_H_
#define INC_EXTERNAL_STORAGE_FS_H_

typedef enum {
  USB_FatFS_OK = 0U,
  USB_FatFS_ERROR = 1U,
} USBH_FatFS_Status_e;

USBH_FatFS_Status_e IM_USB_FatFS_write_data(const uint8_t *p_data,
                                            const uint32_t size);

#endif /* INC_EXTERNAL_STORAGE_FS_H_ */
