/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef SENSOR_H_
#define SENSOR_H_

/*!
 * \brief Sensor module initialization
 */
void Sensor_Deinit(void);

/*!
 * \brief Sensor module de-initialization
 */
void Sensor_Init(void);

/*!
 * \brief Getter function for temperature */
float GetSht31Temp(void);

#endif /* SENSOR_H_ */
