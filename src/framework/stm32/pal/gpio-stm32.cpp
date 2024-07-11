/**
 * @file        gpio-stm32.hpp
 * @brief       STM32 PAL for the GPIO
 * @date        May 2020
 * @copyright   Copyright (c) 2019-2020 Infineon Technologies AG
 *
 * SPDX-License-Identifier: MIT
 */

#include "gpio-stm32.hpp"

#if (TLE5012_FRAMEWORK == TLE5012_FRMWK_STM32)
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"

/**
 * @brief Constructor of the STM32 GPIO class
 *
 * This function is setting the basics for a GPIO.
 *
 */
GPIOStm32::GPIOStm32() : pin(0), port(0), mode(GPIO_MODE_OUTPUT_PP), logic(POSITIVE)
{
}

/**
 * @brief Constructor of the STM32 GPIO class
 *
 * This function is setting the basics for a GPIO. It allows to set the pin number,
 * mode of the pin and the logic level.
 *
 * @param[in]   pin     Number of the desired pin
 * @param[in]   mode    Defines the mode of the pin (INPUT, OUTPUT, etc.)
 * @param[in]   logic   Defines the logic level of the pin
 */
GPIOStm32::GPIOStm32(uint32_t pin, GPIO_TypeDef* port, uint32_t mode, VLogic_t logic): pin(pin), port(port), mode(mode), logic(logic)
{
	this->pin = pin;
	this->port = port;
	this->mode = mode;
	this->logic = logic;
}

/**
 * @brief Initialize the GPIO
 *
 * This function is initializing the chosen pin.
 *
 * @return      GPIOStm32::Error_t
 */
GPIOStm32::Error_t GPIOStm32::init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->pin;
	GPIO_InitStruct.Mode = this->mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->port, &GPIO_InitStruct);
	return OK;
}

/**
 * @brief Changes the GPIO mode
 *
 * This function changes the mode of the chosen pin.
 *
 * @return      GPIOStm32::Error_t
 */
GPIOStm32::Error_t GPIOStm32::changeMode(uint32_t mode)
{
	this->mode = mode;
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = this->pin;
	GPIO_InitStruct.Mode = this->mode;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(this->port, &GPIO_InitStruct);
	return OK;
}



/**
 * @brief Deinitialize the GPIO
 *
 * This function is deinitializing the chosen pin.
 *
 * @return      GPIOStm32::Error_t
 */
GPIOStm32::Error_t GPIOStm32::deinit()
{
	return OK;
}

/**
 * @brief Read GPIO logic level
 *
 * This function reads the logic level of the chosen pin and
 * returns the logic level value.
 *
 * @return      GPIOStm32::VLevel_t
 * @retval      0    = GPIO_LOW
 * @retval      1    = GPIO_HIGH
 */
GPIOStm32::VLevel_t GPIOStm32::read()
{
	return (VLevel_t) HAL_GPIO_ReadPin(this->port, this->pin);
}

/**
 * @brief Set GPIO logic level
 *
 * This functions sets the logic level of the chosen pin.
 *
 * @param[in]   level   Desired logic level of the pin
 * @return      GPIOStm32::Error_t
 */
GPIOStm32::Error_t GPIOStm32::write(VLevel_t level)
{
	HAL_GPIO_WritePin(this->port, this->pin, level ? GPIO_PIN_SET : GPIO_PIN_RESET);
	return OK;
}

/**
 * @brief Enable the GPIO
 *
 * This functions enable the chosen pin. Depending on the chosen logic of the pin
 * it sets the right logic level of the pin.
 *
 * @return      GPIOStm32::Error_t
 */
GPIOStm32::Error_t GPIOStm32::enable()
{
	if(this->logic == POSITIVE){
		HAL_GPIO_WritePin(this->port, this->pin, GPIO_PIN_SET);
	}
	else if(this->logic == NEGATIVE){
		HAL_GPIO_WritePin(this->port, this->pin, GPIO_PIN_RESET);
	}
	return OK;
}

/**
 * @brief Disable the GPIO
 *
 * This functions disables the chosen pin. Depending on the chosen logic of the pin
 * it sets the right logic level of the pin.
 *
 * @return      GPIOStm32::Error_t
 */
GPIOStm32::Error_t GPIOStm32::disable()
{
	if(this->logic == POSITIVE){
		HAL_GPIO_WritePin(this->port, this->pin, GPIO_PIN_SET);
	}
	else if(this->logic == NEGATIVE){
		HAL_GPIO_WritePin(this->port, this->pin, GPIO_PIN_RESET);
	}
	return OK;
}


#endif /** TLE94112_FRAMEWORK **/
