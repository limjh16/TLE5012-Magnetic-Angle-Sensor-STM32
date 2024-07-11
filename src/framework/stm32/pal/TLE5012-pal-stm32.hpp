/*!
 * \file        TLE5012-pal-stm32.hpp
 * \name        TLE5012-pal-stm32.hpp - STM32 Hardware Abstraction Layer
 * \author      Infineon Technologies AG
 * \copyright   2020 Infineon Technologies AG
 * \version     3.1.0
 * \ref         stm32Pal
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef TLE5012_PAL_STM32_HPP_
#define TLE5012_PAL_STM32_HPP_

#include "../../../config/tle5012-conf.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)

/**
 * @addtogroup stm32Pal
 *
 * @{
 */

#include "stm32f4xx.h"
#include "../../../corelib/TLE5012b.hpp"
#include "spic-stm32.hpp"
#include "gpio-stm32.hpp"
#include "spic-stm32.hpp"

// //! Check for XMC mcu family */
// #define PIN_SPI_EN    UNUSED_PIN  /*!< TLE5012 with any other PCB has no switch on/off */

/**
 * @brief represents a basic TLE5012b STM32 class.
 *
 * This class provides a simple API for connecting STM32s.
 *
 * @see Tle5012
 */

class Tle5012Stm32: virtual public Tle5012b
{

	public:

		uint8_t     mSpiNum = 0;          //!< Number of used SPI channel

					// Tle5012Stm32();
					// Tle5012Stm32(uint8_t csPin, slaveNum slave=TLE5012B_S0);
					Tle5012Stm32(uint32_t csPin, GPIO_TypeDef* csPort, uint32_t misoPin, uint32_t mosiPin, uint32_t sckPin, GPIO_TypeDef* spiPort, SPI_HandleTypeDef* hspi, slaveNum slave);
		errorTypes  begin();

};

/**
 * @}
 */

#endif /** TLE5012_FRAMEWORK **/
#endif /** TLE5012_PAL_STM32_HPP_ **/
