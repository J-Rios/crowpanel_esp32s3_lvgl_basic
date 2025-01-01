/**
 * @file    pwm.cpp
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
 * Copyright (c) 2025 Jose Miguel Rios Rubio
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
#include "pwm.h"

// ESP-IDF Framework
#include "driver/ledc.h"
#include "esp_err.h"
//#include "esp32-hal.h"
#include "soc/soc_caps.h"

/*****************************************************************************/

/* Defines */

#ifdef SOC_LEDC_SUPPORT_HS_MODE
#define LEDC_CHANNELS           (SOC_LEDC_CHANNEL_NUM<<1)
#else
#define LEDC_CHANNELS           (SOC_LEDC_CHANNEL_NUM)
#endif

// Use XTAL clock if possible to avoid timer frequency error when setting
// APB clock < 80 Mhz
// Need to be fixed in ESP-IDF
#ifdef SOC_LEDC_SUPPORT_XTAL_CLOCK
#define LEDC_DEFAULT_CLK        LEDC_USE_XTAL_CLK
#else
#define LEDC_DEFAULT_CLK        LEDC_AUTO_CLK
#endif

#define LEDC_MAX_BIT_WIDTH      SOC_LEDC_TIMER_BIT_WIDTH

/*****************************************************************************/

/* In-Scope Attributes */

/* LEDC Chan to Group/Channel/Timer Mapping
 * - ledc: 0  => Group: 0, Channel: 0, Timer: 0
 * - ledc: 1  => Group: 0, Channel: 1, Timer: 0
 * - ledc: 2  => Group: 0, Channel: 2, Timer: 1
 * - ledc: 3  => Group: 0, Channel: 3, Timer: 1
 * - ledc: 4  => Group: 0, Channel: 4, Timer: 2
 * - ledc: 5  => Group: 0, Channel: 5, Timer: 2
 * - ledc: 6  => Group: 0, Channel: 6, Timer: 3
 * - ledc: 7  => Group: 0, Channel: 7, Timer: 3
 * - ledc: 8  => Group: 1, Channel: 0, Timer: 0
 * - ledc: 9  => Group: 1, Channel: 1, Timer: 0
 * - ledc: 10 => Group: 1, Channel: 2, Timer: 1
 * - ledc: 11 => Group: 1, Channel: 3, Timer: 1
 * - ledc: 12 => Group: 1, Channel: 4, Timer: 2
 * - ledc: 13 => Group: 1, Channel: 5, Timer: 2
 * - ledc: 14 => Group: 1, Channel: 6, Timer: 3
 * - ledc: 15 => Group: 1, Channel: 7, Timer: 3
 */
uint8_t channels_resolution[LEDC_CHANNELS] = {0};

/*****************************************************************************/

/* Public Functions */

uint32_t ledc_setup(uint8_t chan, uint32_t freq, uint8_t bit_num)
{
    if (chan >= LEDC_CHANNELS || bit_num > LEDC_MAX_BIT_WIDTH)
    {   return 0U;   }

    ledc_mode_t group = static_cast<ledc_mode_t>(chan / 8U);
    ledc_timer_t timer = static_cast<ledc_timer_t>((chan / 2U) % 4U);

    static ledc_timer_config_t ledc_timer;
    ledc_timer.speed_mode = group;
    ledc_timer.timer_num = timer;
    ledc_timer.duty_resolution = static_cast<ledc_timer_bit_t>(bit_num);
    ledc_timer.freq_hz = freq;
    ledc_timer.clk_cfg = LEDC_DEFAULT_CLK;

    if (ledc_timer_config(&ledc_timer) != ESP_OK)
    {   return 0U;   }

    channels_resolution[chan] = bit_num;
    return ledc_get_freq(group, timer);
}

void ledc_attach_pin(uint8_t pin, uint8_t chan)
{
    if(chan >= LEDC_CHANNELS)
    {   return;   }

    ledc_channel_t channel = static_cast<ledc_channel_t>(chan % 8U);
    ledc_mode_t group = static_cast<ledc_mode_t>(chan / 8U);
    ledc_timer_t timer = static_cast<ledc_timer_t>((chan / 2U) % 4U);

    uint32_t duty = ledc_get_duty(group, channel);

    static ledc_channel_config_t ledc_channel;
    ledc_channel.speed_mode = group;
    ledc_channel.channel = channel;
    ledc_channel.timer_sel = timer;
    ledc_channel.intr_type = LEDC_INTR_DISABLE;
    ledc_channel.gpio_num = pin;
    ledc_channel.duty = duty;
    ledc_channel.hpoint = 0;

    ledc_channel_config(&ledc_channel);
}

void ledc_write(uint8_t chan, uint32_t duty)
{
    if (chan >= LEDC_CHANNELS)
    {   return;   }

    ledc_channel_t channel = static_cast<ledc_channel_t>(chan % 8U);
    ledc_mode_t group = static_cast<ledc_mode_t>(chan / 8U);

    //Fixing if all bits in resolution is set = LEDC FULL ON
    uint32_t max_duty = (1 << channels_resolution[chan]) - 1;

    if ((duty == max_duty) && (max_duty != 1))
    {   duty = max_duty + 1;   }

    ledc_set_duty(group, channel, duty);
    ledc_update_duty(group, channel);
}

/*****************************************************************************/
