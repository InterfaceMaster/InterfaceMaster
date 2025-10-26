/**
 * Copyright (c) 2025 InterfaceMaster organization.
 * This work is licensed under a Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License. To view a
 * copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/
 */

/*
 * task.h
 *
 *  Created on: Aug 3, 2025
 *      Author: MTA
 */

#ifndef INC_TASKS_H_
#define INC_TASKS_H_
/**
 * @brief Forward declaration for the Communication Protocol structure.
 *
 * @details Declared as an incomplete type.This structure definition available
 * in "comminucation.h".
 */
typedef struct CommProtocol_t CommProtocol_t;

/**
 * @brief Forward declaration for the GUI structure.
 *
 * @details Declared as an incomplete type.This structure definition available
 * in "gui_defines.h".
 */

typedef struct GUI_System_t GUI_System_t;

/**
 *@brief This enumeration defines the external flash memory write operation.
 *@attention This enumerations important when the device using 'EXPORT DATA
 *			 MODE'. Any modification to this enumeration requires
 *			 analysis the entire system.
 */
typedef enum {
  USB_HOST_APP_IDLE = 0U,
  USB_HOST_APP_START = 1U,
  USB_HOST_APP_READY = 2U,
  USB_HOST_APP_DISCONNECT = 3U,
} USB_Host_ApplicationStatus_e;

/**
 *@brief This enumaration defines the device work mode.
 *@attention Any modification to this enumaration requires analysis the entire
 * system.
 */

typedef enum {
  WORK_MODE_IDLE = 0U,
  WORK_MODE_USB_BRIDGE = 1U,
  WORK_MODE_EXPORT_DATA = 2U,
} WorkMode_e;

/**
 * @brief  This structure holds the entire system status information and
 * configuration.
 * @details This structure centralizes data related the current state of all
 * tasks and sensors.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef struct SystemInstance_t {
  double ambient_temp;
  GUI_System_t *p_GUI;
  CommProtocol_t *p_comm_protocol;
  WorkMode_e device_work_mode;
  USB_Host_ApplicationStatus_e usb_host_app_status;
} SystemInstance_t;

void IM_peripheral_init(void);
void IM_system_init(void);
void IM_run_tasks(void);

#endif /* INC_TASKS_H_ */
