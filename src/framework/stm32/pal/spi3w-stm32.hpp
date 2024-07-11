/*
 * @file        spi3w-ino.hpp
 * @brief       TLE5012 STM32 HAL SPI Wrapper
 * @date        October 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */
#include "../../../config/tle5012-conf.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)

#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"

/**
 * @addtogroup arduinoPal
 * @{
 */

/**
 * @brief STM32 SPIClass3W is a wrapper for STM32's SPI HAL 
 * The extension allows the use of 3wire SSC SPI interfaces for configured STM32 boards.
 *
 * @attention keep in mind, you can set more different chipselect pins, but
 * you can not separate more than 4 sensors data streams from each other
 * 
 */

#ifndef SPI3W_STM32_HPP
#define SPI3W_STM32_HPP

#define SPI3W_STM32     5

#define MAX_SLAVE_NUM    4              //!< Maximum numbers of slaves on one SPI bus
// #define SPEED            1000000U       //!< default speed of SPI transfer

class SPIClass3W
{

	public:
//		#define SPI3W_STM32 SPI3W_STM32


		GPIO_TypeDef* mCSPort;  //!< Port for chip select
		uint32_t      mCS;      //!< Pin for chip select
		uint8_t       mSpiNum;  //!< Number of used SPI channel

				SPIClass3W();
				~SPIClass3W();
		void    begin(uint32_t miso, uint32_t mosi, uint32_t sck, GPIO_TypeDef* spiPort, SPI_HandleTypeDef* hspi, uint32_t cs, GPIO_TypeDef* csPort);
		void    setCSPin(uint32_t cs, GPIO_TypeDef* csPort);
		void    sendReceiveSpi(uint16_t* sent_data, uint16_t size_of_sent_data, uint16_t* received_data, uint16_t size_of_received_data);

	private:

		GPIO_TypeDef*      mSPIPort;  //!< Port for SPI pins
		SPI_HandleTypeDef* mhspi;     //!< SPI handle
		uint32_t           mMOSI;     //!< Pin for SPI MOSI
		uint32_t           mMISO;     //!< Pin for SPI MISO
		uint32_t           mSCK;      //!< Pin for SPI System Clock


};

/**
 * @brief define a new SPI3W macro for handling more than the default SPI channel
 * 
 */
extern SPIClass3W SPI3W;
#if (NUM_SPI > 1)
	extern SPIClass3W SPI3W1;
#	if (NUM_SPI > 2)
		extern SPIClass3W SPI3W2;
#		if (NUM_SPI > 3)
			extern SPIClass3W SPI3W3;
#			if (NUM_SPI > 4)
				extern SPIClass3W SPI3W4;
#			endif
#		endif
#	endif
#endif

/** @} */

#endif /* SPI3W_STM32_HPP */
#endif /* TLE5012_FRAMEWORK */
