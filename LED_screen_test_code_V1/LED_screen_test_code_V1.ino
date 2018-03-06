#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//#define LOGO16_GLCD_HEIGHT 16 
//#define LOGO16_GLCD_WIDTH  16 


// constants won't change. They're used here to set pin numbers:
const int hallPin = 12;     // the number of the hall effect sensor pin
const int saveButtonPin = 11; //GREEN
const int ResetButtonPin = 10; //YELLOW
const int UnitButtonPin = 9; //BLACK

// variables will change:
int hallState = 0;          // variable for reading the hall sensor status
int oldhallState = 0;       //to store previous hall-state reading.
int counter = 0;

//save button
int SaveBTNState;             // the current reading from the input pin
int lastSaveBTNState = LOW;   // the previous reading from the input pin

//reset button
int ResetBTNState;
int lastResetBTNState = LOW;

//unit button
int UnitBTNState;
int lastUnitBTNState = LOW;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

//unit conversion:
String Unit="m";//initialize units as meters.

//m=y*(m/y)
float m2y=1.09361;//y=m*(y/m)
float y2m=0.9144;//m=y*(m/y)

int circ=20; //circumference of the disk the yarn goes around. In CENTIMETERS.
float YarnLength;//Ylength=counter*circ[umference]/100. in METERS.
float YLDisplay;

boolean ScreenUpdateFlag=1;//start with an inital update.

int x=0;

void setup()   {                
  Serial.begin(9600);//initialize the Baud-rate for the Serial Monitor.

  //set input pins as inputs
  //pinMode(SaveButtonPin, INPUT);
  pinMode(saveButtonPin, INPUT);
  pinMode(ResetButtonPin, INPUT);
  pinMode(UnitButtonPin, INPUT);
  pinMode(hallPin, INPUT);

//***setup for the screen***
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done
  
   // Clear the buffer.
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.println("Bootup finished");
  display.display();
  delay(2000);
  display.clearDisplay();
  //display.display();

  display.println("Look! I WORK!!! yaaaaayyy....");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  
  //**for reference**
  //textsize 2 gives ~10 chars across
  //textsize 3 gives 7 chars across **this is a good size for numbers <10k or >10k with no decimals.
  //textsize 4 gives 5 chars across
  /*
  float test=2530.3;

  if (test>=10000){
    display.setTextSize(2);
  }
  else{
  display.setTextSize(3);
  }
  
  display.setTextColor(WHITE);
  display.print(test,1);display.println("m"); 
  //display.println(0xDEADBEEF, HEX);
  
  display.display();
  delay(2000);
  display.clearDisplay();
*/
}


void loop() {
  display.clearDisplay();
  display.print("x= ");
  display.println(x);
  display.display();
  delay(1000);

  display.setCursor(0,0);
  x++;
}
