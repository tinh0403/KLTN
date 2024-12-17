#include <SoftwareSerial.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <SPI.h>

#define RX_PIN 11
#define TX_PIN 10
//#define SERIAL_BUFFER_SIZE 256
//card 1 63 51 6D 29
//card 3 94 D1 99 8A
//card 4 E5 ED 9A 8A

SoftwareSerial mySerial(RX_PIN, TX_PIN); 

RTC_DS1307 rtc;

LiquidCrystal_I2C lcd(0x27,20,4);

StaticJsonDocument<300> doc;
//StaticJsonDocument<200> doc1;
int t = 0;
char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

#define buttonD1 37
#define buttonD2 39
#define buttonD3 41
#define buttonD4 47
#define buttonD5 45
#define buttonD6 43

#define relayD1 23
#define relayD2 29
#define relayD3 31
#define relayD4 33
#define relayD5 30
#define relayD6 27
#define relayD7 35

int CBHhongngoai = 42;
int CBanhsang = 36;
int Object1 = LOW;
int CBASstate = 0;

unsigned long ms_from_start = 0;
unsigned long ms_pre_read_ds1307 = 0;
unsigned long interval_ds1307 = 20;
unsigned long ms_pre_read_status = 0;
unsigned long interval_status = 50;
unsigned long ms_pre_read_button = 0;
unsigned long interval_button = 50;

byte toggleState_1 = 1,toggleState_2 = 1,toggleState_3 = 1,toggleState_4 = 1,toggleState_5 = 1,toggleState_6 = 1;

void setup () 
{
  Wire.begin(); 
  Serial.begin(9600);
  //Serial.setRxBufferSize(1024);
  mySerial.begin(9600);
  //mySerial.setRxBufferSize(1024);
  pinMode(relayD1,OUTPUT);
  digitalWrite(relayD1,HIGH);
  pinMode(relayD2,OUTPUT);
  digitalWrite(relayD2,HIGH);
  pinMode(relayD3,OUTPUT);
  digitalWrite(relayD3,HIGH);
  pinMode(relayD4,OUTPUT); 
  digitalWrite(relayD4,HIGH);
  pinMode(relayD5,OUTPUT);
  digitalWrite(relayD5,HIGH);
  pinMode(relayD6,OUTPUT);
  digitalWrite(relayD6,HIGH);
  pinMode(relayD7,OUTPUT);
  digitalWrite(relayD7,LOW);

  pinMode(buttonD1,INPUT_PULLUP);
  pinMode(buttonD2,INPUT_PULLUP);
  pinMode(buttonD3,INPUT_PULLUP);
  pinMode(buttonD4,INPUT_PULLUP);  
  pinMode(buttonD5,INPUT_PULLUP);
  pinMode(buttonD6,INPUT_PULLUP);  

  pinMode(CBHhongngoai, INPUT);
  pinMode(CBanhsang, INPUT);

  lcd.init();   
  lcd.backlight();
  lcd.setCursor(0, 2);
  lcd.print("May lanh: OFF");
  lcd.setCursor(0, 3);
  lcd.print("ID: None");
  SPI.begin();      // Initiate  SPI bus

  if (!rtc.begin()) 
  {
    //lcd.print("Couldn't find RTC");
    Serial.println("Couldn't find RTC");
    //while (1);
  }

  if (!rtc.isrunning()) 
  {
    //lcd.print("RTC is NOT running!");
    Serial.println("RTC is NOT running!");
  }
  
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
    //rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));// to set the time manualy 
}
void relayOnOff(int relay){
    switch(relay){
      case 1: 
             if(toggleState_1 == 1){
              digitalWrite(relayD1, LOW); // turn on relay 1
              toggleState_1 = 0;
              Serial.println("Device1 ON");
              }
             else{
              digitalWrite(relayD1, HIGH); // turn off relay 1
              toggleState_1 = 1;
              Serial.println("Device1 OFF");
              }
      break;
      case 2: 
             if(toggleState_2 == 1){
              digitalWrite(relayD2, LOW); // turn on relay 2
              toggleState_2 = 0;
              Serial.println("Device2 ON");
              }
             else{
              digitalWrite(relayD2, HIGH); // turn off relay 2
              toggleState_2 = 1;
              Serial.println("Device2 OFF");
              }
      break;
      case 3: 
             if(toggleState_3 == 1){
              digitalWrite(relayD3, LOW); // turn on relay 3
              toggleState_3 = 0;
              Serial.println("Device3 ON");
              }
             else{
              digitalWrite(relayD3, HIGH); // turn off relay 3
              toggleState_3 = 1;
              Serial.println("Device3 OFF");
              }
      break;
      case 4: 
             if(toggleState_4 == 1){
              digitalWrite(relayD4, LOW); // turn on relay 4
              toggleState_4 = 0;
              Serial.println("Device4 ON");
              }
             else{
              digitalWrite(relayD4, HIGH); // turn off relay 4
              toggleState_4 = 1;
              Serial.println("Device4 OFF");
              }
      break;
      case 5: 
             if(toggleState_5 == 1){
              digitalWrite(relayD5, LOW); // turn on relay 4
              toggleState_5 = 0;
              Serial.println("Device5 ON");
              }
             else{
              digitalWrite(relayD5, HIGH); // turn off relay 4
              toggleState_5 = 1;
              Serial.println("Device5 OFF");
              }
      break;
      case 6: 
             if(toggleState_6 == 1){
              digitalWrite(relayD6, LOW); // turn on relay 4
              toggleState_6 = 0;
              Serial.println("Device6 ON");
              }
             else{
              digitalWrite(relayD6, HIGH); // turn off relay 4
              toggleState_6 = 1;
              Serial.println("Device6 OFF");
              }
      break;
      default : break;      
      }  
}

void statusbutton(){
    //Manual Switch Control
    if (digitalRead(buttonD1) == LOW){
      delay(200);      
      relayOnOff(1);      
    }
    else if (digitalRead(buttonD2) == LOW){
      delay(200);
      relayOnOff(2);
    }
    else if (digitalRead(buttonD3) == LOW){
      delay(200);
      relayOnOff(3);
    }
    else if (digitalRead(buttonD4) == LOW){
      delay(200);
      relayOnOff(4);
    }
    else if (digitalRead(buttonD5) == LOW){
      delay(200);
      relayOnOff(5);
    }
    else if (digitalRead(buttonD6) == LOW){
      delay(200);
      relayOnOff(6);
    }
}
void DS1307_display() {
  
  DateTime now = rtc.now();
    
    lcd.setCursor(0, 1);
    if(now.hour()<=9)
    {
      lcd.print("0");
      lcd.print(now.hour());
    }
    else {
     lcd.print(now.hour()); 
    }
    lcd.print(':');
    if(now.minute()<=9)
    {
      lcd.print("0");
      lcd.print(now.minute());
    }
    else {
     lcd.print(now.minute()); 
    }
    lcd.print(':');
    if(now.second()<=9)
    {
      lcd.print("0");
      lcd.print(now.second());
    }
    else {
     lcd.print(now.second()); 
    }
    lcd.print("   ");

    lcd.setCursor(0, 0);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
    lcd.print(",");
    if(now.day()<=9)
    {
      lcd.print("0");
      lcd.print(now.day());
    }
    else {
     lcd.print(now.day()); 
    }
    lcd.print('/');
    if(now.month()<=9)
    {
      lcd.print("0");
      lcd.print(now.month());
    }
    else {
     lcd.print(now.month()); 
    }
    lcd.print('/');
    if(now.year()<=9)
    {
      lcd.print("0");
      lcd.print(now.year());
    }
    else {
     lcd.print(now.year()); 
    }
}
void hongngoai()
{
  Object1 = digitalRead(CBHhongngoai);
  if (Object1 == HIGH)
  {
    statusbutton();                                          
    DS1307_display(); 
    digitalWrite(relayD1, HIGH); 
    //Serial.println("Device1 OFF");
    digitalWrite(relayD2, HIGH); 
    //Serial.println("Device2 OFF");
    digitalWrite(relayD3, HIGH); 
    //Serial.println("Device3 OFF");
    digitalWrite(relayD4, HIGH); 
    //Serial.println("Device4 OFF");
    digitalWrite(relayD5, HIGH); 
    //Serial.println("Device5 OFF");
    digitalWrite(relayD6, HIGH);
    //Serial.println("Device6 OFF");
    toggleState_1 = 1,toggleState_2 = 1,toggleState_3 = 1,toggleState_4 = 1;toggleState_5 = 1;toggleState_6 = 1; 
  }
  else
  {
    anhsang();
    statusbutton();
    DS1307_display(); 
    Uart();
  }          
}
void Uart()
{
if (mySerial.available()) {
    String line = mySerial.readStringUntil('\n'); 
    DeserializationError error = deserializeJson(doc, line);
    // if (error) {
    //   Serial.print("deserializeJson() failed: ");
    //   Serial.println(error.c_str());
    //   return;
    // }
    serializeJson(doc, mySerial);
    //Serial.println(); 
    mySerial.flush();
    serializeJson(doc, Serial);
    Serial.println(); 
    String val1 = doc["L1"];            
    if(val1 == "1") 
      digitalWrite(relayD1, LOW);        
    else if(val1 == "0")     
      digitalWrite(relayD1, HIGH); 

    String val2 = doc["L2"];             
    if(val2 == "1") 
      digitalWrite(relayD2, LOW);        
    else if(val2 == "0")     
      digitalWrite(relayD2, HIGH); 

    String val3 = doc["L3"];             
    if(val3 == "1") 
      digitalWrite(relayD3, LOW);        
    else if(val3 == "0")     
      digitalWrite(relayD3, HIGH); 

    String val4 = doc["L4"];             
    if(val4 == "1") 
      digitalWrite(relayD4, LOW);        
    else if(val4 == "0")     
      digitalWrite(relayD4, HIGH);    

    String val5 = doc["F1"];
    if(val5 == "1") 
      digitalWrite(relayD5, LOW);        
    else if(val5 == "0")     
      digitalWrite(relayD5, HIGH);  

    String val6 = doc["F2"];
    if(val6 == "1") 
      digitalWrite(relayD6, LOW);        
    else if(val6 == "0")     
      digitalWrite(relayD6, HIGH);   

    String val7 = doc["Z1"];
    String val8 = doc["ID"];
    if(val7 == "1")
    {
      lcd.setCursor(0, 2);
      lcd.print("May lanh: ON ");
      lcd.setCursor(0, 3);
      lcd.print("ID:    " +val8);
    }
    else if (val7 == "0")
    {
      lcd.setCursor(0, 2);
      lcd.print("May lanh: OFF");
      lcd.setCursor(0, 3);
      lcd.print("ID: None");
    }
    String val9 = doc["T"];
    if(val9 == "1")
    {
      lcd.setCursor(10, 3);
      lcd.print("FB: ON");
    }
    else if (val9 == "0")
    {
      lcd.setCursor(10, 3);
      lcd.print("FB: OFF");
    }
  }                                                                                 
}
void anhsang()
{
  CBASstate = digitalRead(CBanhsang);
  // String test = String(CBASstate);
  // String test1 = "";
  // if (!(test.equals(test1))) {
  //   test1 = test;
  //   doc["L5"]=CBASstate;
  //   serializeJson(doc, mySerial);
  //   //serializeJson(doc, Serial);
  //   //Serial.println(); 
  //   //mySerial.flush();
  //   delay(1000);
  // } 
  if (CBASstate == HIGH) {
    digitalWrite(relayD7, HIGH);
  } else {
    digitalWrite(relayD7, LOW);
  }
}
void loop () 
{ 
  hongngoai();
  anhsang();
  statusbutton();                                          
  DS1307_display(); 
  Uart();
}

