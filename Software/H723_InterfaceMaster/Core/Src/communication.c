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

#include "usb_host.h"
#include "usbd_cdc_if.h"

/*
  ==============================================================================
                      ##### STATIC VARIABLE DECLERATIONS #####
  ==============================================================================

  */
static CommProtocol_t comm_protocol_system = {0U};
/*
  ==============================================================================
                      ##### STATIC FUNCTION IMPLEMENTATIONS #####
  ==============================================================================

  */

/*
  ==============================================================================
                      ##### GLOBAL FUNCTIONS #####
  ==============================================================================

  */

/**
 * @brief This function sets communication protocol Tx buffer start address.
 * @attention Any modification to this function not requires.
 * @param Address of Tx buffer.
 * @retVal None.
 */
void set_comm_protocol_tx_buff(uint8_t *p_buff) {
  comm_protocol_system.p_tx_buff = p_buff;
}

/**
 * @brief This function sets communication protocol Rx buffer start address.
 * @attention Any modification to this function not requires.
 * @param Address of Rx buffer.
 * @retVal None.
 */
void set_comm_protocol_rx_buff(uint8_t *p_buff) {
  if (0U == comm_protocol_system.u8_active_rx_buff) {
    ++comm_protocol_system.u8_active_rx_buff;
    comm_protocol_system.p_rx_buff_0 = p_buff;
  } else if (1U == comm_protocol_system.u8_active_rx_buff) {
    comm_protocol_system.u8_active_rx_buff = 0U;
    comm_protocol_system.p_rx_buff_1 = p_buff;
  }
}

/**
 * @brief This function sets communication protocol type.
 * @attention Any modification to this function not requires.
 * @param Type of communication protocol.
 * @retVal None.
 */
void set_comm_protocol_type(CommProtocolType_e comm_protocol_type) {
  comm_protocol_system.type = comm_protocol_type;
}

/**
 * @brief This function set the communication protocol Rx data amaount.
 * @attention Any modification to this function not requires.
 * @param Amaount of Rx data.
 * @retVal None.
 */
void set_comm_protocol_rx_size(uint8_t amaount) {
  comm_protocol_system.u8_rx_size = amaount;
}

/**
 * @brief This function set the communication protocol Tx data amaount.
 * @attention Any modification to this function not requires.
 * @param Amaount of Tx data.
 * @retVal None.
 */
void set_comm_protocol_tx_size(uint8_t amaount) {
  comm_protocol_system.u8_tx_size = amaount;
}

/**
 * @brief This function gets communication protocol Tx buffer start address.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal Address of Tx buff.
 */
uint8_t *get_comm_protocol_tx_buff(void) {
  return comm_protocol_system.p_tx_buff;
}

/**
 * @brief This function gets communication protocol Rx buffer start address.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal Address of Rx buff.
 */
uint8_t *get_comm_protocol_rx_buff(void) {

  if (0U == comm_protocol_system.u8_active_rx_buff) {
    return comm_protocol_system.p_rx_buff_0;
  } else if (1U == comm_protocol_system.u8_active_rx_buff) {
    return comm_protocol_system.p_rx_buff_1;
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
  return comm_protocol_system.u8_rx_size;
}

/**
 * @brief This function get the communication protocol Tx data amaount.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal Amaount of Rx data.
 */
uint8_t get_comm_protocol_tx_size(void) {
  return comm_protocol_system.u8_tx_size;
}

/**
 * @brief This function gets communication protocol type.
 * @attention Any modification to this function not requires.
 * @param Type of communication protocol.
 * @retVal None.
 */
CommProtocolType_e get_comm_protocol_type(void) {
  return comm_protocol_system.type;
}

/**
 * @brief This function sends related data via USB.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal None.
 */
void USB_send_data(void) {}
