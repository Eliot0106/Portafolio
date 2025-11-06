#include "Adafruit_VL53L0X.h"
// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
// set the pins to shutdown
#define SHT_LOX1 1
#define SHT_LOX2 3
// objetos para los vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
const int Sdu = A3;
const int Sdd = A2;
const int Sid = A1;
bool Siu_estado = false;
bool Sdd_estado = false;
bool Sid_estado = false;
const int MI[2] = { 6, 7 };
const int MD[2] = { 8, 9 };
const int limitD = 800;
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 2; i++) {
    pinMode(MI[i], OUTPUT);
    pinMode(MD[i], OUTPUT);
  }
  while (!Serial) { delay(1); }
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  setID();
}
void loop() {
  if (!Siu_estado && !Sdd_estado && !Sid_estado) {
    Sen();
    //cuando el robot esta en el area de juego
  } else if (Siu_estado && !Sdd_estado && !Sid_estado) {
    CRight();
    //Cuando el sensor de arriba a la izquierda esta detectando el limite.
  } else if (!Siu_estado && Sdd_estado || Sid_estado) {
    CLeft();
    //Cuando el sensor de abajo a la derecha y el de la izquierda estan
    detectando el limite.
  } else if (Siu_estado && Sdd_estado && Sid_estado) {
    Moff();
  }
}
void Sensores() {
  if (analogRead(Sdu) > 125) {
    Siu_estado = true;
  } else if (analogRead(Sdu) > 125) {
    Sdd_estado = true;
  } else if (analogRead(Sdu) > 125) {
    Sid_estado = true;
  } else {
    Siu_estado = false;
    Sdd_estado = false;
    Sid_estado = false;
  }
}
void Moff() {
  digitalWrite(MI[0], LOW);
  digitalWrite(MI[0], LOW);
  digitalWrite(MD[0], LOW);
  digitalWrite(MD[0], LOW);
}
void CRight() {
  digitalWrite(MI[0], LOW);
  digitalWrite(MI[1], HIGH);
  digitalWrite(MD[0], LOW);
  digitalWrite(MD[1], HIGH);
}
void CLeft() {
  digitalWrite(MI[1], LOW);
  digitalWrite(MI[0], HIGH);
  digitalWrite(MD[1], LOW);
  digitalWrite(MD[0], HIGH);
}
void Mon() {
  digitalWrite(MI[0], LOW);
  digitalWrite(MI[1], HIGH);
  digitalWrite(MD[1], LOW);
  digitalWrite(MD[0], HIGH);
}
void Sen() {
  lox1.rangingTest(&measure1, false);                 // pass in 'true' to get debug
  data printout !lox2.rangingTest(&measure2, false);  // pass in 'true' to get debug
  data printout if (measure1.RangeMilliMeter <= limitD && measure2.RangeMilliMeter <= limitD) {
    Mon();
  }
  else if (measure1.RangeMilliMeter > limitD && measure2.RangeMilliMeter <= limitD) {
    CRight();
  }
  else if (measure1.RangeMilliMeter > limitD && measure2.RangeMilliMeter > limitD) {
    CLeft();
  }
  else if (measure2.RangeMilliMeter > limitD && measure1.RangeMilliMeter <= limitD) {
    CLeft();
  }
}
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);
  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1)
      ;
  }
  delay(10);
  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);
  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1)
      ;
  }
}
