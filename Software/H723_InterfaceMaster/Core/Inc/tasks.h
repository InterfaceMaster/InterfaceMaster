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
 * @brief  This structure holds the entire system status information and
 * configuration.
 * @details This structure centralizes data related the current state of all
 * tasks and sensors.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef struct SystemInstance_t {
  double ambient_temp;
  CommProtocol_t *p_comm_protocol;
} SystemInstance_t;

void IM_peripheral_init(void);
void IM_system_init(void);
void IM_run_tasks(void);

#endif /* INC_TASKS_H_ */
