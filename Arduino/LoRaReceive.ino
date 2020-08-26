#include "Secrets.h"

#include <ArduinoJson.h>

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// API Url is in Secrets.h!
String url = ApiUrl;
String weatherUrl = url + "/weather";
String batteryUrl = url + "/battery";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData;

const size_t incomingCapacity = JSON_OBJECT_SIZE(3) + 40;
DynamicJsonDocument incomingDoc(incomingCapacity);

const size_t weatherCapacity = JSON_OBJECT_SIZE(2);
DynamicJsonDocument weatherDoc(weatherCapacity);
String weatherOutput = "";

const size_t voltageCapacity = JSON_OBJECT_SIZE(1);
DynamicJsonDocument voltageDoc(voltageCapacity);
String voltageOutput = "";

void connectWiFi(){
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  
  // WiFiSSD and WiFiPass are in Secrets.h!
  Serial.println(WiFiSSID);
  WiFi.begin(WiFiSSID, WiFiPass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  // Print local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() { 
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA RECEIVER ");
  display.display();
  
  //initialize Serial Monitor
  Serial.begin(115200);

  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();  

  connectWiFi();

 
}

void writeToApi() {
  HTTPClient http;
  http.begin(weatherUrl);
  http.addHeader("Content-Type", "application/json");
  http.POST(weatherOutput);
  http.end();
  
  http.begin(batteryUrl);
  http.addHeader("Content-Type", "application/json");
  http.POST(voltageOutput);
  http.end();
}

void loop() {

  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    //read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.println(LoRaData);
    }
    //print RSSI of packet
    int rssi = LoRa.packetRssi();
  
    deserializeJson(incomingDoc, LoRaData);
    
    int voltage = incomingDoc["voltage"]; // 3790
    float celsius = incomingDoc["celsius"]; // 25.25
    float relative_humidity = incomingDoc["relative_humidity"]; // 45.543
  
     // Display information
     display.clearDisplay();
     display.setCursor(0,0);
     display.printf("C: %.2f", celsius);
     display.setCursor(0,20);
     display.printf("RH: %.2f", relative_humidity);
     display.setCursor(0,30);
     display.printf("mV: %d", voltage);
     display.setCursor(0,40);
     display.print("RSSI:");
     display.setCursor(30,40);
     display.print(rssi);
     display.display();   
  
    weatherDoc["celsius"] = celsius;
    weatherDoc["relative_humidity"] = relative_humidity;
    voltageDoc["voltage"] = voltage;
    
    serializeJson(weatherDoc, weatherOutput);  
    serializeJson(voltageDoc, voltageOutput);
    writeToApi();
  }
}
