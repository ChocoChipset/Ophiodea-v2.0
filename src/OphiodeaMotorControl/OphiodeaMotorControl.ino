#define kMOTOR_DIRECTION_CLOCKWISE 0
#define kMOTOR_DIRECTION_ANTI_CLOCKWISE 1    

#define motorEnabledPin 9 // Shoulb be one of the PWM signals
#define directionControlPinA1 10
#define directionControlPinA2 11




// direction selection (1 or 0)
void setDirection(int direction) {

  if (direction == kMOTOR_DIRECTION_CLOCKWISE)
  {
    digitalWrite(directionControlPinA2, LOW);
    digitalWrite(directionControlPinA1, HIGH);    
    break;
  }
  else if (direction == kMOTOR_DIRECTION_ANTI_CLOCKWISE)
  {
    digitalWrite(directionControlPinA1, LOW);
    digitalWrite(directionControlPinA2, HIGH);    
    break;
  }
  else
  {
    Serial.print("Unknown direction for motor");   
  }
}

void setup() {
  pinMode(directionControlPinA1, OUTPUT);
  pinMode(directionControlPinA2, OUTPUT);
  pinMode(motorEnabledPin, OUTPUT);
  
  Serial.begin(115200);
}

void loop() 
{ 
  setDirection(kMOTOR_DIRECTION_CLOCKWISE);
  
  int analogInputValue = analogRead(0); // (Range: 0 to 1023)
  unsigned char pwmValue = analogInputValue / 4; // PWM Value range sholg be 0 to 255.
  
  analogWrite(motorEnabledPin, pwmValue); 
}
