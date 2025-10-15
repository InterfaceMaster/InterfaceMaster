/**
 * Copyright (c) 2025 InterfaceMaster organization.
 * This work is licensed under a Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License. To view a
 * copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/
 */

/*
 * communication.h
 *
 *  Created on: Aug 4, 2025
 *      Author: MTA
 */

#ifndef INC_COMMUNICATION_H_
#define INC_COMMUNICATION_H_
#include <stdint.h>

/*
  ==============================================================================
                      ##### MACRO DEFINES #####
  ==============================================================================

  */

/**
 * @def MAX_COMM_PROTOCOL_SIZE
 * @brief This macro defines maximum communication protocol size in byte. STM32
 * MCU's sends 64 byte on full speed mode.
 * */
#define MAX_USB_PROTOCOL_SIZE 64U

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
  COMM_PROTOCOL_TYPE_CAN = 4U,
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
  COMM_STATUS_FAIL = 2U,
  COMM_STATUS_IDLE = 3U,
} CommProtocolStatus_e;

/**
 * @brief This enumeration defines active communication buffer.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

typedef enum {
  COMM_ACTIVE_BUFF_0 = 0U,
  COMM_ACTIVE_BUFF_1 = 1U,
} CommProtocolActiveBuff_e;

typedef struct CommProtocol_t {
  CommProtocolType_e type;
  CommProtocolActiveBuff_e active_buff;
  uint8_t *p_tx_buff;
  uint8_t *p_rx_buff_0;
  uint8_t *p_rx_buff_1;
  uint8_t u8_tx_size;
  uint8_t u8_rx_size;
  CommProtocolStatus_e tx_status;
  CommProtocolStatus_e rx_status;
} CommProtocol_t;

CommProtocolType_e get_comm_protocol_type(void);
uint8_t *get_comm_protocol_rx_buff(void);
uint8_t *get_comm_protocol_tx_buff(void);
uint8_t get_comm_protocol_rx_size(void);
uint8_t get_comm_protocol_tx_size(void);

void flush_comm_protocol_rx_buff(uint8_t *p_buff);
void set_comm_protocol_rx_size(uint8_t size);
void set_comm_protocol_tx_size(uint8_t size);

void init_comm_protocol_handler(CommProtocol_t *comm_protocol);
void deinit_comm_protocol_handler(void);

void USB_send_data(void);

#endif /* INC_COMMUNICATION_H_ */
