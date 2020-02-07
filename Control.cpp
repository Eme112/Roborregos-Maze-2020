#include "Control.h"

//giros
bool goIn = true; 
double error = 0;
double error_generado = 0; //Diferencia del Angulo Actual al 0
double kp = 1.07; //Valores prueba y error
double ki = 2.91; 
double kd = 3.33;
float P = 0;
double vel = 0; //pwm
double N = 0;
double E = 90;
double S = 180;
double W = 270;
double diferencia_inicial = 0; // = getDiferencia()
const int kLimit_sup_pwm = 255; //No se exceda del pwm limit
int kLimit_inf_pwm = 145;

bool LCDStart = true;
bool BNOStart = true;

LiquidCrystal_I2C lcd(0x27,20,4);

Control::Control(){
  bno = Adafruit_BNO055();
}

void Control::LCDCalibration()
{
  if(LCDStart)
  {
    //Inicializar el LCD
    lcd.init();
    
    //Encender la luz de fondo.
    lcd.backlight();
    
    //Escribimos el Mensaje en el LCD.
    lcd.print("Hola Mundo");

    LCDStart = false;
  }
  
  //Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
  //Escribimos el número de segundos trascurridos
  lcd.print(millis()/1000);
  lcd.print(" Segundos");
  delay(100);
}

void Control::BNOCalibration()
{
  if(BNOStart)
  {
    Serial.begin(9600);
    Serial.println("Orientation Sensor Test"); Serial.println("");
    
    if(!bno.begin())
    {
      Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }
    
    delay(1000);
      
    bno.setExtCrystalUse(true);

    BNOStart = false;
  }

  sensors_event_t event; 
  bno.getEvent(&event);
  
  Serial.print("X: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);
  Serial.println("");
  
  delay(250);

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

void Control::giroIzq(const uint8_t vel) { //Giro Izquierda con parametros de pwm asignados 
     digitalWrite(kMotorIzqAde2, HIGH);
    analogWrite(kMotorIzqAde1, vel);
    digitalWrite(kMotorIzqAtras1, HIGH);
    analogWrite(kMotorIzqAtras2, vel);
    digitalWrite(kMotorDerAde2, HIGH);
    analogWrite(kMotorDerAde1, vel);
    digitalWrite(kMotorDerAtras2, HIGH);
    analogWrite(kMotorDerAtras1, vel);
}

void Control::giroDer(const uint8_t vel) { //Giro Izquierda con parametros de pwm asignados 
    digitalWrite(kMotorIzqAde2, LOW);
    analogWrite(kMotorIzqAde1, vel);
    digitalWrite(kMotorIzqAtras1, LOW);
    analogWrite(kMotorIzqAtras2, vel);
    digitalWrite(kMotorDerAde2, LOW);
    analogWrite(kMotorDerAde1, vel);
    digitalWrite(kMotorDerAtras2, LOW);
    analogWrite(kMotorDerAtras1, vel);  
}

void Control::parar() { 
      digitalWrite(kMotorIzqAde1, HIGH);
      digitalWrite(kMotorIzqAde2, HIGH);
      digitalWrite(kMotorIzqAtras1, HIGH);
      digitalWrite(kMotorIzqAtras2, HIGH);
      digitalWrite(kMotorDerAde1, HIGH);
      digitalWrite(kMotorDerAde2, HIGH);
      digitalWrite(kMotorDerAtras1, HIGH);
      digitalWrite(kMotorDerAtras2, HIGH);

      delay(10);

      digitalWrite(kMotorIzqAde1, LOW);
      digitalWrite(kMotorIzqAde2, LOW);
      digitalWrite(kMotorIzqAtras1, LOW);
      digitalWrite(kMotorIzqAtras2, LOW);
      digitalWrite(kMotorDerAde1, LOW);
      digitalWrite(kMotorDerAde2, LOW);
      digitalWrite(kMotorDerAtras1, LOW);
      digitalWrite(kMotorDerAtras2, LOW); 
}

double Control::getDiferencia(){  //Diferencia del angulo Actual respecto al 0
  sensors_event_t event;
  bno.getEvent(&event);
  double angulo_actual = 0;
  angulo_actual = event.orientation.x;
  if (angulo_actual != 0){
    if (angulo_actual >= 180){
      error_generado = (360 - angulo_actual);
    }
    else {
      error_generado = angulo_actual;
      error_generado = -(error_generado);
    }
  }
  return error_generado;
}

double Control::getAnguloBNOX(){ 
  sensors_event_t event;
  bno.getEvent(&event);
  
  return event.orientation.x;
}

double Control::getAnguloDeseado(double desire){ //Respecto a la diferencia, nuevo angulo deseado
  double angulo_actual = 0;
  angulo_actual = getAnguloBNOX();
  if (angulo_actual > 180){
    desire = desire - diferencia_inicial;
    if (desire < 0){
      desire += 360;
    }
    else{
      desire = desire;
    }
  }
  else{
    desire = desire - diferencia_inicial;
  }
  return desire;
}

double Control::getError(double angulo_actual, double desire){ //Error del angulo actual al SetPoint deseado
  error = desire - angulo_actual;
  if (error < -180){
     error += 360;
   }
    
  else if (error > 180){
    error -= 360;
  }
  return error;
}

void Control::getPwm(double &vel){ 
    if (vel < kLimit_inf_pwm){
    vel = kLimit_inf_pwm;
  }
    
  if (vel > kLimit_sup_pwm){
    vel = kLimit_sup_pwm;
  }
}

void Control::giroGrados(double desire){ 
  
  double angulo_actual = 0;
  desire = getAnguloDeseado(desire); //Asignar el nuevo angulo deseado
  
    do{
    angulo_actual = getAnguloBNOX(); //Asignar el angulo actual del BNO055
    
    error = getError(angulo_actual, desire); //Asginar el error respecto al nuevo angulo deseado
    Serial.println(error);
    P = kp * error;
    vel = P;
    
    getPwm(vel); //Verificar que el Pwm este en el rango   
    if (error < 0){ //Mientras no se cumpla el while que siga girando
      
      giroIzq(vel);
    }   
    
    else {
      giroDer(vel);
    } 
  }
  while ( error < -2 || error > 2); //Asignar un rango ya que no todo es muy preciso
    parar();
}

void Control::escribirNumLCD(int num)
{
  lcd.clear();
  lcd.print(num);
}

void Control::escribirLetraLCD(char letra)
{
 // lcd.clear();
  lcd.print(letra);
}

void Control::escribirLCDabajo(String sE1) {
  lcd.setCursor(0, 1);
  lcd.print(sE1);
}

void Control::escribirLCD(String sE1, String sE2) {
  lcd.clear();
  lcd.print(sE1);
  lcd.setCursor(0, 1);
  lcd.print(sE2);
}

void Control::CalibrationAll(){

bno.begin();
bno.setExtCrystalUse(true);

lcd.init();
lcd.backlight();

escribirLCDabajo("LCD lista");
lcd.clear();
lcd.setCursor(0,0);

/*pinMode(pin1, INPUT); 
pinMode(pin2, INPUT);
pinMode(pin3, INPUT);
pinMode(pin4, INPUT);
pinMode(pin5, INPUT);
pinMode(pin6, INPUT);*/

while(orientationStatus() != 3)
  {
    lcd.setCursor(6,0);
    lcd.print("NCal");
  }
    lcd.setCursor(6,1);
    lcd.print("SCal");


  delay(2700);
  

  pinMode(kMotorIzqAde1, OUTPUT);
  pinMode(kMotorIzqAde2, OUTPUT);
  pinMode(kMotorIzqAtras1, OUTPUT);
  pinMode(kMotorIzqAtras2, OUTPUT);
  pinMode(kMotorDerAde1, OUTPUT);
  pinMode(kMotorDerAde2, OUTPUT);
  pinMode(kMotorDerAtras1, OUTPUT);
  pinMode(kMotorDerAtras2, OUTPUT);

  sensors_event_t event;
  bno.getEvent(&event);
}

int Control::orientationStatus()
{
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  /* Display calibration status for each sensor. */
  uint8_t system, gyro, accel, mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  return mag;
}
