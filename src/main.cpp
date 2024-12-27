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
#include "config/config_screen.h"
#include "buzzer/driver_passive_buzzer.h"
#include "touch_panel/driver_ft6236.h"

/*****************************************************************************/

/* Function Prototypes */

// Initialization
void serial_init();
void buzzer_init();
void touch_init();
void screen_init();
void display_init();

// Management
void manage_uptime();
void manage_buzzer();
void manage_ui();

// LVGL Callbacks
void display_refresh(lv_disp_drv_t* disp_drv, const lv_area_t* area,
    lv_color_t* color_p);
void display_manage_touch(lv_indev_drv_t* indev_driver, lv_indev_data_t* data);

// UI Draw
void ui_draw_screen_1();

/*****************************************************************************/

/* Global Elements */

// Buzzer
PassiveBuzzer Buzzer(IO_BUZZER, 0U, ns_const::BUZZER_MIN_FREQ_HZ,
    ns_const::BUZZER_MAX_FREQ_HZ, ns_const::BUZZER_DUTY_CYCLE);

// Screen Device
LGFX Screen;

// Touch Panel
uint16_t touch_x = 0U;
uint16_t touch_y = 0U;

// UI Render Buffer
lv_disp_draw_buf_t draw_buf;
lv_color_t buf[ns_const::SCREEN_BUFFER_SIZE];

// UI Draw Texts buffer
static const uint32_t MAX_TEXT_LENGTH = 1024U;
char text[MAX_TEXT_LENGTH];

// UI Elements
lv_obj_t* ui_info_box = nullptr;
lv_obj_t* ui_label_info = nullptr;
lv_obj_t* label_uptime = nullptr;
lv_obj_t* label_touch = nullptr;

/*****************************************************************************/

/* Setup & Loop Functions */

void setup()
{
    // Initializations
    serial_init();
    buzzer_init();
    touch_init();
    screen_init();
    display_init();

    // Draw First Screen
    ui_draw_screen_1();

    Serial.printf("\n");
}

void loop()
{
    manage_uptime();
    manage_buzzer();
    manage_ui();
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
    Serial.printf("PSRAM Size: %lu Bytes\n", ESP.getPsramSize());
    Serial.printf("\n");

    Serial.printf("[OK] Serial init\n");
}

void buzzer_init()
{
    pinMode(IO_BUZZER, OUTPUT);
    digitalWrite(IO_BUZZER, LOW);
    Buzzer.init();
    Buzzer.beep(2700U, 1000U);
}

void touch_init()
{
    uint8_t error = touch_panel_init(SDA_FT6236, SCL_FT6236);
    if (error == 0)
    {   Serial.printf("[OK] Touch init\n");   }
    else
    {   Serial.printf("[FAIL] Touch init (error code %d)\n", (int)(error));   }
}

void screen_init()
{
    Screen.begin();
    Screen.setRotation(1U);
    Screen.fillScreen(TFT_BLACK);
    delay(500);
    pinMode(IO_LCD_BACKLIGHT, OUTPUT);
    digitalWrite(IO_LCD_BACKLIGHT, HIGH);
    Serial.printf("[OK] Screen init\n");
}

void display_init()
{
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, ns_const::SCREEN_BUFFER_SIZE);

    // Setup Display
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = ns_const::SCREEN_WIDTH;
    disp_drv.ver_res = ns_const::SCREEN_HEIGHT;
    disp_drv.flush_cb = display_refresh;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Setup Touch Control Callback
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = display_manage_touch;
    lv_indev_drv_register(&indev_drv);

    Serial.printf("[OK] Display init\n");
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
        snprintf(text, MAX_TEXT_LENGTH, "Uptime: %lu seconds", uptime);
        lv_label_set_text(label_uptime, text);
        Serial.printf("%s\n", text);
        uptime = uptime + 1U;
        t0 = millis();
    }
}

void manage_buzzer()
{
    Buzzer.process();
}

void manage_ui()
{
    lv_timer_handler();
}

/*****************************************************************************/

/* LVGL UI Callbacks */

void display_refresh(lv_disp_drv_t* disp_drv, const lv_area_t* area,
    lv_color_t* color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1U);
    uint32_t h = (area->y2 - area->y1 + 1U);

    Screen.startWrite();
    Screen.setAddrWindow(area->x1, area->y1, w, h);
    Screen.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
    Screen.endWrite();
    lv_disp_flush_ready(disp_drv);
}

void display_manage_touch(lv_indev_drv_t* indev_driver, lv_indev_data_t* data)
{
    int pos[2] = {0, 0};

    touch_panel_get_position(pos);

    if ( (pos[0] > 0) && (pos[1] > 0) )
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = ns_const::SCREEN_WIDTH-pos[1];
        data->point.y = pos[0];

        touch_x = static_cast<int>(data->point.x);
        touch_y = static_cast<int>(data->point.y);

        snprintf(text, MAX_TEXT_LENGTH, "Touch X, Y: %u, %u",
            touch_x, touch_y);
        lv_label_set_text(label_touch, text);
        Serial.printf("%s\n", text);
    }
    else
    {   data->state = LV_INDEV_STATE_REL;   }
}

/*****************************************************************************/

/* LVGL UI Screen Draws */

void ui_draw_screen_1()
{
    using namespace ns_const;

    static const lv_color_t LABEL_BORDER_COLOR = lv_color_hex(0x00000000U);
    static const lv_color_t LABEL_TEXT_COLOR = lv_color_hex(0x00000000U);

    // Container
    ui_info_box = lv_obj_create(lv_scr_act());
    lv_obj_set_flex_flow(ui_info_box, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_size(ui_info_box, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(ui_info_box, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(ui_info_box, lv_color_hex(0xDDDDDD), LV_PART_MAIN);
    lv_obj_set_style_pad_all(ui_info_box, 10, LV_PART_MAIN);

    /* Label Info */
    ui_label_info = lv_label_create(ui_info_box);
    lv_obj_set_style_text_color(ui_label_info, LABEL_TEXT_COLOR, LV_PART_MAIN);
    snprintf(text, MAX_TEXT_LENGTH,
        "Project: %s\n"
        "FW Version: v%d.%d.%d (%s %s)\n"
        "ESP-IDF Version: %s\n"
        "PSRAM Size: %lu Bytes",
        PROJECT_NAME, (int)(FW_APP_VERSION_X), (int)(FW_APP_VERSION_Y),
        (int)(FW_APP_VERSION_Z), __DATE__, __TIME__, esp_get_idf_version(),
        ESP.getPsramSize());
    lv_label_set_text(ui_label_info, text);

    /* Label Uptime */
    label_uptime = lv_label_create(ui_info_box);
    snprintf(text, MAX_TEXT_LENGTH, "Uptime: 0 seconds");
    lv_label_set_text(label_uptime, text);

    /* Label Touch */
    label_touch = lv_label_create(lv_scr_act());
    lv_obj_align(label_touch, LV_ALIGN_BOTTOM_MID, 0, -15);
    lv_obj_set_style_border_width(label_touch, 2, LV_PART_MAIN);
    lv_obj_set_style_text_color(label_touch, LABEL_TEXT_COLOR, LV_PART_MAIN);
    snprintf(text, MAX_TEXT_LENGTH, "Touch X, Y: 000, 000");
    lv_label_set_text(label_touch, text);
}

/*****************************************************************************/
