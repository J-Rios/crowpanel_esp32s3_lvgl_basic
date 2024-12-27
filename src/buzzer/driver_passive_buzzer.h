/**
 * @file    driver_passive_buzzer.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-12-27
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * Passive Buzzer driver.
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

#ifndef DRIVER_PASSIVE_BUZZER_H
#define DRIVER_PASSIVE_BUZZER_H

/*****************************************************************************/

/* Libraries */

// I2C Driver
#include <Wire.h>

/*****************************************************************************/

/* Class Interface */

class PassiveBuzzer
{
    public:

        PassiveBuzzer(const int8_t pin, const uint8_t pwm_channel=0U,
            const uint16_t min_freq=MIN_FREQ_HZ,
            const uint16_t max_freq=MAX_FREQ_HZ,
            const uint8_t duty_cycle=DUTY_CYCLE);

        void init();

        void beep(uint16_t frequency, const uint16_t beep_time_ms);

        void beep_off();

        void process();

    /******************************************************************/

    private:

        static constexpr const uint16_t MIN_FREQ_HZ = 0U;
        static constexpr const uint16_t MAX_FREQ_HZ = 2700U;
        static constexpr const uint8_t DUTY_CYCLE = 50U;

        const int8_t _pin;
        const uint8_t _pwm_channel;
        const uint16_t _min_freq;
        const uint16_t _max_freq;
        const uint8_t _duty_cycle;

        bool is_initialized = false;
        bool beep_request = false;
        bool beeping = false;
        unsigned long beep_request_time = 0U;
        unsigned long t0 = 0U;

        void set_pwm(uint8_t percentage);
};

/*****************************************************************************/

/* Include Guard Close */

#endif /* DRIVER_PASSIVE_BUZZER_H */
