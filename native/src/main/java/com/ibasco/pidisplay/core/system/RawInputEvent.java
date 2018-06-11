package com.ibasco.pidisplay.core.system;

/**
 * Represents a raw input device event
 *
 * @author Rafael Ibasco
 */
public class RawInputEvent {
    private InputDevice device;
    private long eventTime;
    private int type;
    private int code;
    private int value;
    private String codeName;
    private String typeName;
    private int repeatCount;

    public RawInputEvent(InputDevice device, long eventTime, int type, int code, int value, String codeName, String typeName, int repeatCount) {
        this.device = device;
        this.eventTime = eventTime;
        this.type = type;
        this.code = code;
        this.value = value;
        this.codeName = codeName;
        this.typeName = typeName;
        this.repeatCount = repeatCount;
    }

    public InputDevice getDevice() {
        return device;
    }

    public long getEventTime() {
        return eventTime;
    }

    public int getType() {
        return type;
    }

    public int getCode() {
        return code;
    }

    public int getValue() {
        return value;
    }

    public String getCodeName() {
        return codeName;
    }

    public String getTypeName() {
        return typeName;
    }

    public int getRepeatCount() {
        return repeatCount;
    }

    @Override
    public String toString() {
        final StringBuffer sb = new StringBuffer();
        sb.append("device=").append((device != null) ? device.getName() : "N/A");
        sb.append(", eventTime=").append(eventTime);
        sb.append(", type=").append(type);
        sb.append(", code=").append(code);
        sb.append(", value=").append(value);
        sb.append(", codeName=").append(codeName);
        sb.append(", typeName=").append(typeName);
        sb.append(", count=").append(repeatCount);
        return sb.toString();
    }
}
