#include <LiquidCrystal_I2C.h>
#include <max6675.h>
#include <Wire.h>

//Codigo para horno

//definicion de pines para el termopar
#define CSK 12
#define CS 11
#define SO 10

MAX6675 termopar(CSK, CS, SO);

//se define el tamaño del display
LiquidCrystal_I2C lcd(0x27, 20, 4);

float C = 0;                   //variable para almacenar la temperatura en grados celcius.
volatile bool boton = false;   //variable en la que se almacenara el cambio del final de carrera.
const int sensor = A0;         //definicion de pin de lectura del sensor.
float distancia = 0;           //variable para guardar la lectura del sensor.
bool objeto = false;           //variable que indicara si hay un objeto o no.
const int rele[2] = { 8, 9 };  //pines para usar un rele.
//Variables para proceso
unsigned long tdc = 0;
unsigned long tch = 2400000;      //tiempo que tarda la lasaña en calentar.
const int bos = 6;                //Buzzern que enciende al final del proceso.
const int swi = 2;                //Switch de encendido o apagado.
const int Fdc = 3;                //Final de carrera.
const int Leds[3] = { 4, 5, 7 };  //Leds R, B ,G (Precalentando, Proceso, Finalizado).
bool cocinando = false;
//pivotes para controles
const int pivote = 180;
const int pivote1 = 100;

void setup() {        //configuracion de Pines y comunicaciones
  lcd.begin();
  pinMode(rele[1], OUTPUT);
  pinMode(rele[0], OUTPUT);
  for (int i = 0; i < 3; i++) {
    pinMode(Leds[3], OUTPUT);
  }
  pinMode(bos, OUTPUT);
  pinMode(swi, INPUT_PULLUP);  
  pinMode(Fdc, INPUT_PULLUP);
}

void loop() {   //Loop principal
  Proceso();
  Temperatura();
}
void Temperatura() {  //Funcion para leer la temperatura
  C = termopar.readCelsius();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("Temperatura ");
  lcd.print(C);
  lcd.print("C");
  delay(1000);
  lcd.clear();
}

void ON_OFF() {
  if (cocinando) {
    if (C > pivote) {
      digitalWrite(rele[0], HIGH);  //ventilador
      digitalWrite(rele[1], LOW);   //resitencias
    } else if (C < pivote) {
      digitalWrite(rele[0], LOW);
      digitalWrite(rele[1], HIGH);
    }
  } else {

    if (C > pivote1) {
      digitalWrite(rele[0], LOW);  //ventilador
      digitalWrite(rele[1], LOW);  //resitencias
    } else if (C < pivote1) {
      digitalWrite(rele[0], LOW);
      digitalWrite(rele[1], LOW);
    }
  }
}
void Proceso() {
  if (digitalRead(swi) == 1 && digitalRead(Fdc) == 1) {
    tdc = millis();
    if (tdc >= tch) {
      Finalizado();
      lcd.setCursor(0, 0);
      lcd.print("Lasaña lista :D");
      digitalWrite(rele[2], HIGH);
      PGreen();
      Melodia();
    } else {  //Proceso de funcionamiento
      cocinando = true;
      lcd.setCursor(0, 0);
      lcd.print("Cocinando");
      lcd.setCursor(0, 3);
      lcd.print("Tiempo faltante:");
      lcd.print(tch - tdc / 1000);
      lcd.print("s");
      LedB();
      ON_OFF();
    }
  } else if (digitalRead(swi) == 0 && digitalRead(Fdc) == 1) {  //Proceso de precalentado
    lcd.setCursor(0, 0);
    lcd.print("Precalentando");
    ON_OFF();
    LedR();
    tdc = 0;
    cocinando = false;
  } else if (digitalRead(swi) == 0 && digitalRead(Fdc) == 0) {  //Horno apagado
    lcd.setCursor(0, 0);
    lcd.print("Bienvenido");
  }
}

void PGreen() {  //Funcion para hacer parpadear led verde
  digitalWrite(Leds[2], HIGH);
  delay(500);
  digitalWrite(Leds[2], LOW);
  delay(500);
  digitalWrite(rele[1], LOW);
  digitalWrite(rele[0], LOW);
}

void LedB() {  //Funcion para encender led blanco
  digitalWrite(rele[1], HIGH);
  digitalWrite(rele[0], LOW);
  digitalWrite(rele[2], LOW);
}

void LedR() {  //Funcion para encender led rojo
  digitalWrite(rele[0], HIGH);
  digitalWrite(rele[1], LOW);
  digitalWrite(rele[2], LOW);
}

void Melodia() {  //Tono de buzzer
  analogWrite(bos, 100);
  analogWrite(bos, 150);
  analogWrite(bos, 50);
  analogWrite(bos, 0);
}

void Finalizado() {
  digitalWrite(rele[0], HIGH);  //ventilador
  digitalWrite(rele[1], LOW);   //resitencias
}