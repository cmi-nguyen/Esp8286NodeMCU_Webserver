#include "LittleFS.h"
 
void setup() {
  Serial.begin(115200);
  
  if(LittleFS.begin()==true){
    Serial.println("ok");
    
  }
  
  File file = LittleFS.open("/test_example.txt.txt", "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("File Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
 
void loop() {

}