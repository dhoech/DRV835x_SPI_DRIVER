/*
 * DRV835x_SPI_DRIVER.c
 *
 *  Created on: 15. pro 2021.
 *      Author: Jan
 */

#include "DRV835x_SPI_DRIVER.h"


uint8_t GSV6_Initialise(DRV835x* device, SPI_HandleTypeDef* SpiHandle){

	device->SPIHandle = SpiHandle;

	return 0;
}





/*
 * LOW-LEVEL FUNKCIJE
 *
 */
HAL_StatusTypeDef DRV835x_SPI_TransmitReceive(DRV835x* device, uint8_t *MsgTx, uint8_t *MsgRx){

	return HAL_SPI_TransmitReceive(device->SPIHandle, MsgTx, MsgRx, 2,
			HAL_MAX_DELAY);

}

HAL_StatusTypeDef DRV835x_SPI_Transmit(DRV835x* device, uint8_t* MsgTx){

	return HAL_SPI_Transmit(device->SPIHandle, MsgTx, 2, HAL_MAX_DELAY);
}

HAL_StatusTypeDef DRV835x_SPI_Receive(DRV835x* device, uint8_t* MsgRx){

	return HAL_SPI_Receive(device->SPIHandle, MsgRx, 2, HAL_MAX_DELAY);
}
