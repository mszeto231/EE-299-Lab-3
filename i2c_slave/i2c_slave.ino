// Player 1 will be using the master
// Player 2 will be using the slave

#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(4);
  Wire.onReceive();

}

void loop() {
  

}
