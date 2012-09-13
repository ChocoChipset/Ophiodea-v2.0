#define kMOTOR_DIRECTION_COMMAND 0
#define kMOTOR_SPEED_COMMAND     1    

#define kMOTOR_DIRECTION_CLOCKWISE      0
#define kMOTOR_DIRECTION_ANTI_CLOCKWISE 1    

#define motorEnabledPin 9 // Shoulb be one of the PWM signals
#define directionControlPinA1 10
#define directionControlPinA2 11


//////////////////////////////////////////////////////////////////
// Global variables for Serial Input Parser
unsigned char incomingByte  = 0;   // for incoming serial data
unsigned char syncState     = 0;
unsigned char command       = 0;
unsigned char value         = 0;

//////////////////////////////////////////////////////////////////
// Serial Input Parser
void parse_serial() {

 // send data only when you receive data:
 if (Serial.available()) {
   // read the incoming byte:
   incomingByte = Serial.read();

   value = (unsigned char)incomingByte;   
   
  analogWrite(motorEnabledPin, value); 
   switch(syncState){
     case 0: // Header
       if(incomingByte == 'X') {
         syncState++;
       }
       break;

     case 1: // Command 
       command = (unsigned char)incomingByte;
       syncState++;
       break;

     case 2: // Speed Value
       value = (unsigned char)incomingByte;
 
       switch (command) {
         case kMOTOR_DIRECTION_COMMAND: 
           setDirection((value == 0 ? kMOTOR_DIRECTION_CLOCKWISE : kMOTOR_DIRECTION_ANTI_CLOCKWISE));
           Serial.print("Direction ");
           Serial.println(value);
           break;
 
         case kMOTOR_SPEED_COMMAND:
           Serial.print("Speed ");
           Serial.println(value);
           analogWrite(motorEnabledPin, value); 
           break;
       }
 
       syncState=0;
     break;

   }
 }
}

// direction selection (1 or 0)
void setDirection(int direction) {

  if (direction == kMOTOR_DIRECTION_CLOCKWISE)
  {
    digitalWrite(directionControlPinA2, LOW);
    digitalWrite(directionControlPinA1, HIGH);    

  }
  else if (direction == kMOTOR_DIRECTION_ANTI_CLOCKWISE)
  {
    digitalWrite(directionControlPinA1, LOW);
    digitalWrite(directionControlPinA2, HIGH);    
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
  
  syncState = 0;
  
  setDirection(kMOTOR_DIRECTION_CLOCKWISE);
  Serial.begin(9600);
  
  Serial.print("Motor Control running...");
}

void loop() 
{ 
  parseSerial();

}
