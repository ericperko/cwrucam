/* Arduino sketch to control the CWRUcam mount
 * Author: Eric Perko <eric.perko@case.edu>
 */

#include <Servo.h>
#include <Messenger.h>

Servo pan;
Servo tilt;
Messenger receiver;

const int pan_pin = 9;
const int tilt_pin = 10;

int pan_pos = 90;
int tilt_pos = 90;

int pan_delta = 0;
int tilt_delta = 0;

void executeCommand() {
  while(receiver.available()) {
    pan_delta = receiver.readInt();
    tilt_delta = receiver.readInt();
  }
  pan_pos += pan_delta;
  tilt_pos += tilt_delta;
}

void setup() {
  pan.attach(pan_pin);
  tilt.attach(tilt_pin);
  Serial.begin(115200);
  Serial.println("      CWRUcam Arduino Controller v0.5");
  Serial.println("Pin 9 is pan servo. Pin 10 is tilt servo");
  
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
  if (pan_pos < 5) {
    pan_pos = 5;
  } else if (pan_pos > 175) {
    pan_pos = 175;
  }
  if (tilt_pos < 0) {
    tilt_pos = 0;
  } else if (tilt_pos > 140) {
    tilt_pos = 140;
  }
  pan.write(pan_pos);
  tilt.write(tilt_pos);
}






