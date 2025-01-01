/**
 * @file    pwm.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2025-01-01
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESP32 PWM controller.
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

#ifndef CONTROLLER_PWM_H
#define CONTROLLER_PWM_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>

/*****************************************************************************/

/* Functions */

uint32_t ledc_setup(uint8_t chan, uint32_t freq, uint8_t bit_num);

void ledc_attach_pin(uint8_t pin, uint8_t chan);

void ledc_write(uint8_t chan, uint32_t duty);

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONTROLLER_PWM_H */
