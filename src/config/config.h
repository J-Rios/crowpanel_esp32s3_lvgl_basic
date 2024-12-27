/**
 * @file    config.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-12-27
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * Configuration Data header file.
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

#ifndef CONFIG_H
#define CONFIG_H

/*****************************************************************************/

/* Configurations */

// Default Project Name
#if !defined(SET_PROJECT_NAME)
    #define SET_PROJECT_NAME "esp32_project"
#endif

// Default Firmware Application Version if not provided
#if !defined(SET_FW_APP_VERSION_X)
    #define SET_FW_APP_VERSION_X 1
#endif
#if !defined(SET_FW_APP_VERSION_Y)
    #define SET_FW_APP_VERSION_Y 0
#endif
#if !defined(SET_FW_APP_VERSION_Z)
    #define SET_FW_APP_VERSION_Z 0
#endif

// I2C Pins Touchscreen
#define SDA_FT6236 38
#define SCL_FT6236 39

// GPIO Pins
#define IO_LCD_BACKLIGHT 46
#define IO_BUZZER 20

/*****************************************************************************/

/* System Configuration Constants */

namespace ns_const
{
    /**
     * @brief Default Project Name.
     */
    static constexpr char PROJECT_NAME[] = SET_PROJECT_NAME;

    /**
     * @brief Firmware Application Major Version.
     */
    static constexpr uint8_t FW_APP_VERSION_X
        = static_cast<uint8_t>(SET_FW_APP_VERSION_X);

    /**
     * @brief Firmware Application Minor Version.
     */
    static constexpr uint8_t FW_APP_VERSION_Y
        = static_cast<uint8_t>(SET_FW_APP_VERSION_Y);

    /**
     * @brief Firmware Application Patch Version.
     */
    static constexpr uint8_t FW_APP_VERSION_Z
        = static_cast<uint8_t>(SET_FW_APP_VERSION_Z);

    /**
     * @brief Default Debug Serial Port Speed.
     */
    static constexpr uint32_t DEFAULT_UART_BAUD_RATE = 115200U;

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

/* Include Guard Close */

#endif /* CONFIG_H */
