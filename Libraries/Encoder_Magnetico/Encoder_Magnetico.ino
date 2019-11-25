int interruptPinA = 2;
volatile long contador = 0;
int CANAL_A = 2;
int CANAL_B= 3;

void setup() {
  pinMode(CANAL_A, INPUT);
  pinMode(CANAL_B,INPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(interruptPinA), IRS,RISING);
}

void loop() {
  Serial.println(contador);
  delay(100);
}

void IRS(){
  if (digitalRead(CANAL_A) == HIGH) {
    if (digitalRead(CANAL_B) == LOW) {
      contador++;
    }
    else {
      contador--;
    }
  }
}
