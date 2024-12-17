#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>
#include <HTTPSRedirect.h>
#include <StreamUtils.h>

#define RX_PIN 4 // D2
#define TX_PIN 5 // D1
SoftwareSerial mySerial(RX_PIN, TX_PIN);
//SoftwareSerial mySerial(4, 5); // RX-D2 | TX-D1
//---------------------------------------------------------------------------------------------------------
// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "hehehe"
#define WIFI_PASSWORD "04032002"
FirebaseData fbdo;
StaticJsonDocument<300> doc;
//StaticJsonDocument<200> doc1;
bool signupOK = false;
// Đường dẫn địa chỉ Google Script Deployment ID:
const char *GScriptId = "AKfycbzg2Xx-dOzDLrP2v_GdMuaYvyPrW_dF0H8ozdS65uqyMJu4jR1EHX2ttUreqHdbIVru";
String gate_number = "";
// đặt biến mang ý nghĩa gửi dữ liệu, "command\": \"insert_row\" chèn một hàng mới vào dữ liệu,"sheet_name": "Sheet1" là tên của tập dữ liệu trên  gg sheet, "value" là giá trị được gửi lên 
String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";
//---------------------------------------------------------------------------------------------------------
// Cài cấu hình cho GG Sheet
const char* host        = "script.google.com";
const int   httpsPort   = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;
//------------------------------------------------------------
// khai báo biến chứa dữ liệu sẽ được gửi lên gg sheet
String student_id;
//------------------------------------------------------------
//khai báo các block chứa nội dung dữ liệu, định nghĩa kích thước
int blocks[] = {4,5};
#define total_blocks  (sizeof(blocks) / sizeof(blocks[0]))
//------------------------------------------------------------
//định nghĩa chân kết nối
#define RST_PIN  0  //D3
#define SS_PIN   2  //D4
//------------------------------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN);// dùng để khởi tạo module MFRC522
MFRC522::MIFARE_Key key;//khai báo một biến key mang kiểu dữ liệu MIAFARE_Key dùng để quản lý trong việc xác thực và truy cập dữ liệu  
MFRC522::StatusCode status; // khai báo biến status dùng để đọc trạng thái đọc và ghi của thẻ RFID
//------------------------------------------------------------
/* khai báo khối block */
int blockNum = 2,n,nn,nnn;  
byte bufferLen = 18;          
byte readBlockData[18];

int UID[4], UIDD[4], UIDDD[4];
int ID1[4] = {99, 81, 109, 41};
int ID4[4] = {229, 237, 154, 138};
int ID3[4] = {148, 209, 153, 138};
int j=0;//D4
// Insert Firebase project API Key
#define API_KEY "AIzaSyDeptJdopNqNt-KkEb-81OUU8LSL-o9254"

// Insert Authorized Username and Corresponding Password
//#define USER_EMAIL "tinhhcmute@gmail.com"
//#define USER_PASSWORD "123456789"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://kltn2025-f323a-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;

// Variables to save database paths
String listenerPath = "Outputs/";

int LED1 = 16;
int ledStatus = 0;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Callback function that runs on database changes
void streamCallback(FirebaseStream data){
  // Get the path that triggered the function
  String streamPath = String(data.dataPath());
  // if the data returned is an integer, there was a change on the GPIO state on the following path /{gpio_number}
  if (data.dataTypeEnum() == fb_esp_rtdb_data_type_integer){
    String Name = streamPath.substring(1);
    int Value1 = data.intData();
    doc[Name] = Value1;
    serializeJson(doc, mySerial);
    //mySerial.println(); 
    mySerial.flush();
    // serializeJson(doc, Serial);
    // Serial.println(); 
  }
  if (data.dataTypeEnum() == fb_esp_rtdb_data_type_json){
    FirebaseJson json = data.to<FirebaseJson>();
    // To iterate all values in Json object
    size_t count = json.iteratorBegin();
    for (size_t l = 0; l < count; l++){
        FirebaseJson::IteratorValue value = json.valueAt(l);
        String Name = value.key.c_str();
        int Value1 = value.value.toInt();
        doc[Name] = Value1;
        serializeJson(doc, mySerial);
        //mySerial.println(); 
        mySerial.flush();
        // serializeJson(doc, Serial);
        // Serial.println(); 
    }
    json.iteratorEnd(); // required for free the used memory in iteration (node data collection)
  }
  Serial.println();
}

void streamTimeoutCallback(bool timeout){
  if (timeout)
    Serial.println("stream timeout, resuming...\n");
  if (!stream.httpConnected())
    Serial.printf("error code: %d, reason: %s\n\n", stream.httpCode(), stream.errorReason().c_str());
}
void ReadDataFromBlock(int blockNum, byte readBlockData[]) 
{ 
  for (byte i = 0; i < 6; i++) {key.keyByte[i] = 0xFF;}
  //----------------------------------------------------------------------------
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockNum, &key, &(mfrc522.uid));
  //----------------------------------------------------------------------------
  if (status != MFRC522::STATUS_OK){return;}
  //----------------------------------------------------------------------------
  status = mfrc522.MIFARE_Read(blockNum, readBlockData, &bufferLen);
  if (status != MFRC522::STATUS_OK) {return;}
  //----------------------------------------------------------------------------
  else {
    readBlockData[16] = ' ';
    readBlockData[17] = ' ';
  }
}
void diemdanh()
{
  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr){
    if (!client->connected()){
      int retval = client->connect(host, httpsPort); return;        
    }
  }
  //----------------------------------------------------------------
  mfrc522.PCD_Init();
  if ( (! mfrc522.PICC_IsNewCardPresent())||(! mfrc522.PICC_ReadCardSerial()) ) {return;}
  //----------------------------------------------------------------
  String values = "", data;
  for (byte i = 0; i < total_blocks; i++) {
    ReadDataFromBlock(blocks[i], readBlockData);
    if(i == 0){
      data = String((char*)readBlockData);
      int tam = data.toInt();
      doc["ID"] = tam;
      data.trim();
      values = "\"" + data + ",";
    }
    else{
      data = String((char*)readBlockData);
      data.trim();
      values += data + ",";
    }   
  }
  values += gate_number + "\"}";
  //----------------------------------------------------------------
  payload = payload_base + values;   
  Serial.println(payload);
  if(client->POST(url, host, payload)) {} 
  //---------------------------------------------------------------- 
  UIDthe();
  //----------------------------------------------------------------
    // serializeJson(doc, mySerial);
    // mySerial.println(); 
    // serializeJson(doc, Serial);
    // Serial.println();  
  //----------------------------------------------------------------  
  //delay(2000) ;
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
void UIDthe()
{
  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") + String(mfrc522.uid.uidByte[i], HEX) + (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  //Serial.println(strID);
  //if(strID.indexOf("63:51:6D:29")>=0 || strID.indexOf("") >= 0) 
  if(strID.indexOf("63:51:6D:29")>=0) 
  { 
    ledStatus = !ledStatus;
    digitalWrite(LED1,!ledStatus);
    doc["Z1"] = ledStatus;
    serializeJson(doc, mySerial);
    mySerial.flush();
    serializeJson(doc, Serial);
    // Serial.println(); 
    Firebase.RTDB.setInt(&fbdo, "/Outputs/Z1", ledStatus);
    delay(500);
  }
}
void setup(){
  Serial.begin(9600);
  //Serial.setRxBufferSize(1024);
  mySerial.begin(9600);
  //mySerial.setRxBufferSize(1024);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  initWiFi();
  SPI.begin();
  //Sử dụng lớp HTTPSRedirect để tạo kết nối TLS mới,TLS(Transport Layer Security) là bảo mật các thông tin nhạy cảm trong quá trình truyền trên internet
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  //----------------------------------------------------------
  Serial.print("Connecting to OK");
  //---------------------------------------------------------- 
  bool flag = false;
  for(int k=0; k<5; k++){ 
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
      flag = true;
      break;
    }
  }
  //----------------------------------------------------------
  delete client;    // delete HTTPSRedirect object
  client = nullptr; // delete HTTPSRedirect object
  // Assign the api key (required)
  config.api_key = API_KEY;
  // Assign the user sign in credentials
  //auth.user.email = USER_EMAIL;
  //auth.user.password = USER_PASSWORD;
  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;
  /* Sign up */
  Serial.print("Connecting to Firebase... ");
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("OK");
    doc["T"] = 1;
    serializeJson(doc, mySerial);
    mySerial.flush();
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    doc["T"] = 0;
    serializeJson(doc, mySerial);
    mySerial.flush();
  }
  Firebase.reconnectWiFi(true);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Streaming (whenever data changes on a path)
  // Begin stream on a database path --> board1/outputs/digital
  if (!Firebase.RTDB.beginStream(&stream, listenerPath.c_str()))
    Serial.printf("stream begin error, %s\n\n", stream.errorReason().c_str());

  // Assign a calback function to run when it detects changes on the database
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);

  delay(2000);
}

void loop(){
  diemdanh();
  if (Firebase.isTokenExpired()){
    Firebase.refreshToken(&config);
    Serial.println("Refresh token");
  }
  // if (mySerial.available()) {
  //     String line = mySerial.readStringUntil('\n');
  //     DeserializationError error = deserializeJson(doc, line);
  //     String test = doc["L5"];
  //     String test1 = "";
  //     if (!(test.equals(test1))) {
  //       test1 = test;
  //       Firebase.RTDB.setString(&fbdo, "/Outputs/L5", test);
  //     } 
  //     mySerial.flush();
  //     delay(1000);
  // }
}