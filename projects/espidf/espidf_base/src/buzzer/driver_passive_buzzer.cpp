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

/* Libraries */

// Library Header
#include "driver_passive_buzzer.h"

// ESP-IDF Framework
#include "controller/pwm/pwm.h"
#include "esp_timer.h"

/*****************************************************************************/

/* In-Scope Functions */

static inline uint32_t millis();

/*****************************************************************************/

/* Public Methods */

PassiveBuzzer::PassiveBuzzer(const int8_t pin, const uint8_t pwm_channel,
    const uint16_t min_freq, const uint16_t max_freq, const uint8_t duty_cycle)
    :
        _pin{pin}, _pwm_channel{pwm_channel}, _min_freq{min_freq},
        _max_freq{max_freq}, _duty_cycle{duty_cycle}
{}

void PassiveBuzzer::init()
{
    const int pwm_channel = 0;
    printf("LEDC setup\n");
    ledc_setup(pwm_channel, MIN_FREQ_HZ, 10U);
    printf("Attach Pin\n");
    ledc_attach_pin(_pin, pwm_channel);
    printf("Beep OFF\n");
    beep_off();
    is_initialized = true;
}

void PassiveBuzzer::beep_off()
{
    beeping = false;
    beep_request = false;
    beep_request_time = 0U;
    t0 = 0U;
    set_pwm(0U);
}

void PassiveBuzzer::beep(uint16_t frequency, const uint16_t beep_time_ms)
{
    // Do nothing if component is not initialized
    if (is_initialized == false)
    {   return;   }
    if (beeping)
    {   return;   }

    // Stop any beep
    beep_off();
    beeping = false;
    beep_request = true;
    beep_request_time = (unsigned long)(beep_time_ms);

    // Setup new frequency
    if (frequency < _min_freq)
    {   frequency = _min_freq;   }
    if (frequency > _max_freq)
    {   frequency = _max_freq;   }
    ledc_setup(_pwm_channel, frequency, 10U);
}

void PassiveBuzzer::process()
{
    // Do nothing if component is not initialized
    if (is_initialized == false)
    {   return;   }

    // Do nothing if there is no Beep request
    if (beep_request == false)
    {   return;   }

    // Handle beep start
    if (beeping == false)
    {
        set_pwm(_duty_cycle);
        t0 = millis();
        beeping = true;
        return;
    }

    // Do nothing if time to stop beeping has not come
    if (millis() - t0 < beep_request_time)
    {   return;   }

    // Handle beep stop
    beep_off();
    beeping = false;
    beep_request = false;
}

/*****************************************************************************/

/* Private Methods */

void PassiveBuzzer::set_pwm(uint8_t percentage)
{
    if(percentage > 100)
    {   percentage = 100U;   }

    uint8_t duty_cycle_value = (percentage * 255U) / 100U;
    ledc_write(_pwm_channel, duty_cycle_value);
}

/*****************************************************************************/

/* Private Methods - Millis */

static inline uint32_t millis()
{
    return static_cast<uint32_t>(esp_timer_get_time() / 1000LL);
}

/*****************************************************************************/
