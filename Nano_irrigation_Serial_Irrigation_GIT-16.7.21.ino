#include <avr/io.h> //serial lib
#include <avr/wdt.h> 
#include <ctype.h> //data type lib

byte rx = 2;//MAX232 Serial comm RX on pin rx
//TX on native tx (pin 0 on Arduino Nano)
#define Relay1 8 //RELAY on pin 8

void setup() 
{
  Serial.begin(9600);
  pinMode(Relay1, OUTPUT); 
  digitalWrite(Relay1,HIGH);
}
void loop() 
{
  //PARAMETERS
  float targetWeight;
  float targetWeight1 = 2946; //target weight for treatment 1
  float targetWeight2 = 2627; //target weight for treatment 2
  float minimumWeight = 350; //minimum weight to initiate irrigation cycle
  bool flag;
  
  //INITIALIZATION
  String serialString = "";
  float serialFloat;

  while((Serial.available())) 
  {
    //Serial.println("Irrigation loop active"); //uncomment previous line to debug loop
  if (digitalRead(12) == HIGH) {flag = 1; targetWeight=targetWeight1;}
  if (digitalRead(13) == HIGH) { flag = 1;targetWeight=targetWeight2;}
  
  
    char incomingChar = Serial.read();//SERIAL READ & PARSE FOR AN UNINTERUPTED STRING OF 8 DIGITS

    if(isDigit(incomingChar) || incomingChar == '.')
    {
      serialString += incomingChar;
    } 
    else 
    {
      if(serialString != "") 
      {
        
        float serialFloat = serialString.toFloat();
        /*Serial.println(serialString);*/ Serial.print("VAL: ");Serial.println(serialFloat);
      
      //IRRIGATION LOGIC
        if ((flag == 1) && ((minimumWeight<serialFloat) && (serialFloat<targetWeight)))
       
        {
          digitalWrite(Relay1,LOW);//Serial.println("LOW");
        }
        if ((flag == 1) && ((minimumWeight>serialFloat) || (serialFloat>=targetWeight)))
        {
          digitalWrite(Relay1,HIGH);flag = 0;
          Serial.println("FLAG RESET");//Serial.println("HIGH");
         
        }
      }
      
      //RESET serialString
      serialString = "";
      serialFloat;
    }
  delay(10);
  }
 
  
  //This line of code can be used in debugging - ur friend rige
  //Serial.println("DEBUG " + serialString + " " + serialString.length() + " " + incomingChar + " " + (isDigit(incomingChar)) + " " + (incomingChar == '.'));
  
  }

