/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-servo-motor
 */

#include <Servo.h>

Servo servo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup() {
  servo.attach(D4);
  Serial.begin(9600);  // attaches the servo on pin D7 to the servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // rotate from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);  
                 // tell servo to go to position in variable 'pos'
    delay(10);                          // waits 10ms for the servo to reach the position
  }

  for (pos = 180; pos >= 0; pos -= 1) { // rotate from 180 degrees to 0 degrees
    servo.write(pos);   
                // tell servo to go to position in variable 'pos'
    delay(10);                          // waits 10ms for the servo to reach the position
  }
}
