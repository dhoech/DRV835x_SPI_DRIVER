/*
 * DRV835x_SPI_DRIVER.c
 *
 *  Created on: 24-11-05
 *      Author: dhoech
 *		original from Jan190
 */

#include "DRV835x_SPI_DRIVER.h"


/*
 * 									MESSAGE DEFINITION
 * 					-------------------------------------------------
 *					|	Bit 15	    |	Bits 14-11   |	Bits 10-0 	|
 * 					-------------------------------------------------
 * 					|	Read/write	|	Address		|	Data		|
 * 					|	1 = read	|				|				|
 * 					|	0 = write	|				|				|
 * 					-------------------------------------------------
 */


/*
 * INITIALIZATION
 */

/**
  *
  * @param  device is a defined structure in which all necessary parameters for starting
  *         all functions within the function will be recorded.
  *
  * @param  spiHandle is a pointer to an SPI_HandleTypeDef structure that contains information
  *         about the SPI module configuration.
  *
  * @param  GPIOx_nscs/enable where x can be from (A..G) to select the GPIO peripheral for the STM32G4xx family
  *         (e.g., GPIOA, GPIOB).
  *
  * @param  GPIO_Pin_nscs/enable specifies the port (0..15) where we will write
  *         (e.g., GPIO_PIN_8, GPIO_PIN_10).
  *
  * @retval Number of errors (errNum).
  *
  */

uint8_t DRV835x_Initialise(DRV835x* device, SPI_HandleTypeDef* spiHandle, GPIO_TypeDef *GPIOx_nscs,
                           uint16_t GPIO_Pin_nscs, GPIO_TypeDef *GPIOx_enable, uint16_t GPIO_Pin_enable) {

	/* Declaration of all parameters within the struct for communication with DRV835 */
	device->SPIHandle = spiHandle;
	device->GPIOx_nSCS = GPIOx_nscs;
	device->GPIOx_ENABLE = GPIOx_enable;
	device->GPIO_Pin_nSCS = GPIO_Pin_nscs;
	device->GPIO_Pin_ENABLE = GPIO_Pin_enable;

	/* Variable for storing the number of errors (returned after the entire function is executed) */
	uint8_t errNum = 0;

	/* Setting the ENABLE pin to HIGH */
	HAL_GPIO_WritePin(GPIOx_enable, GPIO_Pin_enable, 1);

	/* Setting the nSCS pin to HIGH so communication between DRV835 and STM does not start accidentally */
	HAL_GPIO_WritePin(GPIOx_nscs, GPIO_Pin_nscs, 0);

	return errNum;
}



/*
 * DATA ACQUISITION AND COMMAND SENDING
 */

/**
  *
  * @param  device is a defined structure in which all necessary parameters for starting
  *         all functions within the function will be recorded.
  *
  * @param  address is a variable in which the desired address for reading the value is written
  *         (e.g., 0x02, 0x05).
  *
  * @param  MsgRx is a pointer to a variable where the received message from the SPI protocol will be stored.
  *
  * @retval None
  *
  */

HAL_StatusTypeDef DRV835x_CmdRead(DRV835x* device, uint8_t address, uint8_t *MsgRx) {

	uint8_t MsgTx[2];

	/* Writing the message for the read address */
	MsgTx[0] = (address << 3) & 0x78;
	MsgTx[0] = MsgTx[0] | 0x80;
	MsgTx[1] = 0x00;

	/* Sending the message to READ the value from the requested address and storing the value in MsgRx */
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 0);
	HAL_Delay(0.1);
	DRV835x_SPI_TransmitReceive(device, MsgTx, MsgRx);
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 1);
	return 0;
}

/**
  *
  * @param  device is a defined structure in which all necessary parameters for starting
  *         all functions within the function will be recorded.
  *
  * @param  address is a variable in which the desired address for reading the value is written
  *         (e.g., 0x02, 0x05).
  *
  * @param  MsgTx is a variable that contains the message to be sent to DRV835
  *         (e.g., 0x160, 0x280).
  *
  * @retval None
  *
  */

HAL_StatusTypeDef DRV835x_CmdWrite(DRV835x* device, uint8_t address, uint16_t MsgTx) {

	uint8_t Msg[2];

	/* Writing the message for the WRITE address */
	Msg[0] = (address << 3) & 0x78;
	Msg[0] = Msg[0] | ((MsgTx >> 8) & 0x07 );
	Msg[0] = Msg[0] & 0x7F;
	Msg[1] = MsgTx & 0xFF;

	/* Sending the WRITE message to the required address */
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 0);
	HAL_Delay(0.1);
	DRV835x_SPI_Transmit(device, Msg);
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 1);
	return 0;
}


/*
 * LOW-LEVEL FUNCTIONS
 *
 */

HAL_StatusTypeDef DRV835x_SPI_TransmitReceive(DRV835x* device, uint8_t *MsgTx, uint8_t *MsgRx) {
	return HAL_SPI_TransmitReceive(device->SPIHandle, MsgTx, MsgRx, 2, HAL_MAX_DELAY);
}

HAL_StatusTypeDef DRV835x_SPI_Transmit(DRV835x* device, uint8_t* MsgTx) {
	return HAL_SPI_Transmit(device->SPIHandle, MsgTx, 2, HAL_MAX_DELAY);
}
