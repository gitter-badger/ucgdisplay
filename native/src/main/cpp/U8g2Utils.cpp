/*-
 * ========================START=================================
 * Organization: Universal Character/Graphics display library
 * Project: UCGDisplay :: Native Library
 * Filename: U8g2Utils.cpp
 * 
 * ---------------------------------------------------------
 * %%
 * Copyright (C) 2018 Universal Character/Graphics display library
 * %%
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Lesser Public License for more details.
 * 
 * You should have received a copy of the GNU General Lesser Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/lgpl-3.0.html>.
 * =========================END==================================
 */

#include <sstream>
#include <iostream>
#include <memory>
#include <functional>
#include <iomanip>

#include "Global.h"
#include "U8g2Utils.h"
#include "U8g2Hal.h"

static map<uintptr_t, shared_ptr<u8g2_info_t>> u8g2_device_cache; // NOLINT
static map<int, string> pinNameIndexMap; //NOLINT

#define COMINT_4WSPI 0x0001
#define COMINT_3WSPI 0x0002
#define COMINT_6800  0x0004
#define COMINT_8080 0x0008
#define COMINT_I2C  0x0010
#define COMINT_ST7920SPI 0x0020     /* mostly identical to COM_4WSPI, but does not use DC */
#define COMINT_UART 0x0040
#define COMINT_KS0108  0x0080        /* mostly identical to 6800 mode, but has more chip select lines */
#define COMINT_SED1520  0x0100

#define COMTYPE_HW 0
#define COMTYPE_SW 1

static bool _pins_initialized = false;

u8g2_cb_t *u8g2util_ToRotation(int rotation) {
    switch (rotation) {
        case 0: {
            return const_cast<u8g2_cb_t *>(U8G2_R0);
        }
        case 1: {
            return const_cast<u8g2_cb_t *>(U8G2_R1);
        }
        case 2: {
            return const_cast<u8g2_cb_t *>(U8G2_R2);
        }
        case 3: {
            return const_cast<u8g2_cb_t *>(U8G2_R3);
        }
        case 4: {
            return const_cast<u8g2_cb_t *>(U8G2_MIRROR);
        }
        default:
            break;
    }
    return nullptr;
}

u8g2_msg_func_info_t u8g2util_GetByteCb(int commInt, int commType) {
    switch (commInt) {
        case COMINT_3WSPI: {
            if (commType == COMTYPE_HW) {
                //no HW support for 3-wire interface?
                return nullptr;
            }
            return cb_byte_3wire_sw_spi;
        }
        case COMINT_4WSPI: {
            if (commType == COMTYPE_HW) {
                return cb_byte_spi_hw;
            }
            return cb_byte_4wire_sw_spi;
        }
            //Similar to 4W_SPI
        case COMINT_ST7920SPI: {
            if (commType == COMTYPE_HW) {
                return cb_byte_spi_hw;
            }
            return cb_byte_4wire_sw_spi;
        }
        case COMINT_I2C: {
            if (commType == COMTYPE_HW) {
                return cb_byte_i2c_hw;
            }
            return cb_byte_sw_i2c;
        }
        case COMINT_6800: {
            if (commType == COMTYPE_HW) {
                //TODO: Maybe in future, we could allow I2C/SPI/Shift register expansions for 6800 and 8080 modes
                return nullptr;
            }
            return cb_byte_8bit_6800mode;
        }
        case COMINT_8080: {
            if (commType == COMTYPE_HW) {
                return nullptr;
            }
            return cb_byte_8bit_8080mode;
        }
        case COMINT_UART: {
            //TODO: Not yet sure how to implement this
            //SEE: u8g2_Setup_a2printer_384x240_f()
            return nullptr;
        }
            //similar to 6800 mode
        case COMINT_KS0108: {
            if (commType == COMTYPE_HW) {
                return nullptr;
            }
            return cb_byte_ks0108;
        }
        case COMINT_SED1520: {
            if (commType == COMTYPE_HW) {
                return nullptr;
            }
            return cb_byte_sed1520;
        }
        default:
            break;
    }
    return nullptr;
}

std::string hexs(unsigned char *data, int len) {
    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < len; ++i)
        ss << std::setw(2) << std::setfill('0') << (int) data[i];
    return ss.str();
}

shared_ptr<u8g2_info_t>
u8g2util_SetupAndInitDisplay(string setup_proc_name, int commInt, int commType, int address, const u8g2_cb_t *rotation,
                             u8g2_pin_map_t pin_config, bool virtualMode) {
    shared_ptr<u8g2_info_t> info = make_shared<u8g2_info_t>();

    //Initialize device info details
    info->u8g2 = make_shared<u8g2_t>();
    info->pin_map = pin_config;
    info->rotation = const_cast<u8g2_cb_t *>(rotation);
    info->flag_emulation = virtualMode;

    //Get the setup procedure callback
    u8g2_setup_func_t setup_proc_callback = u8g2hal_GetSetupProc(setup_proc_name);

    //Verify that we have found a callback
    if (setup_proc_callback == nullptr) {
        cerr << "u8g2 setup procedure not found: '" << setup_proc_name << "'" << endl;
        return nullptr;
    }

    info->setup_proc_name = setup_proc_name;
    info->setup_cb = setup_proc_callback;

    //Retrieve the byte callback function based on the commInt and commType arguments
    u8g2_msg_func_info_t cb_byte = u8g2util_GetByteCb(commInt, commType);
    if (cb_byte == nullptr) {
        JNIEnv *env;
        GETENV(env);
        JNI_ThrowNativeLibraryException(env, string("No available byte callback procedures for CommInt = ") +
                                             to_string(commInt) + string(", CommType = ") + to_string(commType));
        return nullptr;
    }

    //Byte callback
    info->byte_cb = [cb_byte, info, virtualMode, commInt, commType](u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) -> uint8_t {
        if (virtualMode) {
            JNIEnv *env;
            GETENV(env);

            //Only process byte send events
            if (msg == U8X8_MSG_BYTE_SEND) {
                uint8_t value;
                uint8_t size = arg_int;
                auto *data = (uint8_t *)arg_ptr;

                JNI_FireByteEvent(env, info->address(), U8G2_BYTE_SEND_INIT, size);

                while( size > 0 ) {
                    value = *data;
                    data++;
                    size--;
                    //cout << ">> Data: " << hexs(&value, 1) << endl;
                    JNI_FireByteEvent(env, info->address(), msg, value);
                }
            } else {
                JNI_FireByteEvent(env, info->address(), msg, arg_int);
            }
            return 1;
        }
        return cb_byte(info.get(), u8x8, msg, arg_int, arg_ptr);
    };

    //Gpio callback
    info->gpio_cb = [virtualMode, info](u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) -> uint8_t {
        if (virtualMode) {
            JNIEnv *env;
            GETENV(env);
            JNI_FireGpioEvent(env, info->address(), msg, arg_int);
            return 1;
        }
        return cb_gpio_delay(info.get(), u8x8, msg, arg_int, arg_ptr);
    };

    //Obtain the raw pointer
    u8g2_t *pU8g2 = info->u8g2.get();

    //Assign the i2c addres if applicable
    if (commType == COMINT_I2C) {
        u8g2_SetI2CAddress(pU8g2, address);
    }

    //Insert device info in cache
    auto it = u8g2_device_cache.insert(make_pair((uintptr_t) pU8g2, info));

    //Call the setup procedure
    info->setup_cb(pU8g2, rotation, u8g2util_SetupHelperByte, u8g2util_SetupHelperGpio);

    //Initialize the display
    u8g2_InitDisplay(pU8g2);
    u8g2_SetPowerSave(pU8g2, 0);
    u8g2_ClearDisplay(pU8g2);

    return info;
}

shared_ptr<u8g2_info_t> u8g2util_GetDisplayDeviceInfo(uintptr_t addr) {
    auto it = u8g2_device_cache.find(addr);
    if (it != u8g2_device_cache.end())
        return it->second;
    return nullptr;
}

uint8_t u8g2util_SetupHelperByte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    auto addr = (uintptr_t) u8x8;
    shared_ptr<u8g2_info_t> info = u8g2util_GetDisplayDeviceInfo(addr);
    if (info == nullptr) {
        cerr << "[u8g2_setup_helper_byte] Unable to obtain display device info for address: " << to_string(addr)
             << endl;
        exit(-1);
    }
    return info->byte_cb(u8x8, msg, arg_int, arg_ptr);
}

uint8_t u8g2util_SetupHelperGpio(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    auto addr = (uintptr_t) u8x8;
    shared_ptr<u8g2_info_t> info = u8g2util_GetDisplayDeviceInfo(addr);
    if (info == nullptr) {
        cerr << "[u8g2_setup_helper_gpio] Unable to obtain display device info for address: " << to_string(addr)
             << endl;
        exit(-1);
    }
    return info->gpio_cb(u8x8, msg, arg_int, arg_ptr);
}

string u8g2util_GetPinIndexDesc(int index) {
    if (!_pins_initialized) {
        if (!pinNameIndexMap.empty())
            pinNameIndexMap.clear();
        pinNameIndexMap[0] = "D0 (alt. SPI CLOCK)";
        pinNameIndexMap[1] = "D1 (alt. SPI DATA)";
        pinNameIndexMap[2] = "D2";
        pinNameIndexMap[3] = "D3";
        pinNameIndexMap[4] = "D4";
        pinNameIndexMap[5] = "D5";
        pinNameIndexMap[6] = "D6";
        pinNameIndexMap[7] = "D7";
        pinNameIndexMap[8] = "E";
        pinNameIndexMap[9] = "CS";
        pinNameIndexMap[10] = "DC";
        pinNameIndexMap[11] = "RESET";
        pinNameIndexMap[12] = "I2C CLOCK (SCL)";
        pinNameIndexMap[13] = "I2C DATA (SDA)";
        pinNameIndexMap[14] = "CS1 (Chip Select 1)";
        pinNameIndexMap[15] = "CS2 (Chip Select 2)";
        //Initialize pin names here
        _pins_initialized = true;
    }
    if (index > 15) {
        return nullptr;
    }
    return pinNameIndexMap.at(index);
}

u8g2_t *toU8g2(jlong address) {
    auto *u8g2 = reinterpret_cast<u8g2_t *>(address);
    return u8g2;
}
