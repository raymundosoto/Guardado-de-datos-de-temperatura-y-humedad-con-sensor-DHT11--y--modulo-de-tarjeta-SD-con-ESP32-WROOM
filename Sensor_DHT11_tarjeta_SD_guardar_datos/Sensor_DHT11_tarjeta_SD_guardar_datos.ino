/*
 Programa que permite guardar los datos
*/

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 4
#define DHTTYPE    DHT11     // DHT 11   

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int chipSelect = SS;

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.println("LISTDIR.....");
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.println("CREATEDIR.....");
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path){
Serial.println("REMOVEDIR.....");
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path){
Serial.println("READFILE.....");
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);


Serial.println("WRITEFILE.....");
  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){

  Serial.println("APPENDFILE.....");
  
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.println("RENAMEFILE.....");
  
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
 Serial.println("DELETEFILE.....");
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  
  Serial.println("TESTFILEIO.....");
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}

int n=0;

void setup(){
  Serial.begin(115200);

  delay(2000);
  



  delay(2000);
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

if(!SD.begin(SS)){
 // if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();




  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

//delay(2000);
// Serial.println("TIPO DE TARJETA...");
//  Serial.print("SD Card Type: ");
//  if(cardType == CARD_MMC){
//    Serial.println("MMC");
//  } else if(cardType == CARD_SD){
//    Serial.println("SDSC");  //ESTA
//  } else if(cardType == CARD_SDHC){
//    Serial.println("SDHC");
//  } else {
//    Serial.println("UNKNOWN");
//  }


//delay(2000);
// Serial.println("TAMAÑO TARJETA...");
//  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
//  Serial.printf("SD Card Size: %lluMB\n", cardSize);  //1910MB

//  listDir(SD, "/", 0);
//  delay(2000);

//deleteFile(SD, "/mydir/hello.txt");
//delay(2000);

//  removeDir(SD, "/mydir");
//  delay(2000);
  
  createDir(SD, "/mydir");
  delay(2000);

  
//  listDir(SD, "/", 0);
//  delay(2000);
  

  
//  listDir(SD, "/", 2);
//  delay(2000);


  
  writeFile(SD, "/mydir/datos_sensor_dht11.txt", "n, Temperatura, Humedad\n");
  delay(2000);

  
//  appendFile(SD, "/hello.txt", "raymundo World!\n");
//  delay(2000);
//  
//  appendFile(SD, "/hello.txt", "5\n");
//  delay(2000);

  
  
//  readFile(SD, "/hello.txt");
//  delay(2000);
  
//  deleteFile(SD, "/foo.txt");
//  delay(2000);
//  
//  renameFile(SD, "/hello.txt", "/foo.txt");
//  delay(2000);

  
//  readFile(SD, "/foo.txt");
//  delay(2000);

  

  
// Serial.println("ESPACIO LIBRE TARJETA..");
//  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
//  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
}

void loop(){
//nada que hacer


 // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
n = n+1;
appendFile(SD, "/mydir/datos_sensor_dht11.txt", (String(n) + "," + String(event.temperature) + ",").c_str());


  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }


  appendFile(SD, "/mydir/datos_sensor_dht11.txt", (String(event.relative_humidity)+ "\n").c_str());
  
 //appendFile(SD, "/mydir/datos_sensor_dht11.txt", (String(event.temperature) + "," +String(event.relative_humidity)+ "\n").c_str());
    
Serial.print(String(n)+"\n");
  
  delay(5000);


} 
