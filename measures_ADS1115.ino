//***************************************//
// Measures (ESP32WROVER-16MB)           //
//                                       //
// jenkutler.com                         //
// **************************************//

//To do: 
//fix issue caused by i2c blocking core 1 for ads1115 readings
//Create a 4-character code for node-red to get rid of spam connections and crawlers
//compare readings from filtered and unfiltered eda pins and pick one


#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <WiFi.h>
#include <Arduino.h>
#include <WiFiManager.h>           //https://github.com/tzapu/WiFiManager
#include "FS.h"
#include "SPIFFS.h"
#include "ADS1X15.h"               //https://github.com/RobTillaart/ADS1X15

ADS1115 ADS(0x48); 

#define FORMAT_SPIFFS_IF_FAILED true
const char filename[] = "/i.txt";
File file;

unsigned long ohShit; //in case node-red doesn't respond fast enough with stream

char macIdentifierNodeRed[17];

char uniquePath[20];
const char *host = "146.190.221.89";
String path = "/000000000000.mp3";
int httpPort = 8000;

WiFiClient transferClient;
const int transferPort = 49153;

WiFiClient initClient;
const int initPort = 49152;

//int edasamplecounter = 0;
size_t edaBuffSize = 128;
int16_t edaUnfilt[128] = {0}; 

//#define EDAUNFILTPIN 33
#define PLAYING_INDICATOR 25
#define END_INDICATOR 26
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS      27     // VS1053 chip select pin (output)
#define VS1053_DCS     14     // VS1053 Data/command select pin (output)
#define VS1053_DREQ    13     // VS1053 Data request, ideally an Interrupt pin

#define VOLUME_KNOB    34

int lastvol = 30;

Adafruit_VS1053 musicPlayer =  Adafruit_VS1053(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ);

WiFiClient audioClient;

void setup() {
  WiFi.mode(WIFI_STA); 
  
  Serial.begin(115200);

  // Wait for VS1053 to initialize
  delay(3000);
   if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("VS1053 Failure"));
     while (1);
  }
  
  musicPlayer.setVolume(lastvol, lastvol);
  pinMode(VOLUME_KNOB, INPUT_PULLUP);
  pinMode(PLAYING_INDICATOR, OUTPUT);
  pinMode(END_INDICATOR, OUTPUT);
  digitalWrite(PLAYING_INDICATOR, LOW);
  digitalWrite(END_INDICATOR, LOW);

  WiFiManager wm;
  bool res;
  res = wm.autoConnect("Measures","measures");
  if(!res){
    Serial.println(F("WiFi Failure"));
    //ESP.restart();
  }else{
    Serial.println(F("WiFi Connected"));
  }
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
  }

  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
    Serial.println(F("SPIFFS Mount Failed"));
    return;
  }

  if (!SPIFFS.remove(filename)){
    Serial.println(F("File not deleted"));
  };

  delay(10);

  file = SPIFFS.open(filename, FILE_APPEND);
  if(!file){
    Serial.println(F("SPIFFS can't open file"));
  }
  if(!ADS.begin()){
    Serial.println(F("ADS Failed"));
    while(1);
  }

  getMacIdentifier(); //gets mac address for node-red

  //send mac address to node-red
  if(!initClient.connect(host, initPort)){
    Serial.println(F("Init TCP failed"));
    while(true){
      ;
    }
  }else{
    Serial.println(F("Init TCP success"));
    initClient.println(macIdentifierNodeRed);
    delay(10);
    initClient.stop();
  }
  
  delay(200);

  ADS.requestADC(0);
  
  if (!audioClient.connect(host, httpPort)) {
    Serial.println(F("Icecast failed"));
    return;
  }
  
  // This will send the stream request to the server
  audioClient.print(String("GET ") + path + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  //Serial.println("endofsetup");printMem();
}

uint8_t mp3buff[32];   // vs1053 audio buffer
size_t bytes_read;
int readCounter = 0;
bool needNewSample = false; //because it is first called in setup

void loop() {

  if(audioClient.connected()){ //still in data acquisition section of program
    digitalWrite(PLAYING_INDICATOR, HIGH);
    
    //in order of priority
    if(musicPlayer.readyForData() && audioClient.available()) {
      uint8_t bytesread = audioClient.read(mp3buff, 32);
      musicPlayer.playData(mp3buff, bytesread);
    }

    if((!musicPlayer.readyForData()) && (readCounter == edaBuffSize)){ 

      for (int i=0; i<edaBuffSize; i++){
        file.println(edaUnfilt[i]);
      }
      //Serial.print("File Size: ");
      //Serial.println(file.size());
      readCounter = 0;
    } 
    
    if(!musicPlayer.readyForData() && !ADS.isBusy()){
      //read sample from i2c ads1115
      if(needNewSample == false){
        edaUnfilt[readCounter] = ADS.getValue(); 
        //Serial.print("Value: "); Serial.println(edaUnfilt[readCounter]);
        //Serial.print("Read Counter: "); Serial.println(readCounter);
        readCounter++;
        needNewSample = true;
      }
    } 
    if (!musicPlayer.readyForData() && !ADS.isBusy()){
      if(needNewSample == true){
        ADS.requestADC(0); // this might need to go in a different function 
        needNewSample = false;
      }
    }
    
    if ((!musicPlayer.readyForData()) && (readCounter < edaBuffSize/4)){ // /4 is to make this less frequent
      int vol = analogRead(VOLUME_KNOB);
      vol /= 25;
      if (abs(vol - lastvol) > 3) {
        lastvol = vol;
        musicPlayer.setVolume(lastvol, lastvol);
      }
    }    
  }

  if(!audioClient.connected()){ //move to upload section of program
  digitalWrite(PLAYING_INDICATOR, LOW);
  digitalWrite(END_INDICATOR, HIGH);
  //Serial.println("AUDIOCLIENT NO LONGER CONNECTED"); 
  file.close();
  audioClient.stop();
  uploadData();
  endOfProgram(); 
  }
}

void getMacIdentifier(){
  byte mac[6];
  WiFi.macAddress(mac);
  for (int i = 5; i>=0; i--){
    if(mac[i] < 16){
      Serial.print(F("getMacIdentifier() has failed"));
    }
  }
  sprintf(macIdentifierNodeRed, "%02X%02X%02X%02X%02X%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  sprintf(uniquePath, "/%02X%02X%02X%02X%02X%02X.mp3", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  path = uniquePath;
  Serial.println(uniquePath);
}

void uploadData(){
  ohShit = millis();
  if(ohShit < 10000){
    ESP.restart();
  }
  
  delay(5);
  //Serial.println("UPLOAD DATA CALLED");
  if(!transferClient.connect(host, transferPort)){
    Serial.println(F("transferClient connection failed"));
    while(true){
      ;
    }
  } else {
    //Serial.println(F("DATA TRANSFER INITIATED"));

    char fileBuffer[32];
    
    file = SPIFFS.open(filename);

    if(!file){
      Serial.print("Could not open file");
      while(true){
        ;
      }
    }

    size_t fileSize = (file.size());
    size_t sizeToTransfer = fileSize;
    
    //Serial.println(file.size());
    
    while(file.available()){
      while (sizeToTransfer > 0){
        char edaBuffer[edaBuffSize * 2] = {0};
        file.readBytes(edaBuffer, edaBuffSize);
        transferClient.write(edaBuffer, edaBuffSize);
        sizeToTransfer -=  edaBuffSize;
        //Serial.println(sizeToTransfer);
        if(sizeToTransfer <= edaBuffSize){
          file.close();
        }
      }
    }
         
    transferClient.stop();
    //Serial.println(F("DATA TRANSFER COMPLETE"));
    digitalWrite(END_INDICATOR, LOW);
   }
}


void endOfProgram(){
  while (1){
    digitalWrite(END_INDICATOR, HIGH);
    delay(100);
    musicPlayer.sineTest(0x44, 1000);    // Annoying tone prompting user to turn off device
    digitalWrite(END_INDICATOR, LOW);
    delay(100);
  }
}

/*
void printMem() {
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_SPIRAM)            : %7d\n", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_INTERNAL)          : %7d\n", heap_caps_get_free_size(MALLOC_CAP_INTERNAL) );
  Serial.printf("heap_caps_get_free_size(MALLOC_CAP_DEFAULT)           : %7d\n", heap_caps_get_free_size(MALLOC_CAP_DEFAULT) );
  log_d("Free heap: %d", ESP.getFreeHeap());
  Serial.println();
}
*/
