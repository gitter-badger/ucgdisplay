package com.ibasco.pidisplay.impl.lcd.hitachi.components;

import com.ibasco.pidisplay.core.DisplayContainer;
import com.ibasco.pidisplay.impl.lcd.hitachi.LcdGraphics;

public class LcdDisplayContainer extends DisplayContainer<LcdGraphics> {
    public LcdDisplayContainer() {
        super();
    }

    public LcdDisplayContainer(int width, int height) {
        super(width, height);
    }
}