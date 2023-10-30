#include <Servo.h>
 
// Hardware related
Servo servo1;
const int servoPin = 9;
const int buttonPin = 2;
const int servoRange = 1855; // Installed servo pulse width range: 544us-2400us / 1856 us

// Configuration Variables
const int daysInWeek = 7; // How many days do you consider a week
const int modulesInWeek = 14; // Your goal you wish to achieve in that week eg: read that many books 
const int rangeInHours = 48; // Arbitrary span of time in hours over which visual indication takes place  
const unsigned long interval = 3600; // Sets interval between ticks in sec 
const bool rangeOverflow = true; // How should ticks and module completion be treated when overflowing time range 

// Calculated Variables
// Values to servo are parsed as pulse width in us and must be calculated prior
const int step = map(1, 0, rangeInHours, 0, servoRange); // value in us of one tick
const int moduleValue = step * (24 / (modulesInWeek / daysInWeek)); // Value in us of one module completion
const unsigned long period = interval * 1000; 

// States
int currentAngle = 2400; // Act as both starting value and tracked value
unsigned long currentTime = 0;
unsigned long timeOfLastTick = 0;
unsigned long timeOfLastButtonPress = 0;
int buttonState = 1;
int buttonDelay = 1500; // Minimum time between button activation in ms

void setup() {
  Serial.begin(115200);
  servo1.attach(servoPin);
  pinMode(buttonPin, INPUT_PULLUP);
  servo1.writeMicroseconds(currentAngle);   
}

void loop() {
  currentTime = millis();
  if (currentTime - timeOfLastTick >= period){
    timeOfLastTick = currentTime;
    if (rangeOverflow) currentAngle = servo1.readMicroseconds();
    currentAngle = currentAngle + step;
    servo1.writeMicroseconds(currentAngle);
  }

  buttonState = digitalRead(buttonPin);
  if (buttonState == 0 && currentTime - timeOfLastButtonPress >= buttonDelay){
    timeOfLastButtonPress = currentTime;
    if (rangeOverflow) currentAngle = servo1.readMicroseconds();
    currentAngle = currentAngle - moduleValue;
    servo1.writeMicroseconds(currentAngle);
  }      
}