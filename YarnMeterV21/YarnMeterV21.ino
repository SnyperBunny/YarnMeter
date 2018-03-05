//#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


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

void setup() {
  Serial.begin(9600);//initialize the Baud-rate for the Serial Monitor.

  //set input pins as inputs
//  pinMode(SaveButtonPin, INPUT);
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

//**for reference**
  //textsize 2 gives ~10 chars across
  //textsize 3 gives 7 chars across **this is a good size for numbers <10k or >10k with no decimals.
  //textsize 4 gives 5 chars across
}


void loop() {
  //check the save button.
  int currentSaveBTNState = digitalRead(saveButtonPin);
  int currentResetBTNState=digitalRead(ResetButtonPin);
  int currentUnitBTNState=digitalRead(UnitButtonPin);


  // If the switch changed, due to noise or pressing:
  if (currentSaveBTNState != lastSaveBTNState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if (currentResetBTNState != lastResetBTNState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
  if (currentUnitBTNState!=lastUnitBTNState){
    //reset debounce timer
    lastDebounceTime=millis();
  }
  
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (currentSaveBTNState != SaveBTNState) {
      SaveBTNState = currentSaveBTNState;
      if (SaveBTNState == HIGH) {
        Serial.println("Save!");
        SaveSub();
      }
    }
    if (currentResetBTNState != ResetBTNState) {
      ResetBTNState = currentResetBTNState;
      if (ResetBTNState == HIGH) {
        Serial.println("Reset!");
        ResetSub();
      }
    }
    if (currentUnitBTNState != UnitBTNState) {
      UnitBTNState = currentUnitBTNState;
      if (UnitBTNState == HIGH) {
        Serial.println("Switch Units!!");
        UnitSub();
      }
    }
  }

  // read the state of the hall effect sensor:
  hallState = digitalRead(hallPin);

  if (hallState == LOW) {
    if (hallState != oldhallState) {
      counter++;
      //Serial.println(counter);
      ScreenUpdateFlag=1;
    }
    oldhallState = hallState; //set hall state of (t-1)
  }
  else {
    oldhallState = hallState; //set hall state of (t-1)
  }

  YarnLength=float(counter)*float(circ)/100.0;//num revolutions*circumference(cm)/100
  
  //only update the screen once each revolution.
  if (ScreenUpdateFlag==1){
    UpdateScreen();
    ScreenUpdateFlag=0;
  }
  

//  lastSaveState = SaveState;
  lastSaveBTNState = currentSaveBTNState;
  lastResetBTNState=currentResetBTNState;
  lastUnitBTNState=currentUnitBTNState;
  
  
}//****End of Main****

void UpdateScreen(){
    if (Unit=="m"){
      YLDisplay=YarnLength;//counter*circ/100;//yarn length in meters
    }
    else if (Unit=="y"){
      YLDisplay=YarnLength*m2y;//counter*circ/100*m2y;//yarn length in meters *(y/m) conversion
    }
    
    //String FinalDisplay=YLDisplay+Unit;
    //Serial.println(FinalDisplay);
    display.clearDisplay();
    
    if (YLDisplay>=10000.0){
      display.setTextSize(2);
    }
    else{
      display.setTextSize(3);
    }
    
    display.println();
    display.print(YLDisplay,1);
    display.println(Unit);
    
}

void ResetSub() {
  counter = 0;
  display.clearDisplay();
  display.setTextSize(3);
  //display.setTextColor(WHITE);
  display.println("Reset!");
  delay(2000);
  display.clearDisplay();
  display.println("0.0m");
}

void SaveSub() {
  Serial.println("save sequence over heeeeerreee!! yeaahhhh~~");
}

void UnitSub(){
  if (Unit=="m"){
    Unit="y";
  }
  else {
    Unit="m";
  }
}

