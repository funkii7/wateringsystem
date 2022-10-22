// GPIO numbers of the pins
#define RELAY_PIN 9 // relay com pin
#define SENSOR_PIN 2 // 
/*
#define SD_MISO_PIN 5 // white cable
#define SD_MOSI_PIN 6 // dark green cable
#define SD_SCK_PIN 4 // yellow cable
#define SD_CS_PIN 7 // orange cable
*/

#define WATERING_MOISTURE_LIMIT 1800 // for values > WATERING_MOISTURE_LIMIT the pump activates for WATERING_TIME_SEC
#define WATERING_TIME_SEC 3 // plant will be watered for WATERING_TIME_SEC seconds
#define CHECK_DELAY_SEC 30 // every CHECK_DELAY_SEC seconds the moisture data will be checked
#define SD_CARD_WRITE_DELAY 30 // every SD_CARD_WRITE_DELAY the data gets written to the sd card
#define DISCORD_WRITE_DELAY 10 // every DISCORD_DELAY * SD_CARD_WRITE_DELAY seconds the sensor data gets sent to the discord webhook

// Discord stuff
#include <Discord_WebHook.h>
Discord_Webhook discord;
#define WATERING_MESSAGE "water" 

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "time.h"
#include "credentials.h"


// initialize discord stuff for webhook
void connectdiscord(void){
  discord.begin(DISCORD_WEBHOOK);
  discord.addWiFi(WIFI_SSID, WIFI_PASSWORD); // wifi credentials
  discord.connectWiFi();
  discord.setTTS();  
}

int writeMoistureToFile(int moisture){
  File file = SD.open("/log", FILE_APPEND);
  if(!file){
    Serial.println("file open failed");
    return -1;
  }
  if(!file.println(String(moisture))){
    Serial.println("file.println append failed");
    return -1;
  }
  return 0;
}

int pump(){ // activate pump
//  discord.send(WATERING_MESSAGE);
    pinMode(RELAY_PIN, OUTPUT);
    delay(WATERING_TIME_SEC * 1000);
    pinMode(RELAY_PIN, INPUT);
}

void setup() {
  Serial.begin(115200);
  connectdiscord();
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }

  pinMode(RELAY_PIN, INPUT); // set relay to low per default
  Serial.println("Setup complete");
}
// temp variables for storing the summed up moisture values and counters
unsigned int discordcount = 0;
unsigned int discordtempmoisture = 0;
unsigned int filewritecount = 0;
unsigned int filewritetempmoisture = 0; 

// check sensor every second and add the value to filewritetempmoisture. after 30 seconds divide that through 30 to get the average value. 
// append that value to the logfile 
// same for the discord message but after 300 seconds
void loop() { 
  int sensorvalue = analogRead(SENSOR_PIN);
  filewritecount++;
  filewritetempmoisture += sensorvalue;
  Serial.println(sensorvalue);
  if(filewritecount > SD_CARD_WRITE_DELAY){ // get the average value after SD_CARD_WRITE_DELAY seconds
    filewritetempmoisture /= SD_CARD_WRITE_DELAY; // calculate average value
    filewritecount = 0; // reset counter
    /* debug
    Serial.println("---");
    Serial.println(filewritetempmoisture);
    Serial.println("---");
    */
    writeMoistureToFile(filewritetempmoisture);

    discordtempmoisture += filewritetempmoisture;
    filewritetempmoisture = 0; // reset value
    discordcount++;
    if(discordcount > DISCORD_WRITE_DELAY){
      discord.send(String(discordtempmoisture / DISCORD_WRITE_DELAY)); // calculate average value and send it to discord
      discordcount = 0;
      discordtempmoisture = 0;
    }

  // activate the pump for WATERING_TIME_SEC seconds if the configured moisture limit is reached
    if(sensorvalue > WATERING_MOISTURE_LIMIT){
      pump();
    }
  }
  delay(1000);
}
