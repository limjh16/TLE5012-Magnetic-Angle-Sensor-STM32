/**
 * @file        spic-stm32.hpp
 * @brief       STM32 PAL for the SPI cover
 * @date        June 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "spic-stm32.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include <stdio.h>

/**
 * @addtogroup stm32Pal
 * @{
 */

/**
 * @brief Construct a new SPICStm32::SPICStm32 object of the Arduino SPIC class
 *
 * This function sets all pins for a given SPI port, allowing a free setting
 * of the SPI interface
 * Use this call of:
 * - you want to use a different than this default SPI channel
 * - also with up to four sensors per channel
 *
 * @param csPin    chipselect pin for the SPI port
 * @param csPort   chipselect port
 * @param misoPin  miso pin number
 * @param mosiPin  mosi pin number
 * @param sckPin   systemclock pin number
 * @param spiPort  port for SPI pins
 * @param hspi     handle for SPI instance
 */
SPICStm32::SPICStm32(uint32_t csPin, GPIO_TypeDef* csPort, uint32_t misoPin, uint32_t mosiPin, uint32_t sckPin, GPIO_TypeDef* spiPort, SPI_HandleTypeDef* hspi)
{
	this->csPort  = csPort;
	this->csPin   = csPin;
	this->spiPort = spiPort;
	this->misoPin = misoPin;
	this->mosiPin = mosiPin;
	this->sckPin  = sckPin;
	this->hspi    = hspi;
}

/**
 * @brief Initialize the SPIC
 *
 * This function is initializing the chosen spi channel
 * with the given values for lsb, clock and mode
 *
 * @return      SPICStm32::Error_t
 */
SPICStm32::Error_t SPICStm32::init()
{
	this->spi = new SPIClass3W();
	this->spi->begin(this->misoPin, this->mosiPin, this->sckPin, this->spiPort, this->hspi, this->csPin, this->csPort);
	return OK;
}

/**
 * @brief Deinitialize the SPIC
 *
 * This function is deinitializing the chosen spi channel.
 *
 * @return      SPICStm32::Error_t
 */
SPICStm32::Error_t SPICStm32::deinit()
{
	// this->spi->endTransaction();
	//	this->spi->end();
	return OK;
}

/**
 * @brief 
 * Triggers an update in the register buffer. This function
 * should be triggered once before UPD registers where read as
 * it generates a snapshot of the UPD register values at trigger point
 * 
 * @return SPICStm32::Error_t 
 */
SPICStm32::Error_t SPICStm32::triggerUpdate()
{
	HAL_GPIO_WritePin(this->spiPort, this->sckPin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(this->spiPort, this->mosiPin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(this->csPort, this->csPin, GPIO_PIN_RESET);
	
	//grace period for register snapshot
	uint32_t clk_cycle_start = DWT->CYCCNT;
	const uint32_t FIVE_MICRO = HAL_RCC_GetHCLKFreq() * 5.0e-6f;
	while ((DWT->CYCCNT - clk_cycle_start) < FIVE_MICRO);

	HAL_GPIO_WritePin(this->csPort, this->csPin, GPIO_PIN_SET);
	return OK;
}

/*!
* Main SPI three wire communication functions for sending and receiving data
* @param sent_data pointer two 2*unit16_t value for one command word and one data word if something should be written
* @param size_of_sent_data the size of the command word default 1 = only command 2 = command and data word
* @param received_data pointer to data structure buffer for the read data
* @param size_of_received_data size of data words to be read
*/
SPICStm32::Error_t SPICStm32::sendReceive(uint16_t* sent_data, uint16_t size_of_sent_data, uint16_t* received_data, uint16_t size_of_received_data)
{
	this->spi->setCSPin(this->csPin, this->csPort);
	this->spi->sendReceiveSpi(sent_data,size_of_sent_data,received_data,size_of_received_data);
	return OK;
}

/** @} */

#endif /** TLE5012_FRAMEWORK **/
