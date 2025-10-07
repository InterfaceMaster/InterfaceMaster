/*
 * lm75b_temp_ic.h
 *
 *  Created on: Sep 29, 2025
 *      Author: MTA
 */

#ifndef INC_LM75B_TEMP_IC_H_
#define INC_LM75B_TEMP_IC_H_

typedef enum {
  LM75B_OK = 0U,
  LM75B_ERROR = 1U,
  LM75B_TIMEOUT = 2U,
} LM75B_Status_e;

LM75B_Status_e lm75b_get_ambient_temp(double *buff);

#endif /* INC_LM75B_TEMP_IC_H_ */
