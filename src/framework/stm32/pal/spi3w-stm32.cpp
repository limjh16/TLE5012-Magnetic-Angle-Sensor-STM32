/*
 * @file        spi-platf-ino.cpp
 * @brief       TLE5012 STM32 HAL SPI Wrapper
 * @date        October 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include <algorithm>
#include "spi3w-stm32.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_rcc.h"
#include <stdio.h>

/**
 * @addtogroup stm32Pal
 * @{
 * 
 * @brief STM32 HAL SPI Wrapper to use 3wire SSC SPI interfaces
 */

/**
 * @brief Construct a new SPIClass3W::SPIClass3W object
 * 
 */
SPIClass3W::SPIClass3W()
{
	// this->mCS = PIN_SPI_SS;
	// this->mMISO = PIN_SPI_MISO;
	// this->mMOSI = PIN_SPI_MOSI;
	// this->mSCK = PIN_SPI_SCK;
	// this->mSpiNum = 0;
}

/**
 * @brief Destroy the SPIClass3W::SPIClass3W object
 * 
 */
SPIClass3W::~SPIClass3W()
{
}

/*!
 * @brief New for fetching SPI parameter
 * 
 * @param miso [in] pin number for miso, on sensor2go boards the same than mosi
 * @param mosi [in] pin number for mosi, on sensor2go boards the same than miso
 * @param sck [in] the system clock pin for external clock driver
 * @param spiPort [in] port register for SPI pins
 * @param hspi [in] SPI handle
 * @param cs [in] chipselect pin, up to four different cs pins can be used together with the slave number
 * @param csPort [in] port register for chipselect pin
 */
void SPIClass3W::begin(uint32_t miso, uint32_t mosi, uint32_t sck, GPIO_TypeDef* spiPort, SPI_HandleTypeDef* hspi, uint32_t cs, GPIO_TypeDef* csPort)
{
	// Begin DWT, as a micros counter
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	setCSPin(cs, csPort);
	this->mhspi = hspi;
	this->mMOSI = mosi;
	this->mMISO = miso;
	this->mSCK = sck;
	this->mSPIPort = spiPort;

	HAL_GPIO_WritePin(this->mCSPort, this->mCS, GPIO_PIN_SET);
	HAL_SPI_DeInit(this->mhspi);
	// __HAL_SPI_DISABLE(this->mhspi);
	SPI_1LINE_TX(this->mhspi);
	// __HAL_SPI_ENABLE(this->mhspi);
	HAL_SPI_Init(this->mhspi);
}

/*!
 * @brief Set the chipselect pin. This function is needed if more than one
 * sensor is in the SPI bus
 * 
 * @param cs [in] Pin number of the sensors chip select
 * @param csPort [in] Port register of the sensors chip select
 */
void SPIClass3W::setCSPin(uint32_t cs, GPIO_TypeDef* csPort)
{
	this->mCS = cs;
	this->mCSPort = csPort;
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->mCS;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->mCSPort, &GPIO_InitStruct);
}

/*!
 * @brief Main SPI three wire communication functions for sending and receiving data
 * 
 * @param sent_data pointer two 2*unit16_t value for one command word and one data word if something should be written
 * @param size_of_sent_data the size of the command word default 1 = only command 2 = command and data word
 * @param received_data pointer to data structure buffer for the read data
 * @param size_of_received_data size of data words to be read
 */
void SPIClass3W::sendReceiveSpi(uint16_t* sent_data, uint16_t size_of_sent_data, uint16_t* received_data, uint16_t size_of_received_data)
{
	//send via TX
	HAL_GPIO_WritePin(this->mCSPort, this->mCS, GPIO_PIN_RESET);
	// SPI should be in 1LINE_TX by default
	HAL_SPI_Transmit(this->mhspi, (uint8_t*)sent_data, size_of_sent_data, 100);
	
	uint32_t clk_cycle_start = DWT->CYCCNT;
	const uint32_t FIVE_MICRO = HAL_RCC_GetHCLKFreq() * 5.0e-6f;
	while ((DWT->CYCCNT - clk_cycle_start) < FIVE_MICRO);

	HAL_SPI_DeInit(this->mhspi);
	// __HAL_SPI_DISABLE(this->mhspi);
	SPI_1LINE_RX(this->mhspi);
	// __HAL_SPI_ENABLE(this->mhspi);
	HAL_SPI_Init(this->mhspi);

	HAL_SPI_Receive(this->mhspi, (uint8_t*)received_data, size_of_received_data, 100);
	
	HAL_GPIO_WritePin(this->mCSPort, this->mCS, GPIO_PIN_SET);
	HAL_SPI_DeInit(this->mhspi);
	// __HAL_SPI_DISABLE(this->mhspi);
	SPI_1LINE_TX(this->mhspi);
	// __HAL_SPI_ENABLE(this->mhspi);
	HAL_SPI_Init(this->mhspi);
}

/** @} */

#endif /* TLE5012_FRAMEWORK */
