#include <Servo.h>              //Libreria para servomotor
#include <Wire.h>               //Libreria para comunicacion
#include <LiquidCrystal_I2C.h>  //Libreria para manejar pantalla LCD con modulo I2C
  // Set the LCD address to 0x27 for a 16 chars and 4 line display
  LiquidCrystal_I2C lcd(0x27, 16, 4);
Servo myservo;                 //Se crea un servo como objeto para trabajar la libreria
const int rele[2] = { 5, 4 };  //Bomba de agua|Agitador
const int Activador = 7;       //Boton para activar proceso
bool Estado = false;
const int tiempo_estimado = 50;  //Tiempo estimado de proceso
void setup() {
  myservo.attach(3);  // Se en laza el pin 2 correspodiente para el control del servo
    myservo.write(0);  // Se inicia en una posicion determinada
    // Se inicializa la LCD
    lcd.begin();
  // Se enciende la luz de fondo para la LCD
  lcd.backlight();
  for (int j = 0; j < 2; j++) {
    pinMode(rele[j], OUTPUT);
  }
  pinMode(Activador, INPUT_PULLUP);
  MotorInactivo();
  BombaInactiva();
  lcd.setCursor(5, 0);  //Mensajes de inicio de programa
  lcd.print("Eliot");
  lcd.setCursor(6, 1);
  lcd.print("Amelie");
  lcd.setCursor(4, 2);
  lcd.print("Sebastian");
  lcd.setCursor(5, 3);
  lcd.print("Equipo 6");
  delay(1500);
}
void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Esperando inicio.");  //Aparecera hasta que se presiones el boton activador
  delay(200);
  lcd.print(".");
  delay(200);
  lcd.print(".");
  delay(200);
  Estado = digitalRead(Activador);  //Se toma una lectura del estado del boton
  if (!Estado) {
    BombaActiva();  //Se enciende la bomba de agua
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Llenando");
    delay(15000);     //tiempo de llenado
    BombaInactiva();  //Se apaga la bomba de agua
    Dispensado();     //Se inicia el proceso de dispensado para lapastilla 
    for (int cuenta = tiempo_estimado; cuenta >= 0; cuenta--) {
      MotorActivo();  //Se activa el agitador
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Batiendo");
      lcd.setCursor(0, 1);
      lcd.print("Tiempo de trabajo:");
      lcd.print(cuenta);  //Tiempo que falta para quetermine el proceso de agitado
      lcd.print("s");
      delay(1000);
    }
    MotorInactivo();  //Se apaga el agitador

    lcd.clear();
    lcd.setCursor(6, 1);
    lcd.print("Proceso");
    lcd.setCursor(6, 2);
    lcd.print("Finalizado");
    delay(2000);
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("Puede");
    lcd.setCursor(6, 1);
    lcd.print("vaciar");
    lcd.setCursor(4, 2);
    lcd.print("el reactor");
    delay(2000);
  }
}
void Dispensado() {
  lcd.clear();
  for (int i = 0; i < 120; i++)  //Avanza hacia adelante el eje lineal
  {
    lcd.setCursor(0, 0);
    lcd.print("Dispensando");
    lcd.setCursor(0, 1);
    lcd.print("Pastilla");
    myservo.write(i);
    delay(10);
  }
  for (int i = 120; i >= 0; i--)  //Regresa a su posicion inicia
  {
    lcd.setCursor(0, 0);
    lcd.print("Pastilla");
    lcd.setCursor(0, 1);
    lcd.print("Dispensada");
    myservo.write(i);
    delay(10);  //este retraso hace que lo haga de manera mas controlada
  }
}
void BombaActiva()  //Funcion para activar bomba
{
  digitalWrite(rele[0], LOW);
}
void BombaInactiva()  //Funcion para desactivar bomba
{
  digitalWrite(rele[0], HIGH);
}
void MotorActivo()  //Funcion para activar motor del agitador
{
  digitalWrite(rele[1], LOW);
}
void MotorInactivo()  //Funcion para desactivar motor del agitador
{
  digitalWrite(rele[1], HIGH);
}
