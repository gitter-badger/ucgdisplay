package com.ibasco.pidisplay.impl.lcd.hitachi.events;

import com.ibasco.pidisplay.core.DisplayNode;
import com.ibasco.pidisplay.core.events.Event;
import com.ibasco.pidisplay.core.events.EventType;
import com.ibasco.pidisplay.core.util.Node;
import com.ibasco.pidisplay.impl.lcd.hitachi.LcdGraphics;

import java.util.Objects;

public class LcdMenuItemEvent extends LcdEvent {

    public static final EventType<LcdMenuItemEvent> LCD_ITEM_SELECTED = new EventType<>("LCD_ITEM_SELECTED");

    public static final EventType<LcdMenuItemEvent> LCD_ITEM_FOCUS = new EventType<>("LCD_ITEM_FOCUS");

    private Node<String> selectedItem;

    private int selectedIndex;

    public LcdMenuItemEvent(EventType<? extends Event> eventType, Node<String> selectedItem, int selectedIndex, DisplayNode<LcdGraphics> display) {
        super(eventType, display);
        this.selectedItem = selectedItem;
        this.selectedIndex = selectedIndex;
    }

    public int getSelectedIndex() {
        return selectedIndex;
    }

    public Node<String> getSelectedItem() {
        return selectedItem;
    }

    @Override
    public String toString() {
        return Objects.toString(selectedItem, "N/A");
    }
}
