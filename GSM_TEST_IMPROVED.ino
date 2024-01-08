#include <SoftwareSerial.h>
SoftwareSerial sim800l(9, 10); 

#define START_BUTTON 7 
#define EMERGENCY_BUTTON 8  

float thresholdTime  = 0.1666667;
float thresholdTime1 = 0.25 ; // 

unsigned long startTime; 
bool START_BUTTON_STATE;  
bool EMERGENCY_BUTTON_STATE; 

bool SYSTEM_HALT_STATE = false; 

void setup(){
    pinMode(START_BUTTON, INPUT_PULLUP); 
    pinMode(EMERGENCY_BUTTON, INPUT_PULLUP);
    sim800l.begin(9600);  
    Serial.begin(9600);   
    delay(1000);
    Serial.println("Press Start Button to start the system..");
}

void loop(){
    START_BUTTON_STATE = digitalRead(START_BUTTON);   
    EMERGENCY_BUTTON_STATE = digitalRead(EMERGENCY_BUTTON);

    //Check if the start button is pressed and the system is halt free
    
    if(START_BUTTON_STATE == LOW && !SYSTEM_HALT_STATE){
      Serial.println();
      Serial.println("Start button is pressed...!");   
      delay(200); 
      LINE_IS_WORKING(); 
    }

  //In case emergency button is pressed while system was running. Here !SYSTEM_HALT_STATE refers the system was initially in no emergency state

     else if(SYSTEM_HALT_STATE && millis() - startTime >= thresholdTime*60*1000){
        Serial.println();
        Serial.println("15 minutes line failure ..!");
        delay(200);
        LINE_IS_STOPPED();  
        startTime = millis(); 
        Serial.println();
        Serial.println("System is in halt state..!");
        Serial.println("Press Start Button if it's fixed or fixing...!");   
     }
     
     else if(START_BUTTON_STATE == LOW && SYSTEM_HALT_STATE){
         Serial.println();
         Serial.println("Recovery is Done...!");   
         delay(200); 
         SYSTEM_HALT_STATE = false;
      }
   
     else if(SYSTEM_HALT_STATE && millis() - startTime >= thresholdTime1*60*1000){
        Serial.println();
        Serial.println("30 minutes line failure ..!");
        delay(200);
        LONG_TIME_LINE_FAILURE();
        startTime = millis(); 
        Serial.println();
        Serial.println("System is in halt state..!");
        Serial.println("Press Start Button if it's fixed or fixing...!");   
     }
     
      else if(START_BUTTON_STATE == LOW && SYSTEM_HALT_STATE){
         Serial.println();
         Serial.println("Recovery is Done...!");   
         delay(200); 
         SYSTEM_HALT_STATE = false;
      }
        
}

void LINE_IS_WORKING()
{
  Serial.println("Line is working ! Sending SMS to Supervisor...");              
  sim800l.print("AT+CMGF=1\r");                   
  delay(100);
  SUPERVISOR_NUMBER(); //supervisors Tel numbers
  delay(500);
  sim800l.print("Line is working");       
  delay(500);
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("SMS Sent.");
  delay(500);

}

void LINE_IS_STOPPED()
{
  Serial.println("Line has stopped! Sending SMS To Line Leader...");                 
  sim800l.print("AT+CMGF=1\r");                   
  delay(100);
  LINELEADER_NUMBER(); 
  delay(500);
  sim800l.print("Line is stopped from 15 minutes");    
  delay(500);   
  sim800l.print((char)26);// (required according to the datasheet)
  delay(500);
  sim800l.println();
  Serial.println("SMS Sent.");
  delay(500);
}
void  LONG_TIME_LINE_FAILURE ()
{
  Serial.println("Long time line failure! Sending SMS to SHIFTLEADER...");           
  sim800l.print("AT+CMGF=1\r");                 
  delay(100);
  SHIFTLEADER_NUMBER();
  delay(500);
  sim800l.print("Line is stopped from 30 minutes");       
  delay(500);
  sim800l.print((char)26);
  delay(500);
  sim800l.println();
  Serial.println("SMS Sent.");
  delay(500);

}
void SUPERVISOR_NUMBER() // Supervisors tell numbers
{
sim800l.print("AT+CMGS=\"+21698763315\"\r");  
sim800l.print("AT+CMGS=\"+21698763323\"\r");  
}
void LINELEADER_NUMBER()// Line Leaders tell numbers
{
sim800l.print("AT+CMGS=\"+919080659530\"\r");  
sim800l.print("AT+CMGS=\"+919080659531\"\r");  
}
void SHIFTLEADER_NUMBER()// Shift Leaders tell numbers
{
sim800l.print("AT+CMGS=\"+919080659530\"\r");  
sim800l.print("AT+CMGS=\"+919080659531\"\r");    
}
