#include <i2cmaster.h>


int deviceAddress = 0x50<<1;    // En la hoja técnica del MLX906114, 0x5A es 
                                // la dirección de comunicación I²C por defecto.
                                // Corra la dirección 1 bit a la derecha, la
                                // librería I²Cmaster solo necesita los 7 bits
                                // mas significativos para la dirección.

float celcius = 0;              // Variable que contiene la temperatura en Celcius.
float fahrenheit = 0;           // Variable que contiene la temperatura en Fahrenheit.

void setup() {
  Serial.begin(9600);           // Inicia la comunicación serial a 9600bps.

  i2c_init();                               // Inicia el bus i2c.
  PORTC = (1 << PORTC4) | (1 << PORTC5);    // Habilita ‘pullups’.
}

void loop() {
  celcius = temperatureCelcius(deviceAddress);  // Lee los datos del MLX90614
                                                // con la dirección dada,
                                                // los transforma en la
                                                // temperatura en Celcius y
                                                // la almacena en la variable
                                                // celcius.
  
  fahrenheit = (celcius*1.8) + 32;     // Convierte Celcius en Fahrenheit 
                                       // y almacena en la variable Fahrenheit.

  Serial.print("Celcius: ");           // Imprime ambas lecturas en el
  Serial.println(celcius);             // puerto serial.
  Serial.print("Fahrenheit: ");
  Serial.println(fahrenheit);
  Serial.println();

  delay(1000);                         // Espera un segundo antes de imprimir de nuevo.
}

float temperatureCelcius(int address) {
  int dev = address;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;

  // Escribe
  i2c_start_wait(dev+I2C_WRITE);
  i2c_write(0x07);

  // Lee
  i2c_rep_start(dev+I2C_READ);
  data_low = i2c_readAck();       // Lee 1 byte y envía ack.
  data_high = i2c_readAck();      // Lee 1 byte y envía ack
  pec = i2c_readNak();
  i2c_stop();

  // Esto convierte los bytes altos y bajos juntos y procesa la temperatura.
  double tempFactor = 0.02;       // 0.02 grados por LSB (medida de
                                  // resolución del MLX90614).
  double tempData = 0x0000;       
  int frac;                       // Datos después del punto decimal.

  // Esto oculta el error del byte alto y lo mueve a la izquierda
  // 8 bits y agrega el byte bajo.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor)-0.01;
  float celcius = tempData - 273.15;
  
  // Retorna la temperatura en Celcius.
  return celcius;
}
