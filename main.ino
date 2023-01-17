// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int in=0, out=0, count=0, current = 0, reading = 0;
const int pir1=7,pir2=6;
const int button = 10;
const int buzzer = 13;
const int led = 8;
bool buttonInput = true;
int maxCurrent = 5;
bool printStatus = true;
double celsius;
Servo door;

void setup() {
  // Set up LCD Screen
  lcd.begin(16, 2);
  lcd.print("Visitor Counter");
  lcd.setCursor(4,1);
  lcd.print("system");
  delay(2000);
  pinMode(pir1, INPUT); //motion sensor 1
  pinMode(pir2, INPUT); //motion sensor 2
  pinMode(button, INPUT); //toggle button
  pinMode(buzzer, OUTPUT); //buzzer
  pinMode(led, OUTPUT); //red led
  pinMode(A0, INPUT); //temperature sensor
  Serial.begin(9600);
  door.attach(9);
  door.write(0);
}

void loop() {
  begin:
  if (digitalRead(pir1)) {
   count++; 
   in = in+1; 
   lcd.clear();
   // Sounds the buzzer at the frequency relative to the note F in Hz
   tone(buzzer,349);    
   delay(250);    
   noTone(buzzer); 
   // Sounds the buzzer at the frequency relative to the note G in Hz
   tone(buzzer, 1000);            
   noTone(buzzer); 
   delay(250);
 }
 
  if (digitalRead(pir2)){
   count--; 
   out = out+1; 
   delay(500);
   lcd.clear();
 }

  reading = digitalRead(button);

  //toggle switch 
  if (reading) {
    
    //print to the screen
  	lcd.clear();
  	lcd.setCursor(0,0);
    lcd.print("People Visited:");

    lcd.setCursor(0,1); 
    lcd.print("IN:");

    lcd.setCursor(3,1);
    lcd.print(in); 

    lcd.setCursor(6,1); 
    lcd.print("OUT:");

    lcd.setCursor(10,1);
    lcd.print(out);

    delay(500);
    lcd.setCursor(0,1); 
    lcd.print("IN:");
    
  }
  //toggle switch
  else {
    delay(500); 
    lcd.setCursor(0,0);
    lcd.print("Current Visitor:");
    lcd.setCursor(0,1); 

    lcd.print("Currrent: ");
    lcd.print(current);
    delay(500);
  }
  //measure the temperature in celsius
  celsius = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125);
  current = in - out;
  
  //if visitor > max or temperature > 37.8
  if (current >= maxCurrent || celsius >= 37.8) {
    	digitalWrite(led, HIGH); //turn on red led
        door.write(90); //close the door
    	//print status inside console
   		if (!printStatus) {
        	Serial.println("STATUS");
          	Serial.println("Door Closed");
          	Serial.println("lights on");
            Serial.println("");
    		printStatus = true;
        }
    	goto begin;
    	
    	
  }
  else if (current == maxCurrent-1 || celsius < 37.8){
      	digitalWrite(led, LOW); //turn off red led
    	door.write(0); //open the door
    	//print the status inside console
    	if (printStatus) {
            Serial.println("STATUS");
          	Serial.println("Door Opened");
          	Serial.println("lights off");
            Serial.println("");
    		printStatus = false;
        }
  }
}