package com.ibasco.pidisplay.impl.lcd.hitachi;

import com.google.common.base.Charsets;
import com.ibasco.pidisplay.core.AbstractDisplay;
import com.ibasco.pidisplay.core.events.EventDispatcher;
import org.apache.commons.lang3.StringUtils;

import java.nio.ByteBuffer;

@SuppressWarnings("WeakerAccess")
abstract public class LcdDisplay extends AbstractDisplay<LcdGraphics> {

    protected static final String CMD_CHAR = "char";

    protected LcdDisplay() {
        registerVar(CMD_CHAR, this::processCustomChar);
    }

    private String processCustomChar(String args) {
        if (StringUtils.isNumeric(args)) {
            byte charCode = Byte.parseByte(args);
            return Charsets.UTF_8.decode(ByteBuffer.wrap(new byte[]{charCode})).toString();
        }
        return StringUtils.EMPTY;
    }

    @Override
    public void draw(LcdGraphics graphics) {
        if (!EventDispatcher.THREAD_NAME.equals(Thread.currentThread().getName()))
            throw new RuntimeException("This method should only be called from the event dispatcher thread");
        if (getWidth() == -1)
            setWidth(graphics.getWidth());
        if (getHeight() == -1)
            setHeight(graphics.getHeight());
    }
}