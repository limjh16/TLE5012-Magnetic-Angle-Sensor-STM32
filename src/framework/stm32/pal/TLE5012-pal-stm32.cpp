/** 
 * @file        TLE5012-pal-stm32.cpp
 * @brief       TLE5012 Stm32 Hardware Platforms
 * @date        October 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "TLE5012-pal-stm32.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)

/**
 * @addtogroup stm32Pal
 */

/**
 * Construct a new Tle5012Stm32::Tle5012Stm32 object with individual SPI and pin assignment
 * Use this if:
 * - you use different SPI channels than default
 * - you use a software SPI with different pin settings than default
 * 
 * @param csPin    pin number of the CS pin
 * @param misoPin  MISO pin for the SPI/SSC interface
 * @param mosiPin  MOSI pin for the SPI/SSC interface
 * @param sckPin   system clock pin for external sensor clock setting
 * @param slave    optional sensor slave setting
 */
Tle5012Stm32::Tle5012Stm32(uint32_t csPin, GPIO_TypeDef* csPort, uint32_t misoPin, uint32_t mosiPin, uint32_t sckPin, GPIO_TypeDef* spiPort, SPI_HandleTypeDef* hspi, slaveNum slave):Tle5012b()
{
	Tle5012b::mSlave = slave;
	Tle5012b::sBus = new SPICStm32(csPin,csPort,misoPin,mosiPin,sckPin,spiPort,hspi);
}

/**
 * @brief begin method with default assignments for the SPI bus
 * and pin setting.
 * The EN pin is only available for the Sensor2go kit, therefore it will be NULL.
 * 
 * @return errorTypes 
 */
errorTypes Tle5012Stm32::begin(void)
{
	#define PIN_SPI_EN    UNUSED_PIN           /*!< TLE5012 Sensor2Go Kit has a switch on/off pin */

	// init helper libs
	sBus->init();
	Tle5012b::en = NULL;
	// start sensor
	enableSensor();
	writeSlaveNumber(Tle5012b::mSlave);
	// initial CRC check, should be = 0
	return (readBlockCRC());
}

#endif /** TLE5012_FRAMEWORK **/
/** @} */
