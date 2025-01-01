/**
 * @file    i2c.cpp
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2025-01-01
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESP32 I2C controller.
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
#include "i2c.h"

// ESP-IDF Framework
#include "esp_err.h"

/*****************************************************************************/

/* Public Functions */

void i2c_setup(const i2c_port_t i2c_port, const uint8_t io_sda,
    const uint8_t io_scl, const uint32_t frequency)
{
    i2c_config_t i2c_config;
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = static_cast<int>(io_sda);
    i2c_config.scl_io_num = static_cast<int>(io_scl);
    i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.master.clk_speed = frequency;
    i2c_config.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    ESP_ERROR_CHECK(i2c_param_config(i2c_port, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(i2c_port, I2C_MODE_MASTER, 0U, 0U, 0));
}

bool i2c_write(const i2c_port_t i2c_port, const uint16_t slave_address,
    const uint8_t data_to_write)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, data_to_write, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);

    return (ret == ESP_OK);
}

bool i2c_read_register(const i2c_port_t i2c_port, const uint16_t slave_address,
    const uint8_t reg_address, uint8_t* data_read)
{
    *data_read = 0;

    // Write Command Request
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (slave_address << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_address, true);

    // Keep Communication to get response
    i2c_master_start(cmd);

    // Read Response
    i2c_master_write_byte(cmd, (slave_address << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, data_read, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);

    return (ret == ESP_OK);
}

/*****************************************************************************/
