/**
 * @file    i2c.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2025-01-01
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESP32 I2C controller.
 *
 * @section LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2024 Jose Miguel Rios Rubio
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*****************************************************************************/

/* Include Guard */

#ifndef CONTROLLER_I2C_H
#define CONTROLLER_I2C_H

/*****************************************************************************/

/* Libraries */

// ESP-IDF Framework
#include "driver/i2c.h"

/*****************************************************************************/

/* Functions */

void i2c_setup(const i2c_port_t i2c_port, const uint8_t io_sda,
    const uint8_t io_scl, const uint32_t frequency = 100000UL);

bool i2c_write(const i2c_port_t i2c_port, const uint16_t slave_address,
    const uint8_t data_to_write);

bool i2c_read_register(const i2c_port_t i2c_port, const uint16_t slave_address,
    const uint8_t reg_address, uint8_t* data_read);

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONTROLLER_I2C_H */
