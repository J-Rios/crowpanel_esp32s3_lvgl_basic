/**
 * @file    main.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-12-27
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * Project Main file.
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

// Framework Library
#include <Arduino.h>

// Standard C++ Libraries
#include <cstdint>

// Graphic Libraies
#include <lvgl.h>

// Project Headers
#include "config/config.h"
#include "touch_panel/driver_ft6236.h"

/*****************************************************************************/

/* Function Prototypes */

// Initialization
void serial_init();
void touch_init();

// Management
void manage_uptime();
void manage_touch();

/*****************************************************************************/

/* Global Elements */

uint16_t touch_x = 0U;
uint16_t touch_y = 0U;

/*****************************************************************************/

/* Setup & Loop Functions */

void setup()
{
    // Serial Initialization
    serial_init();

    // Touchscreen Initialization
    touch_init();

    Serial.printf("\n");
}

void loop()
{
    manage_uptime();
    manage_touch();

    delay(10);
}

/*****************************************************************************/

/* Initialization Functions */

void serial_init()
{
    using namespace ns_const;

    Serial.begin(DEFAULT_UART_BAUD_RATE);

    Serial.printf("\n\n---------\n\n");
    Serial.printf("Project: %s\n", PROJECT_NAME);
    Serial.printf("FW Version: v%u.%u.%u (%s %s)\n", FW_APP_VERSION_X,
        FW_APP_VERSION_Y, FW_APP_VERSION_Z, __DATE__, __TIME__);
    Serial.printf("ESP-IDF Version: %s\n", esp_get_idf_version());
    Serial.printf("PSRAM Size: %lu\n", ESP.getPsramSize());
    Serial.printf("\n");

    Serial.printf("[OK] Serial init\n");
}

void touch_init()
{
    uint8_t error = touch_panel_init(SDA_FT6236, SCL_FT6236);
    if (error == 0)
    {   Serial.printf("[OK] Touch init\n");   }
    else
    {   Serial.printf("[FAIL] Touch init (error code %d)\n", (int)(error));   }
}

/*****************************************************************************/

/* Management Functions */

void manage_uptime()
{
    static const uint16_t T_INCREASE_UPTIME_MS = 1000U;
    static uint32_t uptime = 0U;
    static unsigned long t0 = millis();

    if (millis() - t0 >= T_INCREASE_UPTIME_MS)
    {
        Serial.printf("Uptime: %lu seconds\n", uptime);
        uptime = uptime + 1U;
        t0 = millis();
    }
}

void manage_touch()
{
    int pos[2] = {0, 0};

    touch_panel_get_position(pos);

    if ( (pos[0] > 0) && (pos[1] > 0) )
    {
        touch_x = ns_const::SCREEN_WIDTH - static_cast<uint16_t>(pos[1]);
        touch_y = static_cast<uint16_t>(pos[0]);
        Serial.printf("x, y: %d, %d\n", touch_x, touch_y);
    }
}

/*****************************************************************************/
