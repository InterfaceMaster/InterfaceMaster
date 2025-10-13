/**
 * Copyright (c) 2025 InterfaceMaster organization.
 * This work is licensed under a Creative Commons
 * Attribution-NonCommercial-ShareAlike 4.0 International License. To view a
 * copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/
 */

/*
 * communication.c
 *
 *  Created on: Aug 4, 2025
 *      Author: MTA
 */
#include "communication.h"
#include <stdint.h>
#include <string.h>

#include "usb_host.h"
#include "usbd_cdc_if.h"

/*
  ==============================================================================
                      ##### STATIC VARIABLE DECLERATIONS #####
  ==============================================================================

  */

/**
 * @brief This structure defines communication protocol.
 * @attention Any modification to this structure requires analysis the entire
 * system.
 */

static CommProtocol_t s_comm_protocol_handler = {0U};
/*
  ==============================================================================
                      ##### STATIC FUNCTION IMPLEMENTATIONS #####
  ==============================================================================

  */

/**
 * @brief This function sets communication protocol Tx buffer start address.
 * @attention Any modification to this function not requires.
 * @param Address of Tx buffer.
 * @retVal None.
 */
static void s_set_comm_protocol_tx_buff_addrs(uint8_t *p_buff) {
  if (NULL == p_buff) {
    return;
  }
  s_comm_protocol_handler.p_tx_buff = p_buff;
}

/**
 *@brief This function sets the rx buffers start address.
 * @attention Any modification to this function not requires.
 * @param Address of Rx buffer.
 * @retVal None.
 */

static void s_set_comm_protocol_rx_buff_addrs(uint8_t *p_rx_buff_0,
                                              uint8_t *p_rx_buff_1) {
  if ((NULL == p_rx_buff_0) || (NULL == p_rx_buff_1)) {
    return;
  }
  s_comm_protocol_handler.p_rx_buff_0 = p_rx_buff_0;
  s_comm_protocol_handler.p_rx_buff_1 = p_rx_buff_1;
}

/**
 * @brief This function sets communication protocol type.
 * @attention Any modification to this function not requires.
 * @param Type of communication protocol.
 * @retVal None.
 */
static void s_set_comm_protocol_type(CommProtocolType_e comm_protocol_type) {

  switch (comm_protocol_type) {
  case COMM_PROTOCOL_TYPE_UART:
    s_comm_protocol_handler.type = COMM_PROTOCOL_TYPE_UART;
    break;
  case COMM_PROTOCOL_TYPE_I2C:
    s_comm_protocol_handler.type = COMM_PROTOCOL_TYPE_I2C;
    break;
  case COMM_PROTOCOL_TYPE_SPI:
    s_comm_protocol_handler.type = COMM_PROTOCOL_TYPE_SPI;
    break;
  case COMM_PROTOCOL_TYPE_CAN:
    s_comm_protocol_handler.type = COMM_PROTOCOL_TYPE_CAN;
  default:
    s_comm_protocol_handler.type = COMM_PROTOCOL_TYPE_NONE;
    break;
  }
}

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function fill communication protocol Rx buffer.
 * @attention Any modification to this function not requires.
 * @param Address of data buffer.
 * @retVal None.
 */
void fill_comm_protocol_rx_buff(uint8_t *p_buff) {
  if ((NULL == p_buff) || (NULL == s_comm_protocol_handler.p_rx_buff_0) ||
      (NULL == s_comm_protocol_handler.p_rx_buff_1)) {
    return;
  }

  if (62U < s_comm_protocol_handler.u8_rx_size) {
    return;
  }

  if (COMM_ACTIVE_BUFF_0 == s_comm_protocol_handler.active_buff) {
    s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_1;
    memset(s_comm_protocol_handler.p_rx_buff_0, '\0', MAX_COMM_PROTOCOL_SIZE);
    memcpy(s_comm_protocol_handler.p_rx_buff_0, p_buff,
           MAX_COMM_PROTOCOL_SIZE - 2U);
    *(s_comm_protocol_handler.p_rx_buff_0 + 62U) = '\r';
    *(s_comm_protocol_handler.p_rx_buff_0 + 63U) = '\n';
  } else if (COMM_ACTIVE_BUFF_1 == s_comm_protocol_handler.active_buff) {
    s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_0;
    memset(s_comm_protocol_handler.p_rx_buff_1, '\0', MAX_COMM_PROTOCOL_SIZE);
    memcpy(s_comm_protocol_handler.p_rx_buff_1, p_buff,
           MAX_COMM_PROTOCOL_SIZE - 2U);
    *(s_comm_protocol_handler.p_rx_buff_1 + 62U) = '\r';
    *(s_comm_protocol_handler.p_rx_buff_1 + 63U) = '\n';
  }
}

/**
 * @brief This function set the communication protocol Rx data amaount.
 * @attention Any modification to this function not requires.
 * @param Amaount of Rx data.
 * @retVal None.
 */
void set_comm_protocol_rx_size(uint8_t size) {
  s_comm_protocol_handler.u8_rx_size = size;
}

/**
 * @brief This function set the communication protocol Tx data amaount.
 * @attention Any modification to this function not requires.
 * @param Amaount of Tx data.
 * @retVal None.
 */
void set_comm_protocol_tx_size(uint8_t size) {
  s_comm_protocol_handler.u8_tx_size = size;
}

/**
 * @brief This function gets communication protocol Tx buffer start address.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal Address of Tx buff.
 */
uint8_t *get_comm_protocol_tx_buff(void) {
  return s_comm_protocol_handler.p_tx_buff;
}

/**
 * @brief This function gets communication protocol Rx buffer start address.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal Address of Rx buff.
 */
uint8_t *get_comm_protocol_rx_buff(void) {

  if (COMM_ACTIVE_BUFF_0 == s_comm_protocol_handler.active_buff) {
    return s_comm_protocol_handler.p_rx_buff_0;
  } else if (COMM_ACTIVE_BUFF_1 == s_comm_protocol_handler.active_buff) {
    return s_comm_protocol_handler.p_rx_buff_1;
  }
  return 0U; /*TODO: add logs for hardfault*/
}

/**
 * @brief This function get the communication protocol Rx data amaount.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal Amaount of Rx data.
 */
uint8_t get_comm_protocol_rx_size(void) {
  return s_comm_protocol_handler.u8_rx_size;
}

/**
 * @brief This function get the communication protocol Tx data amaount.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal Amaount of Rx data.
 */
uint8_t get_comm_protocol_tx_size(void) {
  return s_comm_protocol_handler.u8_tx_size;
}

/**
 * @brief This function gets communication protocol type.
 * @attention Any modification to this function not requires.
 * @param Type of communication protocol.
 * @retVal None.
 */
CommProtocolType_e get_comm_protocol_type(void) {
  return s_comm_protocol_handler.type;
}

/**
 *@brief This function initialize the communication task handler.
 *@param Communication protocol handler.
 *retVal None.
 */

void init_comm_protocol_handler(CommProtocol_t *comm_protocol) {

  if (NULL == comm_protocol) {
    return;
  }

  s_set_comm_protocol_type(comm_protocol->type);
  s_set_comm_protocol_rx_buff_addrs(&comm_protocol->p_rx_buff_0[0U],
                                    &comm_protocol->p_rx_buff_1[0U]);
  s_set_comm_protocol_tx_buff_addrs(&comm_protocol->p_tx_buff[0U]);
}

/**
 *@brief This function deinitialize the communication task handler.
 *@param None.
 *retVal None.
 */

void deinit_comm_protocol_handler(void) {

  memset(&s_comm_protocol_handler.p_rx_buff_0[0U], '\0', MAX_COMM_PROTOCOL_SIZE);
  memset(&s_comm_protocol_handler.p_rx_buff_1[0U], '\0', MAX_COMM_PROTOCOL_SIZE);

  s_comm_protocol_handler.p_rx_buff_0 = NULL;
  s_comm_protocol_handler.p_rx_buff_1 = NULL;

  s_comm_protocol_handler.type = COMM_PROTOCOL_TYPE_NONE;
}

/**
 *@brief This function is the communication task state machine.
 *@param None.
 *retVal None.
 */

void run_comm_protocol_task(CommProtocol_t *comm_protocol) {

  if (NULL == comm_protocol) {
    return;
  }
}

/**
 * @brief This function sends related data via USB.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal None.
 */
void USB_send_data(void) {}
