/*-
 * ========================START=================================
 * Organization: Universal Character/Graphics display library
 * Project: UCGDisplay :: Graphics LCD Driver
 * Filename: GlcdSize.java
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
/*
 * THIS IS AN AUTO-GENERATED CODE!! PLEASE DO NOT MODIFY (Last updated: Oct 12 2018 13:44:00)
 */

package com.ibasco.ucgdisplay.drivers.glcd.enums;

import java.util.Arrays;

@SuppressWarnings("unused")
public enum GlcdSize {
    SIZE_104x64(13, 8),
    SIZE_128x128(16, 16),
    SIZE_128x32(16, 4),
    SIZE_128x64(16, 8),
    SIZE_128x96(16, 12),
    SIZE_136x32(17, 4),
    SIZE_136x64(17, 8),
    SIZE_160x104(20, 13),
    SIZE_160x128(20, 16),
    SIZE_160x160(20, 20),
    SIZE_160x80(20, 10),
    SIZE_176x104(22, 13),
    SIZE_176x72(22, 9),
    SIZE_192x32(24, 4),
    SIZE_192x64(24, 8),
    SIZE_200x200(25, 25),
    SIZE_240x120(30, 15),
    SIZE_240x128(30, 16),
    SIZE_240x160(30, 20),
    SIZE_240x64(30, 8),
    SIZE_256x128(32, 16),
    SIZE_256x160(32, 20),
    SIZE_256x32(32, 4),
    SIZE_256x64(32, 8),
    SIZE_296x128(37, 16),
    SIZE_320x240(40, 30),
    SIZE_32x8(4, 1),
    SIZE_384x136(48, 17),
    SIZE_384x240(48, 30),
    SIZE_400x240(50, 30),
    SIZE_48x64(6, 8),
    SIZE_64x128(8, 16),
    SIZE_64x32(8, 4),
    SIZE_64x48(8, 6),
    SIZE_72x40(9, 5),
    SIZE_88x48(11, 6),
    SIZE_8x8(1, 1),
    SIZE_96x16(12, 2),
    SIZE_96x72(12, 9),
    SIZE_96x96(12, 12);

    private int tileWidth;
    private int tileHeight;

    GlcdSize(int tileWidth, int tileHeight) {
        this.tileWidth = tileWidth;
        this.tileHeight = tileHeight;
    }

    public int getDisplayWidth() {
        return tileWidth * 8;
    }

    public int getDisplayHeight() {
        return tileHeight * 8;
    }

    public int getTileWidth() {
        return tileWidth;
    }

    public int getTileHeight() {
        return tileHeight;
    }

    public static GlcdSize get(int tileWidth, int tileHeight) {
        return Arrays.stream(GlcdSize.values())
                .filter(p -> (p.getTileWidth() == tileWidth) && (p.getTileHeight() == tileHeight))
                .findFirst().orElse(null);
    }
}
