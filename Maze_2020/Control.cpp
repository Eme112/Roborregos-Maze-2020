#include "Control.h"
#include <NewPing.h>

#define SONAR_NUM 6      // Number of sensors.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(4, 5, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(6, 7, MAX_DISTANCE), 
  NewPing(8, 9, MAX_DISTANCE), 
  NewPing(4, 5, MAX_DISTANCE), 
  NewPing(6, 7, MAX_DISTANCE), 
  NewPing(6, 7, MAX_DISTANCE)
};

LiquidCrystal_I2C lcd(0x27,20,4);

Control::Control(){
  bno = Adafruit_BNO055();
}

void Control::LCDCalibration() {
  lcd.init(); 
  lcd.backlight();
  lcd.print("Hola Mundo");
  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
  lcd.print(" Segundos");
  delay(kTimeToPrint);
}

void Control::BNOCalibration() {
  Serial.println("Orientation Sensor Test"); Serial.println("");   
  if(!bno.begin())
  {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(kTimeToPrintLCD); 
  
  bno.setExtCrystalUse(true);
  sensors_event_t event; 
  bno.getEvent(&event);
  
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  
  delay(kRepose);

  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);
 
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }
  
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.println(mag, DEC);
}

uint8_t Control::orientationStatus() {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  return mag;
}

void Control::calibrationAll() {
  bno.begin();
  bno.setExtCrystalUse(true);
  lcd.init();
  lcd.backlight();
  writeLCDdown("LCD ready");
  lcd.clear();
  lcd.setCursor(0,0);
  
  while(orientationStatus() != 3) {
    lcd.setCursor(6,0);
    lcd.print("I´m not ready");
  }
  lcd.setCursor(6,1);
  lcd.print("I´m ready");

  delay(kDelayAfterBNO);
  
  pinMode(kMotorLeftForward1, OUTPUT);
  pinMode(kMotorLeftForward2, OUTPUT);
  pinMode(kMotorLeftBack1, OUTPUT);
  pinMode(kMotorLeftBack2, OUTPUT);
  pinMode(kMotorRightForward1, OUTPUT);
  pinMode(kMotorRightForward2, OUTPUT);
  pinMode(kMotorRightBack1, OUTPUT);
  pinMode(kMotorRightBack2, OUTPUT);

  sensors_event_t event;
  bno.getEvent(&event);
}

double Control::getDifferenceWithZero() { 
  double error_generated = 0;
  
  double current_angle = getAngleBNOX();
  if (current_angle >= 180) {
    error_generated = (360 - current_angle);
  }
  
  else {
    error_generated = current_angle;
    error_generated = -(error_generated);
  }
  
  return error_generated;
}

double Control::getAngleBNOX() { 
  sensors_event_t event;
  bno.getEvent(&event);
  
  return event.orientation.x;
}

double Control::getAngleBNOY() { 
  sensors_event_t event;
  bno.getEvent(&event);
  
  return event.orientation.y;
}

double Control::getAngleBNOZ() { 
  sensors_event_t event;
  bno.getEvent(&event);
  
  return event.orientation.z;
}

double Control::getDesiredAngle(double desire) {
  double initial_difference = 0; 
  double current_angle = getAngleBNOX();
  
  if (current_angle > 180) {
    desire = desire - initial_difference;
    if (desire < 0) {
      desire += 360;
    }
    
    else{
      desire = desire;
    }
    
  }
  else {
    desire = desire - initial_difference;
  }
  
  return desire;
}

double Control::getError(const double current_angle, const double desire){
  double error = desire - current_angle;
  
  if (error < -180) {
     error += 360;
   }
    
  else if (error > 180) {
    error -= 360;
  }
  
  return error;
}

double Control::getErrorUltrasonic(const double current_distance, const double desire) {
  double error = current_distance - desire;
  return error;
}

void Control::getPwm(double &vel) { 
  if (vel < kLimit_inf_pwm) {
  vel = kLimit_inf_pwm;
  }
  
  if (vel > kLimit_sup_pwm) {
    vel = kLimit_sup_pwm;
  }
}

double Control::getDistanceFrontLeft() {
  delay(50);
  return sonar[0].ping_cm();
}

double Control::getDistanceFrontRight() {
  delay(50);
  return sonar[1].ping_cm();
}

double Control::getDistanceRightUp() {
  delay(50);
  return sonar[2].ping_cm();
}

double Control::getDistanceRightDown() {
  delay(50);
  return sonar[3].ping_cm();
}

double Control::getDistanceLeftUp() {
  delay(50);
  return sonar[4].ping_cm();
}

double Control::getDistanceLeftDown() {
  delay(50);
  return sonar[5].ping_cm(); 
}

bool Control::checkWallsRight() {
  double current_distance_right_up = getDistanceRightUp();
  double current_distance_right_down = getDistanceRightDown();
  if (current_distance_right_up < 15 and current_distance_right_down < 15) {
    return true;
  }
  else if (current_distance_right_up > 15 || current_distance_right_down > 15) {
    return false;
  }
}

bool Control::checkWallsLeft() {
  double current_distance_left_up = getDistanceLeftUp();
  double current_distance_left_down = getDistanceLeftDown();
  if (current_distance_left_up < 15 and current_distance_left_down < 15) {
    return true;
  }
  else if (current_distance_left_up > 15 || current_distance_left_down > 15) {
    return false;
  }
}

void Control::advance(const double desire, const double desireUltrasonic) {
  double encoder1 = 0;
  double encoder2 = 0;
  double pwm_left = 0;
  double pwm_right = 0;
  double final_error = 0;
  double current_distance_right_up = getDistanceRightUp();
  double current_distance_right_down = getDistanceRightDown();
  double current_distance_left_up = getDistanceLeftUp();
  double current_distance_left_down = getDistanceLeftDown();
  double error_right_up = getErrorUltrasonic(current_distance_right_up, desireUltrasonic);
  double error_right_down = getErrorUltrasonic(current_distance_right_down, desireUltrasonic);
  double error_left_up = getErrorUltrasonic(current_distance_left_up, desireUltrasonic);
  double error_left_down = getErrorUltrasonic(current_distance_left_down, desireUltrasonic);
  double current_angle = getAngleBNOX();
  double errorBNO = getError(current_angle, desire);

  do {
    double error_ultrasonic_right = error_right_up + error_right_down;
    double error_ultrasonic_left = error_left_up + error_left_down;
    if (error_ultrasonic_left > 0) {
      error_ultrasonic_left = -(error_ultrasonic_left);
    }
    else {
      error_ultrasonic_left = -(error_ultrasonic_left);
    }
    final_error = errorBNO + error_ultrasonic_right + error_ultrasonic_left;
    if (final_error > 0) {
    pwm_right = kLimit_inf_pwm;
    pwm_left = kP2 * final_error;
    getPwm(pwm_left);
  }
  
  else {
    pwm_left = kLimit_inf_pwm;
    pwm_right = kP2 * final_error;
    getPwm(pwm_right);
  }
  forwardPwm(pwm_right, pwm_left);
  }
  while (encoder1 < 500 and encoder2 < 500);
  }
  
void Control::turnLeft(const uint8_t vel) { 
    digitalWrite(kMotorLeftForward2, HIGH);
    analogWrite(kMotorLeftForward1, vel);
    digitalWrite(kMotorLeftBack1, HIGH);
    analogWrite(kMotorLeftBack2, vel);
    digitalWrite(kMotorRightForward2, HIGH);
    analogWrite(kMotorRightForward1, vel);
    digitalWrite(kMotorRightBack2, HIGH);
    analogWrite(kMotorRightBack1, vel);
}

void Control::turnRight(const uint8_t vel) { 
    digitalWrite(kMotorLeftForward2, LOW);
    analogWrite(kMotorLeftForward1, vel);
    digitalWrite(kMotorLeftBack1, LOW);
    analogWrite(kMotorLeftBack2, vel);
    digitalWrite(kMotorRightForward2, LOW);
    analogWrite(kMotorRightForward1, vel);
    digitalWrite(kMotorRightBack2, LOW);
    analogWrite(kMotorRightBack1, vel);  
}

void Control::turnDegrees(double desire) {
  double vel = 0;
  double error = 0;
  double current_angle = getAngleBNOX();
  desire = getDesiredAngle(desire);
  
  do{
    error = getError(current_angle, desire);
    Serial.println(error);
    vel = kP * error;
    
    getPwm(vel); // Verify to the pwm stay in the range.
    if (error < 0) { 
      turnLeft(vel);
    }   
    else {
      turnRight(vel);
    } 
  }
  while ( error < -(kRange_error) || error > kRange_error); // Asigne a range to stop the robot.
  stopEngines();
}

void Control::stopEngines() { 
    digitalWrite(kMotorLeftForward1, HIGH);
    digitalWrite(kMotorLeftForward2, HIGH);
    digitalWrite(kMotorLeftBack1, HIGH);
    digitalWrite(kMotorLeftBack2, HIGH);
    digitalWrite(kMotorRightForward1, HIGH);
    digitalWrite(kMotorRightForward2, HIGH);
    digitalWrite(kMotorRightBack1, HIGH);
    digitalWrite(kMotorRightBack2, HIGH);

    delay(kTimeToStop);

    digitalWrite(kMotorLeftForward1, LOW);
    digitalWrite(kMotorLeftForward2, LOW);
    digitalWrite(kMotorLeftBack1, LOW);
    digitalWrite(kMotorLeftBack2, LOW);
    digitalWrite(kMotorRightForward1, LOW);
    digitalWrite(kMotorRightForward2, LOW);
    digitalWrite(kMotorRightBack1, LOW);
    digitalWrite(kMotorRightBack2, LOW); 
}

void Control::fastForward() {
  forwardPwm(kPwm_max, kPwm_max);
}

void Control::forwardPwm(const uint8_t pwm_right, const uint8_t pwm_left) {
    digitalWrite(kMotorLeftForward2, LOW);
    analogWrite(kMotorLeftForward2, pwm_left);
    digitalWrite(kMotorLeftBack1, LOW);
    analogWrite(kMotorLeftBack2, pwm_left);
    digitalWrite(kMotorRightForward1, LOW);
    analogWrite(kMotorRightForward2, pwm_right);
    digitalWrite(kMotorRightBack1, LOW);
    analogWrite(kMotorRightBack2, pwm_right);
}

void Control::moveBack() {
    digitalWrite(kMotorLeftForward1, LOW);
    analogWrite(kMotorLeftForward2, kPwm_max);
    digitalWrite(kMotorLeftBack2, LOW);
    analogWrite(kMotorLeftBack1, kPwm_max);
    digitalWrite(kMotorRightForward2, LOW);
    analogWrite(kMotorRightForward1, kPwm_max);
    digitalWrite(kMotorRightBack2, LOW);
    analogWrite(kMotorRightBack1, kPwm_max);
}

void Control::writeNumLCD(const int num) {
  lcd.clear();
  lcd.print(num);
}

void Control::writeLyricsLCD(const char letra) {
  lcd.print(letra);
}

void Control::writeLCD(const String sE1, const String sE2) {
  lcd.clear();
  lcd.print(sE1);
  lcd.setCursor(0, 1);
  lcd.print(sE2);
}

void Control::writeLCDdown(const String sE1) {
  lcd.setCursor(0, 1);
  lcd.print(sE1);
}

void Control::printLocation(double x, double y, double z) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("X:");
  lcd.setCursor(2,0);
  lcd.print(x);

   lcd.setCursor(7,0);
  lcd.print("Y:");
  lcd.setCursor(9,0);
  lcd.print(y);

  lcd.setCursor(11,0);
  lcd.print("Z:");
  lcd.setCursor(13,0);
  lcd.print(z);

delay(kTimeSeeLocation);
}

void Control::setup() {
  dispenser.attach(42);
  dispenser.write(90);   // Initialize the dispenser in 90 degrees(in the middle).
}

void Control::drop_one_kit_left() {
  dispenser.write(dispenserDegrees_0);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_100);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_90);
  delay(kTime_2ms);
}

void Control::drop_one_kit_right() {
  dispenser.write(dispenserDegrees_180);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_80);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_90);
  delay(kTime_2ms);
}

void Control::drop_two_kits_right() {
  dispenser.write(dispenserDegrees_180);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_80);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_90);
  delay(kTime_2ms);
  dispenser.write(dispenserDegrees_180);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_80);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_90);
  delay(kTime_2ms);
}

void Control::drop_two_kits_left() {
  dispenser.write(dispenserDegrees_0);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_100);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_90);
  delay(kTime_2ms);
  dispenser.write(dispenserDegrees_0);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_100);
  delay(kTime_1sec);
  dispenser.write(dispenserDegrees_90);
  delay(kTime_2ms);
}

bool Control::heatVictim(){
  
}
bool Control::visualVictim(){
  
}
bool Control::colorVictim(){
  
}
