/* Arduino sketch to control the CWRUcam mount
 * Author: Eric Perko <eric.perko@case.edu>
 */

#include <Servo.h>
#include <Messenger.h>

Servo pan;
Servo tilt;
Messenger receiver;

const int pan_pin = 13;
const int tilt_pin = 10;

int pan_pos = 90;
int tilt_pos = 90;

void executeCommand() {
  while(receiver.available()) {
    pan_pos = receiver.readInt();
    tilt_pos = receiver.readInt();
  }
  Serial.println(pan_pos);
  Serial.println(tilt_pos);
  Serial.println("Command received");
  pan.write(pan_pos);
  tilt.write(tilt_pos);
}

void setup() {
  pan.attach(pan_pin);
  tilt.attach(tilt_pin);
  Serial.begin(115200);
  Serial.println("      CWRUcam Arduino Controller v0.2");
  Serial.println("Pin 13 is pan servo for testing. Pin 10 is tilt servo");
  
  receiver.attach(executeCommand);
  
  // Initialization
  Serial.println("Centering mount...");
  pan.write(pan_pos);
  tilt.write(tilt_pos);
}

void loop() {
  while(Serial.available()) {
    receiver.process(Serial.read());
  }
}






