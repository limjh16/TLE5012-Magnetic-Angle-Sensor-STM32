/**
 * @file        spic-stm32.hpp
 * @brief       STM32 PAL for the SPI cover
 * @date        June 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */



#ifndef SPIC_STM32_HPP_
#define SPIC_STM32_HPP_

#include "../../../config/tle5012-conf.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)

#include "../../../pal/spic.hpp"
#include "spi3w-stm32.hpp"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"

/**
 * @addtogroup stm32Pal
 * @{
 */

/**
 * @brief STM32 SPIC class
 * This function is setting the basics for a SPIC and the default spi.
 */

class SPICStm32: virtual public SPIC
{
	private:

		GPIO_TypeDef*      csPort;   //<! \brief chipselect port
		uint32_t           csPin;    //<! \brief chipselect pin for the device
		GPIO_TypeDef*      spiPort;  //<! \brief SPI port
		uint32_t           misoPin;  //<! \brief SPI miso pin
		uint32_t           mosiPin;  //<! \brief SPI mosi pin
		uint32_t           sckPin;   //<! \brief SPI system clock pin
		SPI_HandleTypeDef* hspi;     //<! \brief SPI handle
		SPIClass3W         *spi;     //<! \brief extended SPI class pointer

	public:
					// SPICStm32(uint32_t csPin, GPIO_TypeDef* csPort);
					SPICStm32(uint32_t csPin, GPIO_TypeDef* csPort, uint32_t misoPin, uint32_t mosiPin, uint32_t sckPin, GPIO_TypeDef* spiPort, SPI_HandleTypeDef* hspi);
					~SPICStm32();
		Error_t     init();
		Error_t     deinit();
		Error_t     triggerUpdate();
		Error_t     sendReceive(uint16_t* sent_data, uint16_t size_of_sent_data, uint16_t* received_data, uint16_t size_of_received_data);

};

/** @} */

#endif /** TLE5012_FRAMEWORK **/
#endif /** SPIC_STM32_HPP_ **/
