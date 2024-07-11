/*
 * run.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: limjh
 */

#include "config/tle5012-conf.hpp"
#include "corelib/TLE5012b.hpp"
#include "framework/stm32/wrapper/TLE5012-platf-stm32.hpp"
#include "main.h"
#include "spi.h"
#include "SEGGER_RTT.h"
#include <stdio.h>

Tle5012Stm32 Tle5012Sensor = Tle5012Stm32(
		// SPIbus,
		(uint32_t) TLE5012_CS_Pin,
		TLE5012_CS_GPIO_Port,
		(uint32_t) NULL, // MISO
		(uint32_t) SPI1_MOSI_Pin,
		(uint32_t) SPI1_SCK_Pin,
		SPI1_MOSI_GPIO_Port, &hspi1, Tle5012b::TLE5012B_S0);
errorTypes checkError = NO_ERROR;

void setup() {

	SEGGER_RTT_Init();
	printf("RTT Inited\r\n");

	checkError = Tle5012Sensor.begin();
	printf("Sensor Init, checkError: %u\r\n", checkError);
}

void loop() {
	double d = 0.0;
	double r = 0.0;
	double s = 0.0;
	double t = 0;
	int16_t b = 0;

	Tle5012Sensor.getAngleValue(d);
	Tle5012Sensor.getNumRevolutions(b);
	Tle5012Sensor.getAngleSpeed(s);
	Tle5012Sensor.getAngleRange(r);
	Tle5012Sensor.getTemperature(t);

	printf("Temperature: %.2f degC | ", t);
	printf("angle: %.2f deg | ", d);
	printf("range: %.2f | ", r);
	printf("speed: %.2f | ", s);
	printf("Revolution: %d\r\n", b);

	HAL_Delay(1000);
}

