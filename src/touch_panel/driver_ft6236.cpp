/**
 * @file    driver_ft6236.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-12-27
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * FT6236 Touch panel I2C controller driver.
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
#include "driver_ft6236.h"

/*****************************************************************************/

/* In-Scope Constants */

// I2C Address
static constexpr uint8_t TOUCH_I2C_ADDRESS = 0x38U;

// Touch X & Y Position Registers
static constexpr uint8_t TOUCH_REG_XL = 0x04U;
static constexpr uint8_t TOUCH_REG_XH = 0x03U;
static constexpr uint8_t TOUCH_REG_YL = 0x06U;
static constexpr uint8_t TOUCH_REG_YH = 0x05U;

/*****************************************************************************/

/* In-Scope Function Prototype */

static int touch_panel_read_reg(const uint8_t reg);
static int touch_panel_get_point_x();
static int touch_panel_get_point_y();

/*****************************************************************************/

/* Public Functions */

int touch_panel_init(const uint8_t pin_i2c_sda, const uint8_t pin_i2c_scl)
{
    Wire.begin(static_cast<int>(pin_i2c_sda), static_cast<int>(pin_i2c_scl));
    Wire.beginTransmission(TOUCH_I2C_ADDRESS);
    return Wire.endTransmission();
}

void touch_panel_get_position(int pos[2])
{
    int xl = 0;
    int xh = 0;
    int yl = 0;
    int yh = 0;

    xh = touch_panel_read_reg(TOUCH_REG_XH);
    if(xh >> 6 == 1)
    {
        pos[0] = -1;
        pos[1] = -1;
        return;
    }
    xl = touch_panel_read_reg(TOUCH_REG_XL);
    yh = touch_panel_read_reg(TOUCH_REG_YH);
    yl = touch_panel_read_reg(TOUCH_REG_YL);

    pos[0] = ( ( (xh & 0x0F) << 8 ) | xl );
    pos[1] = ( ( (yh & 0x0F) << 8 ) | yl );
}

int touch_panel_read_reg(const uint8_t reg)
{
    int data = 0;

    // Write Request
    Wire.beginTransmission(TOUCH_I2C_ADDRESS);
    Wire.write(reg);
    Wire.endTransmission();

    // Read Response
    Wire.requestFrom(static_cast<int>(TOUCH_I2C_ADDRESS), 1);
    if (Wire.available())
    {
        data = Wire.read();
    }

    return data;
}

/*****************************************************************************/

/* Private Functions */

int touch_panel_get_point_x()
{
    int xl = 0;
    int xh = 0;

    xh = touch_panel_read_reg(TOUCH_REG_XH);
    if(xh >> 6 == 1)
    {   return -1;   }
    xl = touch_panel_read_reg(TOUCH_REG_XL);

    return ( ( (xh & 0x0F) << 8 ) | xl );
}

int touch_panel_get_point_y()
{
    int yl = 0;
    int yh = 0;

    yh = touch_panel_read_reg(TOUCH_REG_YH);
    yl = touch_panel_read_reg(TOUCH_REG_YL);

    return ( ( (yh & 0x0F) << 8 ) | yl );
}

/*****************************************************************************/
