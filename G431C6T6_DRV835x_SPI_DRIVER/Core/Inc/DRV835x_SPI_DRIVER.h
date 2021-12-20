/*
 * DRV835x_SPI_DRIVER.h
 *
 *  Created on: 15. pro 2021.
 *      Author: Jan
 */

#ifndef INC_DRV835X_SPI_DRIVER_H_
#define INC_DRV835X_SPI_DRIVER_H_


/*!!!!!!!    POTREBNO IZMJENITI AKO SE KORISTI NEKA DRUGA FAMILIJA KONTROLERA    !!!!!!!*/
#include "stm32g4xx.h"


/*
 * DRIVER STRUCT
 */

typedef struct{

	/*SPI Handle*/
	SPI_HandleTypeDef* SPIHandle;
	GPIO_TypeDef*  GPIOx_nSCS, *GPIOx_ENABLE;
	uint16_t GPIO_Pin_nSCS, GPIO_Pin_ENABLE;


}DRV835x;

/*
 * INICIJALIZACIJA
 */

uint8_t DRV835x_Initialise(DRV835x* device, SPI_HandleTypeDef* SPIHandle, GPIO_TypeDef *GPIOx_nscs,
						   uint16_t GPIO_Pin_nscs, GPIO_TypeDef *GPIOx_enable, uint16_t GPIO_Pin_enable);

/*
 * AKVIZICIJA PODATAKA I SLANJE NAREDBI
 */

HAL_StatusTypeDef DRV835x_CmdRead(DRV835x* device, uint8_t address, uint8_t *MsgRx);
HAL_StatusTypeDef DRV835x_CmdWrite(DRV835x* device, uint8_t address, uint16_t MsgTx);

/*
 * LOW-LEVEL FUNKCIJE
 *
 */
HAL_StatusTypeDef DRV835x_SPI_TransmitReceive(DRV835x* device, uint8_t *MsgTx, uint8_t *MsgRx);
HAL_StatusTypeDef DRV835x_SPI_Transmit(DRV835x* device, uint8_t* MsgTx);

#endif /* INC_DRV835X_SPI_DRIVER_H_ */
