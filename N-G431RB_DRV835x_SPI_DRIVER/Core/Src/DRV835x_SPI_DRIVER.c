/*
 * DRV835x_SPI_DRIVER.c
 *
 *  Created on: 15. pro 2021.
 *      Author: Jan
 */

#include "DRV835x_SPI_DRIVER.h"


uint8_t DRV835x_Initialise(DRV835x* device, SPI_HandleTypeDef* SpiHandle){

	/*Deklaracija parametara unutar structa za SPI izlaz*/
	device->SPIHandle = SpiHandle;

	/* Varijable za spremanje broja errora (vracaju se nakon sto se izvrti cijela funkcija)*/
	uint8_t errNum = 0;
/*	HAL_StatusTypeDef status;*/

	/*Bufferi*/
/*	uint8_t MsgTx[2], MsgRx[2];*/



/*	DRV835x_SPI_TransmitReceive(device, &MsgTx, &MsgRx);*/

	return errNum;
}

/*
 * AKVIZICIJA PODATAKA I SLANJE NAREDBI
 */

HAL_StatusTypeDef DRV835x_CmdRead(DRV835x* device, uint8_t address, uint8_t *MsgRx){

	uint8_t MsgTx[2];

	/* Pisanje poruke za read addresse*/
	MsgTx[0] = (address << 3) & 0x78;
	MsgTx[0] = MsgTx[0] | 0x80;
	MsgTx[1] = 0x00;

	DRV835x_SPI_TransmitReceive(device, MsgTx, MsgRx);

	return 0;
}

HAL_StatusTypeDef DRV835x_CmdWrite(DRV835x* device, uint8_t address, uint16_t MsgTx){

	uint8_t Msg[2];

	/* Pisanje poruke za write addresse*/
	Msg[0] = (address << 3) & 0x78;
	Msg[0] = Msg[0] | ((MsgTx >> 8) & 0x07 );
	Msg[0] = Msg[0] & 0x7F;
	Msg[1] = MsgTx & 0xFF;

	DRV835x_SPI_Transmit(device, Msg);

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
