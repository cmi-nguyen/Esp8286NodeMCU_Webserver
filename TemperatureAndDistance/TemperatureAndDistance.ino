#include <I2S.h>

#include <SoftwareSerial.h>
 
const int US100_TX = 4;
const int US100_RX = 5;
 
SoftwareSerial US100Serial(US100_RX, US100_TX);
 
unsigned int MSByteDist = 0;
unsigned int LSByteDist = 0;
unsigned int mmDist = 0;
int temp = 0;
 
void setup() {
    Serial.begin(9600);
    US100Serial.begin(9600);
}
 
void loop() {
 
    US100Serial.flush();
    US100Serial.write(0x55); 
 
    delay(500);
 
    if(US100Serial.available() >= 2) 
    {
        MSByteDist = US100Serial.read(); 
        LSByteDist = US100Serial.read();
        mmDist  = MSByteDist * 256 + LSByteDist; 
        if((mmDist > 1) && (mmDist < 10000)) 
        {
            Serial.print("Distance: ");
            Serial.print(mmDist, DEC);
            Serial.println(" mm");
        }
    }
 
    US100Serial.flush(); 
    US100Serial.write(0x50); 
 
    delay(500);
    if(US100Serial.available() >= 1) 
    {
        temp = US100Serial.read();
        if((temp > 1) && (temp < 130)) // temprature is in range
        {
            temp -= 45; // correct 45º offset
            Serial.print("Temp: ");
            Serial.print(temp, DEC);
            Serial.println(" ºC.");
        }
    }
 
    delay(500);
}