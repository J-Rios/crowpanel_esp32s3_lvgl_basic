/**
 * @file    main.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-12-30
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

// Standard C++ Libararies
#include <stdio.h>

// FreeRTOS Libraries
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ESP-IDF Framework
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_psram.h"
#include "esp_system.h"

// Project Headers
#include "config/config.h"

/*****************************************************************************/

/* Function Prototypes */

extern "C" { void app_main(void); }

uint32_t psram_get_size();

/*****************************************************************************/

/* Main Function */

void app_main(void)
{
    using namespace ns_const;

    printf("\n\n---------\n\n");
    printf("Project: %s\n", PROJECT_NAME);
    printf("FW Version: v%u.%u.%u (%s %s)\n", FW_APP_VERSION_X,
        FW_APP_VERSION_Y, FW_APP_VERSION_Z, __DATE__, __TIME__);
    printf("ESP-IDF Version: %s\n", esp_get_idf_version());
    printf("PSRAM Size: %lu Bytes\n", (uint32_t)(esp_psram_get_size()));
    printf("\n");

    while(1)
    {
        static uint32_t uptime = 0U;

        printf("Uptime: %lu seconds\n", uptime);
        uptime = uptime+ 1U;

        vTaskDelay(1000U/portTICK_PERIOD_MS);
    }
}

/*****************************************************************************/

/* Auxiliary Functions */

// None

/*****************************************************************************/
