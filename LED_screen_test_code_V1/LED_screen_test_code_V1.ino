//#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//#define LOGO16_GLCD_HEIGHT 16 
//#define LOGO16_GLCD_WIDTH  16 

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  /*
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  */
  //textsize 3 gives
  //textsize 4 gives 5 chars across
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.print("0x"); 
  display.println(0xDEADBEEF, HEX);
  
  display.display();
  delay(2000);
  display.clearDisplay();

}


void loop() {
  
}
