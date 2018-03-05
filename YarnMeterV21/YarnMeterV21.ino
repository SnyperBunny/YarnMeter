//YarnMeter -- test to see how SparkleShare updates small changes.

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
String Unit="m";

//m=y*(m/y)
float m2y=1.09361;//y=m*(y/m)
float y2m=0.9144;//m=y*(m/y)

int circ=20; //circumference of the disk the yarn goes around. In CENTIMETERS.
float YarnLength;//Ylength=counter*circ[umference]/100. in METERS.
float YLDisplay;



void setup() {
  Serial.begin(9600);//initialize the Baud-rate for the Serial Monitor.

  //set input pins as inputs
//  pinMode(SaveButtonPin, INPUT);
  pinMode(saveButtonPin, INPUT);
  pinMode(ResetButtonPin, INPUT);
  pinMode(UnitButtonPin, INPUT);
  pinMode(hallPin, INPUT);

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
      Serial.println(counter);
    }
    oldhallState = hallState; //set hall state of (t-1)
  }
  else {
    oldhallState = hallState; //set hall state of (t-1)
  }

  YarnLength=counter*circ/100;//num revolutions*circumference(cm)/100
  
  //only update the screen at each 0.5 meters or yards.
  if (counter%4==0){//if number of revolutions is such that it equals 0.5m (if it is within 0.1 of being half a meter)
    UpdateScreen();
    
    
  }
  

//  lastSaveState = SaveState;
  lastSaveBTNState = currentSaveBTNState;
  lastResetBTNState=currentResetBTNState;
  lastUnitBTNState=currentUnitBTNState;
  
  
}//****End of Main****

void UpdateScreen(){
    
    if (Unit="m"){
      YLDisplay=counter*circ/100;//yarn length in meters
    }
    else if (Unit="y"){
      YLDisplay=counter*circ/100*m2y;//yarn length in meters *(y/m) conversion
    }
    
    
    String FinalDisplay=YLDisplay+Unit;
    Serial.println(FinalDisplay);

    
}

void ResetSub() {
  counter = 0;
  Serial.println("REEEEESSEEEEEETTT!!!!");
}

void SaveSub() {
  Serial.println("save sequence over heeeeerreee!! yeaahhhh~~");
}

void UnitSub(){
  Serial.println("OH yeah, switching units up in heaaaa!");
  if (Unit=="m"){
    Unit="y";
    //NewL=YarnLength*m2y;
  }
  else if (Unit=="y"){
    Unit="m";
    //NewL=YarnLength*y2m;
  }
  //YarnLength=NewL;
}

