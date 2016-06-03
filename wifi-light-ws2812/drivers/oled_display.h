void OLED_clear() {
    oled_display.clearDisplay();
    oled_display.setTextSize(1);
    oled_display.setTextColor(WHITE);
    oled_display.setCursor(0,0);
    oled_display.display();
}

void initializeOLED_display() {
    oled_display.begin(SSD1306_SWITCHCAPVCC, OLED_address,false);
    OLED_clear();
    oled_display.println("Display initialized");
    oled_display.display();
}

