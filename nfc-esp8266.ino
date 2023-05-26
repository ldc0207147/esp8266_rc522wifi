#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <Servo.h>

#define SS_PIN D8
#define RST_PIN D0
MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key;
byte nuidPICC[4];
byte uid[]={0x80, 0xC9, 0xBF, 0x25};
const char* ssid     = "SA-AP";         // 填入你連線區網Wi-Fi的SSID(name)
const char* password = "sa623500";     // Wi-Fi的密碼

const char *host = "192.168.31.208";



// Set IFTTT Webhooks event name and key
#define IFTTT_Key "dFbGit8PT6SSq6fhtIOJnX"
#define IFTTT_Event "rc522" // or whatever you have chosen
String IFTTT_Value1 = "";
String IFTTT_Value2 = "";
#define IFTTT_Value3 "123"
WiFiClient client;
Servo servo;

void setup() {
  String IFTTT_Value1 = "";
  String IFTTT_Value2 = "";
 	Serial.begin(115200);
 	SPI.begin(); // Init SPI bus
 	rfid.PCD_Init(); // Init MFRC522
 	rfid.PCD_DumpVersionToSerial();
 	for (byte i = 0; i < 6; i++) {
 			key.keyByte[i] = 0xFF;
 	}

  servo.attach(2);
  servo.write(0);

  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("連接WiFi到 ");
  Serial.print(ssid); Serial.println(" ...");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // 等待Wi-Fi連線
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println("WIFI已連線!");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());         // ESP8266連線到計算機的IP
}
void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  String ADCData, station, getData, Link;
 	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
 	if ( ! rfid.PICC_IsNewCardPresent())
 			return;
 	// Verify if the NUID has been readed
 	if ( ! rfid.PICC_ReadCardSerial())
 			return;
 	MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
 	// Check is the PICC of Classic MIFARE type
 	if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
 					piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
 					piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
 			Serial.println(F("您的卡片不符合偵測類型無法識別"));
 			return;
 	}
 	if (rfid.uid.uidByte[0] != nuidPICC[0] ||
 					rfid.uid.uidByte[1] != nuidPICC[1] ||
 					rfid.uid.uidByte[2] != nuidPICC[2] ||
 					rfid.uid.uidByte[3] != nuidPICC[3] ) {
 			// Store NUID into nuidPICC array
 			for (byte i = 0; i < 4; i++) {
 					nuidPICC[i] = rfid.uid.uidByte[i];
 			}
      
      bool they_match = true; // 初始值是假設為真 
      for ( int i = 0; i < 4; i++ ) { // 卡片UID為4段，分別做比對
        if ( uid[i] != rfid.uid.uidByte[i] ) { 
          they_match = false; // 如果任何一個比對不正確，they_match就為false，然後就結束比對
          break; 
        }
      }
      if(they_match){
      Serial.print(F("解鎖成功!"));
      Serial.println();
      servo.write(180);
      
      IFTTT_Value1 = "";
      IFTTT_Value2 = "";
      for (int i = 0; i < 4; i++) {
      if (i > 0) {
      IFTTT_Value1 += ",";
      }
      IFTTT_Value1 += String(rfid.uid.uidByte[i], HEX);
      }
      IFTTT_Value2 = they_match ? "true" : "false";
      
      send_webhook();
      send_webhook1();
      delay(3000);
      servo.write(0);
      }else{
        Serial.print(F("解鎖失敗!"));
        Serial.println();

        IFTTT_Value1 = "";
        IFTTT_Value2 = "";
        for (int i = 0; i < 4; i++) {
        if (i > 0) {
        IFTTT_Value1 += ",";
        }
        IFTTT_Value1 += String(rfid.uid.uidByte[i], HEX);
        }
        IFTTT_Value2 = they_match ? "true" : "false";
        
        send_webhook();
        send_webhook1();
        delay(1000);
        }
      rfid.PICC_HaltA();
 	}

   
}
/**
 		Helper routine to dump a byte array as hex values to Serial.
*/
void printHex(byte *buffer, byte bufferSize) {
 	for (byte i = 0; i < bufferSize; i++) {
 			Serial.print(buffer[i] < 0x10 ? " 0" : " ");
 			Serial.print(buffer[i], HEX);
 	}
}
/**
 		Helper routine to dump a byte array as dec values to Serial.
*/
void printDec(byte *buffer, byte bufferSize) {
 	for (byte i = 0; i < bufferSize; i++) {
 			Serial.print(buffer[i] < 0x10 ? " 0" : " ");
 			Serial.print(buffer[i], DEC);
 	}
}

void sendCommandToWifi(String s){
  Serial1.println(s);  //向ESP8266傳送字串s
  Serial.println(s);  //顯示字串s到監控視窗
}

void send_webhook(){
  // construct the JSON payload
  String jsonString = "";
  jsonString += "{\"value1\":\"";
  jsonString += IFTTT_Value1;
  jsonString += "\",\"value2\":\"";
  jsonString += IFTTT_Value2;
  jsonString += "\",\"value3\":\"";
  jsonString += IFTTT_Value3;
  jsonString += "\"}";
  int jsonLength = jsonString.length();  
  String lenString = String(jsonLength);
  // connect to the Maker event server
  client.connect("maker.ifttt.com", 80);
  // construct the POST request
  String postString = "";
  postString += "POST /trigger/";
  postString += IFTTT_Event;
  postString += "/with/key/";
  postString += IFTTT_Key;
  postString += " HTTP/1.1\r\n";
  postString += "Host: maker.ifttt.com\r\n";
  postString += "Content-Type: application/json\r\n";
  postString += "Content-Length: ";
  postString += lenString + "\r\n";
  postString += "\r\n";
  postString += jsonString; // combine post request and JSON
  
  client.print(postString);
  delay(500);
  client.stop();
}

void send_webhook1() {
  // construct the JSON payload
  String jsonString = "";
  jsonString += "{\"value1\":\"";
  jsonString += IFTTT_Value1;
  jsonString += "\",\"value2\":\"";
  jsonString += IFTTT_Value2;
  jsonString += "\",\"value3\":\"";
  jsonString += IFTTT_Value3;
  jsonString += "\"}";
  
  // construct the URL
  String url = "http://192.168.31.208/log.php?card=" + IFTTT_Value1 + "&state=" + IFTTT_Value2;
  
  // send HTTP POST request
  WiFiClient client;
  HTTPClient http;
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonString);
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP POST request sent. Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.println("HTTP POST request failed.");
  }
  
  http.end();
}

