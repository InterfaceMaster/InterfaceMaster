/*
 * communication.h
 *
 *  Created on: Aug 4, 2025
 *      Author: MTA
 */

#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_

/**
 * @brief This enumaration define Communication Protocol types.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef enum {
  COMM_PROTOCOL_TYPE_NONE = 0U,
  COMM_PROTOCOL_TYPE_UART = 1U,
  COMM_PROTOCOL_TYPE_I2C = 2U,
  COMM_PROTOCOL_TYPE_SPI = 3U,
} CommProtocolType_e;

/**
 * @brief This structure define Communication Protocol.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef struct {
  CommProtocolType_e comm_protocol_type;
  uint8_t *p_tx_buff;
  uint8_t *p_rx_buff;
  uint8_t u8_tx_size;
  uint8_t u8_rx_size;
  uint8_t u8_tx_complete;
  uint8_t u8_rx_complete;
} CommProtocol_t;

#endif /* INC_COMMUNICATION_H_ */
