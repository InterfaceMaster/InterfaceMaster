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

#include "i2c.h"
#include "spi.h"
#include "usart.h"
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
    return &s_comm_protocol_handler.p_rx_buff_0[0U];
  } else if (COMM_ACTIVE_BUFF_1 == s_comm_protocol_handler.active_buff) {
    return &s_comm_protocol_handler.p_rx_buff_1[0U];
  }
  return NULL; /*TODO: add logs for hardfault*/
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
 *@brief This function checks the communication protocol state.
 *@param
 *@retVal None.
 */

void set_comm_protocol_receive_state(CommProtocolStatus_e state) {
  s_comm_protocol_handler.rx_status = state;
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
 *@brief This function fills transmit data with received data.
 *@param None.
 *@retVal None.
 */

void flush_comm_prtocol_tx_buff(void) {
  uint8_t *p_active_buff = get_comm_protocol_rx_buff();
  memcpy(&s_comm_protocol_handler.p_tx_buff[0U], p_active_buff,
         MAX_USB_PROTOCOL_SIZE);
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

  memset(&s_comm_protocol_handler.p_rx_buff_0[0U], '\0', MAX_USB_PROTOCOL_SIZE);
  memset(&s_comm_protocol_handler.p_rx_buff_1[0U], '\0', MAX_USB_PROTOCOL_SIZE);

  s_comm_protocol_handler.p_rx_buff_0 = NULL;
  s_comm_protocol_handler.p_rx_buff_1 = NULL;

  s_comm_protocol_handler.type = COMM_PROTOCOL_TYPE_NONE;
}

/**
 * @brief This function sends related data via USB.
 * @attention Any modification to this function not requires.
 * @param None.
 * @retVal None.
 */
void USB_send_data(void) {

  uint8_t *data_buff = NULL;
  uint8_t status;

  if (COMM_STATUS_OK == s_comm_protocol_handler.rx_status) {

    flush_comm_prtocol_tx_buff();

    status = CDC_Transmit_HS(&s_comm_protocol_handler.p_tx_buff[0U],
                             MAX_USB_PROTOCOL_SIZE);

    if ((uint8_t)USBD_OK != status) {
      s_comm_protocol_handler.tx_status = COMM_STATUS_FAIL;
      return;
      /*TODO give error from gui*/
    } else {
      s_comm_protocol_handler.tx_status = COMM_STATUS_OK;
    }
  }
}

/**
 * @brief  Reception Event Callback (Rx event notification called after use of
 * advanced reception service).
 * @param  huart UART handle
 * @param  Size  Number of data available in application reception buffer
 * (indicates a position in reception buffer until which, data are available)
 * @retval None
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
  /* Prevent unused argument(s) compilation warning */
  if (UART7 == huart->Instance) {

    if (62U < Size) {
      s_comm_protocol_handler.u8_rx_size = (uint8_t)Size;

      if ((NULL == s_comm_protocol_handler.p_rx_buff_0) ||
          (NULL == s_comm_protocol_handler.p_rx_buff_1)) {
        return;
      }

      if (COMM_ACTIVE_BUFF_0 == s_comm_protocol_handler.active_buff) {

        s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_1;

        *(s_comm_protocol_handler.p_rx_buff_0 + 62U) = '\r';
        *(s_comm_protocol_handler.p_rx_buff_0 + 63U) = '\n';

        s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

        memset(s_comm_protocol_handler.p_rx_buff_1, '\0',
               MAX_USB_PROTOCOL_SIZE);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart7,
                                     &s_comm_protocol_handler.p_rx_buff_1[0U],
                                     MAX_USB_PROTOCOL_SIZE);

      } else if (COMM_ACTIVE_BUFF_1 == s_comm_protocol_handler.active_buff) {

        s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_0;

        *(s_comm_protocol_handler.p_rx_buff_1 + 62U) = '\r';
        *(s_comm_protocol_handler.p_rx_buff_1 + 63U) = '\n';

        s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

        memset(s_comm_protocol_handler.p_rx_buff_0, '\0',
               MAX_USB_PROTOCOL_SIZE);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart7,
                                     &s_comm_protocol_handler.p_rx_buff_0[0U],
                                     MAX_USB_PROTOCOL_SIZE);
      } else {
        s_comm_protocol_handler.rx_status = COMM_STATUS_FAIL;
      }
    } else {
      s_comm_protocol_handler.rx_status = COMM_STATUS_FAIL;
    }

  } else if (USART2 == huart->Instance) {

    if (62U < Size) {

      if ((NULL == s_comm_protocol_handler.p_rx_buff_0) ||
          (NULL == s_comm_protocol_handler.p_rx_buff_1)) {
        return;
      }

      if (COMM_ACTIVE_BUFF_0 == s_comm_protocol_handler.active_buff) {

        s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_1;

        *(s_comm_protocol_handler.p_rx_buff_0 + 62U) = '\r';
        *(s_comm_protocol_handler.p_rx_buff_0 + 63U) = '\n';

        s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

        memset(&s_comm_protocol_handler.p_rx_buff_1[0U], '\0',
               MAX_USB_PROTOCOL_SIZE);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart2,
                                     &s_comm_protocol_handler.p_rx_buff_1[0U],
                                     MAX_USB_PROTOCOL_SIZE);

      } else if (COMM_ACTIVE_BUFF_1 == s_comm_protocol_handler.active_buff) {

        s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_0;

        *(s_comm_protocol_handler.p_rx_buff_1 + 62U) = '\r';
        *(s_comm_protocol_handler.p_rx_buff_1 + 63U) = '\n';

        s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

        memset(s_comm_protocol_handler.p_rx_buff_0, '\0',
               MAX_USB_PROTOCOL_SIZE);

        HAL_UARTEx_ReceiveToIdle_DMA(&huart2,
                                     &s_comm_protocol_handler.p_rx_buff_0[0U],
                                     MAX_USB_PROTOCOL_SIZE);

      } else {
        s_comm_protocol_handler.rx_status = COMM_STATUS_FAIL;
      }
    } else {
      s_comm_protocol_handler.rx_status = COMM_STATUS_FAIL;
    }
  }
}

/**
 * @brief  Slave Rx Transfer completed callback.
 * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
 *                the configuration information for the specified I2C.
 * @retval None
 */

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {

  if (I2C4 == hi2c->Instance) {

    if ((NULL == s_comm_protocol_handler.p_rx_buff_0) ||
        (NULL == s_comm_protocol_handler.p_rx_buff_1)) {
      return;
    }

    if (COMM_ACTIVE_BUFF_0 == s_comm_protocol_handler.active_buff) {

      s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_1;

      *(s_comm_protocol_handler.p_rx_buff_0 + 62U) = '\r';
      *(s_comm_protocol_handler.p_rx_buff_0 + 63U) = '\n';

      s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

      memset(&s_comm_protocol_handler.p_rx_buff_1[0U], '\0',
             MAX_USB_PROTOCOL_SIZE);

      HAL_I2C_Slave_Receive_DMA(&hi2c4,
                                &s_comm_protocol_handler.p_rx_buff_1[0U],
                                (uint16_t)MAX_USB_PROTOCOL_SIZE);

    } else if (COMM_ACTIVE_BUFF_1 == s_comm_protocol_handler.active_buff) {

      s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_0;

      *(s_comm_protocol_handler.p_rx_buff_1 + 62U) = '\r';
      *(s_comm_protocol_handler.p_rx_buff_1 + 63U) = '\n';

      s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

      memset(&s_comm_protocol_handler.p_rx_buff_0[0U], '\0',
             MAX_USB_PROTOCOL_SIZE);

      HAL_I2C_Slave_Receive_DMA(&hi2c4,
                                &s_comm_protocol_handler.p_rx_buff_0[0U],
                                (uint16_t)MAX_USB_PROTOCOL_SIZE);
    } else {
      s_comm_protocol_handler.rx_status = COMM_STATUS_FAIL;
    }
  }
}

/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI
 * line.
 * @retval None
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

  if (GPIO_Pin == GPIO_PIN_3) {

    HAL_SPI_DMAStop(&hspi2);

    if ((NULL == s_comm_protocol_handler.p_rx_buff_0) ||
        (NULL == s_comm_protocol_handler.p_rx_buff_1)) {
      return;
    }

    if (COMM_ACTIVE_BUFF_0 == s_comm_protocol_handler.active_buff) {

      s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_1;

      *(s_comm_protocol_handler.p_rx_buff_0 + 62U) = '\r';
      *(s_comm_protocol_handler.p_rx_buff_0 + 63U) = '\n';

      s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

      memset(&s_comm_protocol_handler.p_rx_buff_1[0U], '\0',
             MAX_USB_PROTOCOL_SIZE);

      HAL_SPI_Receive_DMA(&hspi2, &s_comm_protocol_handler.p_rx_buff_1[0U],
                          (uint16_t)MAX_USB_PROTOCOL_SIZE);

    } else if (COMM_ACTIVE_BUFF_1 == s_comm_protocol_handler.active_buff) {

      s_comm_protocol_handler.active_buff = COMM_ACTIVE_BUFF_0;

      *(s_comm_protocol_handler.p_rx_buff_1 + 62U) = '\r';
      *(s_comm_protocol_handler.p_rx_buff_1 + 63U) = '\n';

      s_comm_protocol_handler.rx_status = COMM_STATUS_OK;

      memset(&s_comm_protocol_handler.p_rx_buff_0[0U], '\0',
             MAX_USB_PROTOCOL_SIZE);

      HAL_SPI_Receive_DMA(&hspi2, &s_comm_protocol_handler.p_rx_buff_0[0U],
                          (uint16_t)MAX_USB_PROTOCOL_SIZE);

    } else {
      s_comm_protocol_handler.rx_status = COMM_STATUS_FAIL;
    }
  }
}
