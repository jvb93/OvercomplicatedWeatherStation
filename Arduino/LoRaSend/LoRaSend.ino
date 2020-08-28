#include <Adafruit_SHT31.h>
#include <axp20x.h>
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//915E6 for North America
#define BAND 915E6

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  300 

AXP20X_Class axp;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

String weatherOutput;
String voltageOutput;


void setup()
{
    Serial.begin(115200);
    Wire.begin(21, 22);

    // setup power mgmt
    if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS))
    {
        Serial.println("AXP192 Begin PASS");
    }
    else
    {
        Serial.println("AXP192 Begin FAIL");
    }
    axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
    axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
    axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
    axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
    axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
    

   // init temperature sensor over i2c
   if (!sht31.begin(0x44)) {   
      Serial.println("Couldn't find SHT31");
   }

     //SPI LoRa pins
    SPI.begin(SCK, MISO, MOSI, SS);

    // setup LoRa transceiver module
    LoRa.setPins(SS, RST, DIO0);
    
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    //max transmit dBm
    LoRa.setTxPower(20);

    Serial.println("LoRa Initializing OK!");
    delay(2000);

    // take measurements
    float t = sht31.readTemperature();
    float h = sht31.readHumidity(); 
    float v = axp.getBattVoltage();

    // send data
    LoRa.beginPacket();
    LoRa.printf("%.2f|%.2f|%.2f",t ,h, v);
    LoRa.endPacket();

    // wait for packet send to complete and then go to sleep
    delay(2000);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void loop()
{
   
}
