/*
 * DRV835x_SPI_DRIVER.c
 *
 *  Created on: 15. pro 2021.
 *      Author: Jan
 */

#include "DRV835x_SPI_DRIVER.h"


/*
 * 									DEFINICIJA PORUKE
 * 					-------------------------------------------------
 *					|	15 bit		|	14-11 bit	|	10-0 bit 	|
 * 					-------------------------------------------------
 * 					|	Read/write	|				|				|
 * 					|	1 = read	|	Addressa	|	  Data		|
 * 					|	0 = write	|				|				|
 * 					-------------------------------------------------
 */


/*
 * INICIJALIZACIJA
 */

/**
  *
  * @param  device je definirani structure u koji ce se upisati svi potrebni parametri za porektanja
  * 		svih funkcija unutar funkcije
  *
  * @param  spiHandle je pointer na SPI_HandleTypeDef structure u kojem su sadrzane informacije
  * 		o konfiguraciji SPI modula
  *
  * @param  GPIOx_nscs/enable gdje x moze biti od (A..G) kako bi smo izabrali GPIO periferiju za STM32G4xx familiju
  * 		(npr. GPIOA, GPIOB)
  *
  * @param  GPIO_Pin_nscs/enable specifira port (0..15) po kojem cemo pisat
  * 		(npr. GPIO_PIN_8, GPIO_PIN_10)
  *
  * @retval Broj errora (errNum)
  *
  */

uint8_t DRV835x_Initialise(DRV835x* device, SPI_HandleTypeDef* spiHandle, GPIO_TypeDef *GPIOx_nscs,
						   uint16_t GPIO_Pin_nscs, GPIO_TypeDef *GPIOx_enable, uint16_t GPIO_Pin_enable){

	/*Deklaracija svih parametara unutar structa za komunikaciju s DRV835*/
	device->SPIHandle = spiHandle;
	device->GPIOx_nSCS = GPIOx_nscs;
	device->GPIOx_ENABLE = GPIOx_enable;
	device->GPIO_Pin_nSCS = GPIO_Pin_nscs;
	device->GPIO_Pin_ENABLE = GPIO_Pin_enable;

	/* Varijable za spremanje broja errora (vracaju se nakon sto se izvrsi cijela funkcija)*/
	uint8_t errNum = 0;

	/*Postavljanje pina za ENABLE u HIGH*/
	HAL_GPIO_WritePin(GPIOx_enable, GPIO_Pin_enable, 1);

	/*Postavljanje pina za nSCS u HIGH kako komunikacija između DRV835 i STM nebi zapocela slucajno*/
	HAL_GPIO_WritePin(GPIOx_nscs, GPIO_Pin_nscs, 0);

	return errNum;
}



/*
 * AKVIZICIJA PODATAKA I SLANJE NAREDBI
 */

/**
  *
  * @param  device je definirani structure u koji ce se upisati svi potrebni parametri za porektanja
  * 		svih funkcija unutar funkcije
  *
  * @param  address je variabla u koju se piše željena adresa za ocitavanja vrijednosti
  *			(npr. 0x02, 0x05)
  *
  * @param  MsgRx je pointer na varijablu u koju se sprema dobivena poruka s SPI protokola
  *
  * @retval None
  *
  */

HAL_StatusTypeDef DRV835x_CmdRead(DRV835x* device, uint8_t address, uint8_t *MsgRx){

	uint8_t MsgTx[2];

	/* Pisanje poruke za read addresse*/
	MsgTx[0] = (address << 3) & 0x78;
	MsgTx[0] = MsgTx[0] | 0x80;
	MsgTx[1] = 0x00;

	/*Slanje poruke za READ vrijednosti s trazene adrese i spremanje njihove vrijednosti u MsgRx*/
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 0);
	HAL_Delay(0.1);
	DRV835x_SPI_TransmitReceive(device, MsgTx, MsgRx);
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 1);
	return 0;
}

/**
  *
  * @param  device je definirani structure u koji ce se upisati svi potrebni parametri za porektanja
  * 		svih funkcija unutar funkcije
  *
  * @param  address je variabla u koju se piše željena adresa za ocitavanja vrijednosti
  *			(npr. 0x02, 0x05)
  *
  * @param  MsgTx je varijabla u kojoj je upisana poruka koja se želi poslati DRV835
  * 		(npr. 0x160, 0x280)
  *
  * @retval None
  *
  */

HAL_StatusTypeDef DRV835x_CmdWrite(DRV835x* device, uint8_t address, uint16_t MsgTx){

	uint8_t Msg[2];

	/* Pisanje poruke za WRITE addresse*/
	Msg[0] = (address << 3) & 0x78;
	Msg[0] = Msg[0] | ((MsgTx >> 8) & 0x07 );
	Msg[0] = Msg[0] & 0x7F;
	Msg[1] = MsgTx & 0xFF;

	/* Slanje poruke za WRITE na potrebnu adresu*/
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 0);
	HAL_Delay(0.1);
	DRV835x_SPI_Transmit(device, Msg);
	HAL_GPIO_WritePin(device->GPIOx_nSCS, device->GPIO_Pin_nSCS, 1);
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
