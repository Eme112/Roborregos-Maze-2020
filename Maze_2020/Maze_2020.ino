#include "Control.h"
Control robot;

void setup() {
  Serial.begin(9600);
  robot.orientationStatus();
  robot.calibrationAll();
}

void loop() {
  robot.turnDegrees(robot.E);
}
