#ifndef CONTROL_H
#define CONTROL_H

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "arduino.h"

class Control{
  public:
  
    Control();
    Adafruit_BNO055 bno;
    
    void LCDCalibration();
    void BNOCalibration();
    int orientationStatus();
    void CalibrationAll();

    double getDiferencia();
    double getAnguloBNOX();
    double getAnguloDeseado(double);
    double getError(double, double);
    void getPwm(double&);
    
    
    void avanzar(double);
    void giroIzq(const uint8_t);
    void giroDer(const uint8_t);
    void giroGrados(double);
    void parar();

    const int kMotorIzqAde1  = 9;  
    const int kMotorIzqAde2  = 8;  
    const int kMotorIzqAtras1  = 10;
    const int kMotorIzqAtras2  = 11;
    const int kMotorDerAde1  = 5;
    const int kMotorDerAde2  = 4;
    const int kMotorDerAtras1  = 7;
    const int kMotorDerAtras2  = 6;  

    const double N = 0;
    const double E = 90;
    const double S = 180;
    const double W = 270;

    void escribirNumLCD(int);
    void escribirLetraLCD(char);
    void escribirLCD(String,String);
    void escribirLCDabajo(String);
    
};

#endif
