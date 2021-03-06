/*-
 * ========================START=================================
 * Organization: Universal Character/Graphics display library
 * Project: UCGDisplay :: Character LCD Driver
 * Filename: LcdRegisterSelectState.java
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
package com.ibasco.ucgdisplay.drivers.clcd.enums;

import com.pi4j.io.gpio.PinState;

public enum LcdRegisterSelectState {
    /**
     * LCD Instruction for Data Processing. Equivalent to {@link PinState#HIGH}
     */
    DATA(PinState.HIGH),
    /**
     * LCD Instruction for Command Processing. Equivalent to {@link PinState#LOW}
     */
    COMMAND(PinState.LOW);

    private PinState state;

    LcdRegisterSelectState(PinState state) {
        this.state = state;
    }

    public PinState getPinState() {
        return state;
    }

    @Override
    public String toString() {
        return "LcdRegisterSelectState{" +
                "state=" + state +
                '}';
    }
}
