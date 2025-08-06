/*
 * communication.c
 *
 *  Created on: Aug 4, 2025
 *      Author: MTA
 */

#include "communication.h"

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
 * @brief This function sets communication protocol Rx buffer start address.
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
  comm_protocol_system.p_rx_buff = p_buff;
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
 * @brief This function sends related data via USB.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal None.
 */
void USB_send_data(void) {}
