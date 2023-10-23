/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MYGPIO_H_
#define MYGPIO_H_

/*!
 * \brief Test the gpio function
 */
void Gpio_Test(void);

/*!
 * \brief Module de-initialitation
 */
void Gpio_Deinit(void);

/*!
 * \brief Module initialization. Note: function will block!
 */
void Gpio_Init(void);

#endif /* MYGPIO_H_ */
