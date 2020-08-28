#include "Secrets.h"

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
HTTPClient http;
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
  
  LoRa.onReceive(onReceivePacket);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  //reboot on a 10 minute interval
  if(millis() > 600000)
  {
    Serial.println("resetting now!");
    Serial.flush();
    ESP.restart();
  }
}

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

void onReceivePacket(int packetSize)
{
  char packet[64];
  tryReceivePacket(packet, packetSize);
  
  double values[] = {0,0,0,0};
  extractValuesFromPacket(packet, values);
  
  double celsius = *values;
  double relative_humidity = *(values + 1);
  double voltage = *(values + 2);
  double pressure = *(values + 3);
  int rssi = LoRa.packetRssi();
  
  displayInformation(rssi, celsius, relative_humidity, voltage, pressure);
  
  sendHttp(celsius, relative_humidity, voltage, pressure);
}

bool tryReceivePacket(char* packet, int packetSize) {
  int index = 0;
  for (int i = 0; i < packetSize; i++) {
     packet[i] = (char)LoRa.read();
     index++;
  }
  // terminate string
  packet[index] = '\0';
  return true;
 
}

void extractValuesFromPacket(char* packet, double* values) {
    char delimiter[] = "|";
    char *ptr = strtok(packet, delimiter);
    int index = 0;
    
    while(ptr != NULL)
    {
      double val = atof(ptr);
      Serial.println(val);
      values[index] = val;
      ptr = strtok(NULL, delimiter);
      index++;
    }
}

void displayInformation(int rssi, double celsius, double relative_humidity, double voltage, double pressure){
  // Display information
  display.clearDisplay();
  
  display.setCursor(0,0);
  display.printf("C: %.2f", celsius);
  display.setCursor(0,20);
  display.printf("RH: %.2f", relative_humidity);
  display.setCursor(0,30);
  display.printf("mV: %.2f", voltage);
  display.setCursor(0,40);
  display.printf("Pa: %.2f", pressure);
  display.setCursor(0,50);
  display.print("RSSI:");
  display.setCursor(30,50);
  display.print(rssi);
  display.display();   
}


void sendHttp(double celsius, double relative_humidity, double voltage, double pressure){
  int responseCode = 0;
  char weatherPayload[100];
  sprintf(weatherPayload, "{\"celsius\": %.2f, \"relative_humidity\": %.2f, \"pressure\": %.2f}", celsius, relative_humidity, pressure);
  Serial.println("POST");
  Serial.println(weatherUrl);
  Serial.println(weatherPayload);
  http.begin(weatherUrl);
  http.addHeader("Content-Type", "application/json");
  responseCode = http.POST(weatherPayload);
  Serial.println(responseCode);
  http.end();

  char voltagePayload[80];
  sprintf(voltagePayload, "{\"voltage\": %.2f}", voltage);
  Serial.println("POST");
  Serial.println(batteryUrl);
  Serial.println(voltagePayload);
  http.begin(batteryUrl);
  http.addHeader("Content-Type", "application/json");
  responseCode = http.POST(voltagePayload);
  Serial.println(responseCode);
  http.end();
}
