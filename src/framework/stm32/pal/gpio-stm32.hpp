/**
 * @file        gpio-stm32.hpp
 * @brief       STM32 PAL for the GPIO
 * @date        May 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */


#ifndef GPIO_STM32_HPP_
#define GPIO_STM32_HPP_

#include "../../../config/tle5012-conf.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)

#include "../../../corelib/TLE5012b.hpp"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"

/**
 * @addtogroup stm32Pal
 * @{
 */


/**
 * @brief STM32 GPIO class
 *
 */
class GPIOStm32: virtual public GPIO
{
private:
	#define     UNUSED_PIN    0xFF  /**< Unused pin */
	uint32_t    pin;
	GPIO_TypeDef*		port;
	uint32_t	mode;
	VLogic_t    logic;

public:
				GPIOStm32();
				GPIOStm32(uint32_t pin, GPIO_TypeDef* port, uint32_t mode, VLogic_t logic);
				~GPIOStm32();
	Error_t     init();
	Error_t     changeMode(uint32_t mode);
	Error_t     deinit();
	VLevel_t    read();
	Error_t     write(VLevel_t level);
	Error_t     enable();
	Error_t     disable();
};
/** @} */

#endif /** TLE5012_FRAMEWORK **/
#endif /** GPIO_STM32_HPP_ **/
