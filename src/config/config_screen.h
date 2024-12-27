/**
 * @file    config_screen.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-12-27
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * Screen configuration Data header file.
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

#ifndef CONFIG_SCREEN_H
#define CONFIG_SCREEN_H

/*****************************************************************************/

/* Libraries */

// Standard C++ Libraries
#include <cstdint>

// Graphic Screen Driver
#include <LovyanGFX.hpp>

/*****************************************************************************/

/* Screen Configuration Constants */

namespace ns_const
{
    /**
     * @brief Screen Width.
     */
    static constexpr uint16_t SCREEN_WIDTH  = 480U;

    /**
     * @brief Screen Height.
     */
    static constexpr uint16_t SCREEN_HEIGHT = 320U;

    /**
     * @brief Screen Buffer Size.
     */
    static constexpr uint32_t SCREEN_BUFFER_SIZE
        = (SCREEN_WIDTH * SCREEN_HEIGHT / 5U);
}

/*****************************************************************************/

/* Screen Setup */

class LGFX : public lgfx::LGFX_Device
{
    public:
        LGFX(void)
        {
            {
                auto cfg = _bus_instance.config();
                cfg.port = 0;
                cfg.freq_write = 80000000;
                cfg.pin_wr = 18;
                cfg.pin_rd = 48;
                cfg.pin_rs = 45;

                cfg.pin_d0 = 47;
                cfg.pin_d1 = 21;
                cfg.pin_d2 = 14;
                cfg.pin_d3 = 13;
                cfg.pin_d4 = 12;
                cfg.pin_d5 = 11;
                cfg.pin_d6 = 10;
                cfg.pin_d7 = 9;
                cfg.pin_d8 = 3;
                cfg.pin_d9 = 8;
                cfg.pin_d10 = 16;
                cfg.pin_d11 = 15;
                cfg.pin_d12 = 7;
                cfg.pin_d13 = 6;
                cfg.pin_d14 = 5;
                cfg.pin_d15 = 4;
                _bus_instance.config(cfg);
                _panel_instance.setBus(&_bus_instance);
            }

            {
                auto cfg = _panel_instance.config();

                cfg.pin_cs = -1;
                cfg.pin_rst = -1;
                cfg.pin_busy = -1;
                cfg.memory_width = 320;
                cfg.memory_height = 480;
                cfg.panel_width = 320;
                cfg.panel_height = 480;
                cfg.offset_x = 0;
                cfg.offset_y = 0;
                cfg.offset_rotation = 2;
                cfg.dummy_read_pixel = 8;
                cfg.dummy_read_bits = 1;
                cfg.readable = true;
                cfg.invert = false;
                cfg.rgb_order = false;
                cfg.dlen_16bit = true;
                cfg.bus_shared = true;
                _panel_instance.config(cfg);
            }
            setPanel(&_panel_instance);
        }

    private:
        lgfx::Panel_ILI9488 _panel_instance;
        lgfx::Bus_Parallel16 _bus_instance;
};

/*****************************************************************************/

/* Include Guard Close */

#endif /* CONFIG_SCREEN_H */
