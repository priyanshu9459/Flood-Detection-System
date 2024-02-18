/*Flood detection and management */
/*
 * components used are as follows:
 * -Arduino Uno
 * -GSM sensor 800a
 * -GPS sensor neo6
 * -Buzzer
 * -Led
 * -servor motor 
 * -float sensor
 * -LM 35 temp sensor
 * -moisture sensor
 */

#include <SoftwareSerial.h>
SoftwareSerial mySerial(9, 10);
#include <Servo.h>
Servo myservo;
int FloatSensor = 2;
int Buzzer= 13;
int buttonState = 0;
const int out = 12;
const int in = 5;
int pos = 0;
int a, null, val;
int tempPin = 1;
int sensorPin = A0;
int sensorValue;
int Rainfall = 0;
double lowlevel, highlevel;

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
  myservo.attach(8);
  pinMode(FloatSensor, INPUT_PULLUP);
  pinMode (Buzzer, OUTPUT);
  pinMode(in, INPUT);
  pinMode(out, OUTPUT);

  pinMode(13, OUTPUT);
}
void loop()
{
  long dur;
  long dis;
  long tocm;

  buttonState = digitalRead(FloatSensor);

  sensorValue = analogRead(sensorPin);
  Serial.print("Moisture value: ");
  Serial.println(sensorValue);
 
  val = analogRead(tempPin);
  float mv = ( val / 1024.0) * 5000;
  float cel = mv / 10;
  Serial.print("TEMPRATURE = ");
  Serial.print(val);
   Serial.print("c");
  int P=330;
  int M=50;
  int T=val;
int  Evp =(P/((6*8*M*T)));
 Serial.println(" ");
  Serial.print("Evapuration :");
  Serial.print(10);
  Serial.print("micro meter");
  Serial.println();
  delay(1000);



  if (buttonState == LOW)
  {
    digitalWrite(Buzzer, LOW);
    Serial.println( "SAFE WATER LEVEL");
  }
  else
  {
    digitalWrite(Buzzer,HIGH);
    Serial.println( "ALERT HIGH WATER LEVEL" );

   delay(5000);
    SendMessage();




    
  }
  delay(1000);


  digitalWrite(out, LOW);
  delayMicroseconds(2);
  digitalWrite(out, HIGH);
  delayMicroseconds(10);
  digitalWrite(out, LOW);
  dur = pulseIn(in, HIGH);
  tocm = microsecondsToCentimeters(dur);
  Serial.print("Water level:  ");
  Serial.println(String(tocm));
  Serial.println("  ");

  


  if (Serial.available() > 0) {
    Rainfall = Serial.read();
    lowlevel = 17 + Rainfall;
    highlevel  = 15 + Rainfall;
    if (tocm >= lowlevel)
    {
      for (pos = 0; pos <= 50; pos += 1)
        myservo.write(pos);
      delay(1000);
    }
    if (tocm <= highlevel)
    {
      for (pos = 50; pos >= 0; pos -= 1)
        myservo.write(pos);
      delay(1000);
    }

  }

  else {
    if (tocm >= 17)
    {
      for (pos = 0; pos <= 50; pos += 1)
        myservo.write(pos);
      delay(1000);
    }
    if (tocm <= 15)
    {
      for (pos = 50; pos >= 0; pos -= 1)
        myservo.write(pos);
      delay(1000);
    }
    delay(100);

  }

}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;



}

void SendMessage()
{
  Serial.println("sending message.....");
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CMGS=\"+917018493476\"\r");
  delay(1000);



  mySerial.println("**** Alert River Water Level is HIGH. Evacuate as Soon as possible ****");

  delay(100);
  mySerial.println((char)26);
  delay(1000);

}