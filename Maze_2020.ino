#include "Control.h"
Control robot;

void setup() {
  robot.orientationStatus();
  robot.CalibrationAll();
}

void loop() {
  robot.giroGrados(robot.E);
}
