#ifndef CONTROL_H
#define CONTROL_H
/* Roborregos 2020.
 * This control class it was made for the purpose to 
 * have a better control of the robot, it is function 
 * is to move the robot with precision
 * also calibrates all robot sensors.
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "arduino.h"
#include <Servo.h>

class Control {
  
  public:
    Control();
    // Initialize LCD.
    void LCDCalibration();
    // Initialize BNO.
    void BNOCalibration();
    // Return the Orientation Status of the BNO, the numbers to return are 1, 2, and 3, if the number returned is not 3, it means that it´s not calibrated.
    uint8_t orientationStatus();
    // Initialize all the sensors.
    void calibrationAll();

    // Get the difference of the current angle with the 0, and return a new error.
    double getDifferenceWithZero();
    // Get the current angle, return the event.orientation.x through a function.
    double getAngleBNOX();
    // Get the current angle, return the event.orientation.Y through a function.
    double getAngleBNOY();
    // Get the current angle, return the event.orientation.Z through a function.
    double getAngleBNOZ();
    // Get the new desired angle respect to the difference achieved.
    double getDesiredAngle(double desire);
    // Get the difference with the desired angle to the current angle, return error.
    double getError(const double angulo_actual, const double desire);
    // Get the difference with the desired distance to the current distance, return error.
    double getErrorUltrasonic(const double current_distance, const double desire); 
    // Verify if the pwm is in the range (kLimit_inf_pwm - kLimit_sup_pwm).
    void getPwm(double &vel);
    // Get the Ultrasonic Distance Right Up.
    double getDistanceRightUp();
    // Get the Ultrasonic Distance Right Down.
    double getDistanceRightDown();
    // Get the Ultrasonic Distance Left Up.
    double getDistanceLeftUp();
    // Get the Ultrasonic Distance Left Down.
    double getDistanceLeftDown();
    // Get the Ultrasonic Distance Front Left.
    double getDistanceFrontLeft();
    // Get the Ultrasonic Distance Front Right.
    double getDistanceFrontRight();
    // Check if the right wall is complete.
    bool checkWallsRight();
    // Check if the left wall is complete
    bool checkWallsLeft();
    
    // Move the robot to the forward.   
    void advance(const double desire, const double desireUltrasonic);
    // Turn the robot to 90 degrees to the left.
    void turnLeft(const uint8_t vel);
    // Turn the robot to 90 degrees to the right.
    void turnRight(const uint8_t vel);
    // Turn left or right depending on the desired angle
    void turnDegrees(double desire);
    // Stop the robot.
    void stopEngines();
    // Go fast to the ramp.
    void fastForward();
    // Move the robot to the back.
    void moveBack();
    // Move the robot forward through the pwm asigned in each side.
    void forwardPwm(const uint8_t pwm_right, const uint8_t pwm_left);

    // Write a number.      
    void writeNumLCD(const int num);
    // Write a letter.
    void writeLyricsLCD(const char letra);
    // Write a sentence.
    void writeLCD(const String sE1,const String sE2);
    // Write a sentence down.
    void writeLCDdown(const String sE1); 
    // Print the location of the robot.
    void printLocation(double x, double y, double z);
    
    // Initialize the dispenser.
    void setup();
    // Drop one kit for Heated Victims and Coloured Victims(red and yellow).
    void drop_one_kit_right();
    // Drop one kit for Heated Victims and Coloured Victims(red and yellow).
    void drop_one_kit_left();
    // Drop two kits for Visual Victims(Stable).
    void drop_two_kits_right();
    // Drop two kits for Visual Victims(Stable).
    void drop_two_kits_left();
    
    bool heatVictim();
    bool visualVictim();
    bool colorVictim();
    
    
    const uint8_t kMotorLeftForward1 = 9;  
    const uint8_t kMotorLeftForward2 = 8;  
    const uint8_t kMotorLeftBack1 = 10;
    const uint8_t kMotorLeftBack2 = 11;
    const uint8_t kMotorRightForward1 = 5;
    const uint8_t kMotorRightForward2 = 4;
    const uint8_t kMotorRightBack1 = 7;
    const uint8_t kMotorRightBack2 = 6;  

    const uint8_t kPwm_max = 255;
    const uint8_t kLimit_sup_pwm = 255; 
    const uint8_t kLimit_inf_pwm = 145;

    // Turns.
    const double kP = 1.07; 
    const double kI = 2.91; 
    const double kD = 3.33;

    // Advance.
    const double kP2 = 4.52; 
    const double kI2 = 3.45; 
    const double kD2 = 2.05;

    const uint8_t kRange_error = 2;

    const uint8_t N = 0;
    const uint8_t E = 90;
    const uint8_t S = 180;
    const int W = 270;
    
    const uint8_t dispenserDegrees_0 = 0;
    const uint8_t dispenserDegrees_80 = 80;
    const uint8_t dispenserDegrees_90 = 90;
    const uint8_t dispenserDegrees_100 = 100;
    const uint8_t dispenserDegrees_180 = 180;

    const uint8_t kRepose = 250;
    const int kDelayAfterBNO = 2700;
    const uint8_t kTimeToStop = 10;
    const uint8_t kTimeToPrint = 100;
    const int kTimeToPrintLCD = 1000;
    const int kTime_1sec = 1000;
    const uint8_t kTime_2ms = 200;
    const uint8_t kTimeSeeLocation = 100;

  private:
     Adafruit_BNO055 bno; 
     Servo dispenser; 
};
#endif
