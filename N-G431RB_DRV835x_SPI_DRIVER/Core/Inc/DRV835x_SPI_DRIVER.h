/*
 * DRV835x_SPI_DRIVER.h
 *
 *  Created on: 15. pro 2021.
 *      Author: Jan
 */

#ifndef INC_DRV835X_SPI_DRIVER_H_
#define INC_DRV835X_SPI_DRIVER_H_

#include "stm32g4xx.h"


/*
 * DRIVER STRUCT
 */

typedef struct{

	/*SPI Handle*/
	SPI_HandleTypeDef* SPIHandle;


}DRV835x;

/*
 * INICIJALIZACIJA
 */

uint8_t GSV6_Initialise(DRV835x* device, SPI_HandleTypeDef* SPIHandle);

/*
 * AKVIZICIJA PODATAKA I SLANJE NAREDBI
 */

HAL_StatusTypeDef DRV835x_FaultStatus(DRV835x* device);
HAL_StatusTypeDef DRV835x_CmdWrite(DRV835x* device, uint8_t address, uint8_t MsgTx);
HAL_StatusTypeDef DRV835x_CmdRead(DRV835x* device, uint8_t address, uint8_t MsgRx);

/*
 * LOW-LEVEL FUNKCIJE
 *
 */
HAL_StatusTypeDef DRV835x_SPI_TransmitReceive(DRV835x* device, uint8_t *pTxData, uint8_t *pRxData);
HAL_StatusTypeDef DRV835x_SPI_Transmit(DRV835x* device, uint8_t* MsgTx);
HAL_StatusTypeDef DRV835x_SPI_Receive(DRV835x* device, uint8_t* MsgRx);

#endif /* INC_DRV835X_SPI_DRIVER_H_ */
