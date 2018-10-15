package com.ibasco.pidisplay.drivers.lcd.hd44780.exceptions;

public class DisplayDriverException extends RuntimeException {
    public DisplayDriverException() {
    }

    public DisplayDriverException(String message) {
        super(message);
    }

    public DisplayDriverException(String message, Throwable cause) {
        super(message, cause);
    }

    public DisplayDriverException(Throwable cause) {
        super(cause);
    }

    public DisplayDriverException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}