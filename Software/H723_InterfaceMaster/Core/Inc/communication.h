/*
 * communication.h
 *
 *  Created on: Aug 4, 2025
 *      Author: MTA
 */

#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_

/*
  ==============================================================================
                      ##### MACRO DEFINES #####
  ==============================================================================

  */

/**
 * @def MAX_COMM_PROTOCOL_SIZE
 * @brief This macro defines maximum communication protocol size in byte.
 * */
#define MAX_COMM_PROTOCOL_SIZE 64U

/*
  ==============================================================================
                      ##### USER DEFINE TYPES #####
  ==============================================================================

  */

/**
 * @brief This enumaration defines communication protocol types.
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
 * @brief This enumeration defines status and error codes for communication
 * protocol functions.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef enum {
  COMM_STATUS_OK = 0U,
  COMM_STATUS_BUSY = 1U,
  COMM_STATUS_EMEM = 2U,
  COMM_STATUS_FAIL = 3U,
} CommProtocolStatus_e;

/**
 * @brief This structure defines communication protocol.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef struct {
  CommProtocolType_e type;
  uint8_t *p_tx_buff;
  uint8_t *p_rx_buff;
  uint8_t u8_tx_size;
  uint8_t u8_rx_size;
  CommProtocolStatus_e tx_status;
  CommProtocolStatus_e rx_status;
} CommProtocol_t;

void set_comm_protocol_type(CommProtocolType_e comm_protocol_type);
void USB_send_data(void);

#endif /* INC_COMMUNICATION_H_ */
