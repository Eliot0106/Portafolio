// Este codigo es para un maze resolving robot
#include "Adafruit_VL53L0X.h"  //Libreria para utilizar el sensor laser de distancia Vl53L0X.

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

const int LEDR = 8;   //Led rojo que se encarga de indicar si el sensor laser esta fuera de rango.
const int LEDW = 7;   //Led blanco que indica que el sensor se encuenta midiendo en su rango (2mm - 2000mm).
const int LEDB = 12;  //Led azul que indica que el robot esta encendido y funcionando.
const int LEDY = 9;   //Led amarillo que indica que el robot llego a la meta.

//Pines que se encargan de controlar el sentido de giro de los motores.
const int motor1[] = { 3, 2 };  //Motor Izquierdo.
const int motor2[] = { 5, 4 };  //Motor Derecho.

const int velM1 = 10;  //Pines encargados de controlar la velocidad de los motores.
const int velM2 = 11;

int speedM1 = 43;  //Velocidad motor izquierdo.
int speedM2 = 46;  //velocidad motor derecho.
int speedTurnM1 = 60; //Velocidad unicamente para los giros.
int speedTurnM2 = 60; //Velocidad unicamente para los giros.

//Estos valores se deberan recalibrar dependiendo de la carga de las baterias.
const int TimeIzq = 850;
const int TimeDer = 850;
const int TimeIzqEje = 300;
const int TimeDerEje = 300;
const int Time180 = 600;  //Tiempo para dar una vuelta de 180°.
const int TimeDeath = 50;

//Sensores analogicos (CNY70).
const int SensorDerecho = A0;    //Sensor del lado derecho.
const int SensorIzquierdo = A1;  //Sensor del lado izquierdo
const int SensorSuelo = A2;      //Sensor viendo al suelo.

const int on_off = 6;        //Pin del push button.
bool Iniciar = false;  //Se le asigna un valor falso al inicio ya que el verdadero sera hasta que se presione el push button.
int count = 0;
int pasado = 0;

int Distans_Front = 0;  //Se declara como variable golbal poara poder utilizarla en todos lados.

const int LimiteFrontal = 50;  //distancia minima de deteccion de pared frontal en mm.
const int LimiteLejano = 500;  //distancia maxima de deteccion de pared frontal en mm.

const int LimiteUmbralDerecho = 5;   //Limites de deteccion inicales para una pared.
const int LimiteUmbralIzquierdo = 5;

//Limites maximos de deteccion de una pared, apartir de
//superar este limite se debera realizar una correccion en la direccion del robot, solo hasta que ya no se detecte el limite.
const int LimiteUmbralDerechoCritico = 100;
const int LimiteUmbralIzquierdoCritico = 100;


void CriticoDerecha() {  //Funcion que corrige la posicion del robot si se encuentra mucho a la derecha.
  if (analogRead(SensorDerecho) >= LimiteUmbralDerechoCritico) {
    analogWrite(velM1, speedM1);
    analogWrite(velM2, speedM2);
    digitalWrite(motor1[1], LOW);
    digitalWrite(motor1[0], HIGH);
    digitalWrite(motor2[1], HIGH);
    digitalWrite(motor2[0], LOW);
    delay(TimeDeath);
  }
}

void CriticoIzquierda() {  //Funcion que corrige la posicion del robot si se encuentra mucho a la izquierda.
  if (analogRead(SensorIzquierdo) >= LimiteUmbralIzquierdoCritico) {
    analogWrite(velM1, speedM1);
    analogWrite(velM2, speedM2);
    digitalWrite(motor1[1], HIGH);
    digitalWrite(motor1[0], LOW);
    digitalWrite(motor2[1], LOW);
    digitalWrite(motor2[0], HIGH);
    delay(TimeDeath);
  }
}

void adelante() {  //Para que vaya hacia adelante
  analogWrite(velM1, speedM1);
  analogWrite(velM2, speedM2);
  digitalWrite(motor1[1], HIGH);
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor2[1], HIGH);
  digitalWrite(motor2[0], LOW);
}

void derechaEje() {
  analogWrite(velM1, speedTurnM1);
  analogWrite(velM2, speedTurnM2);
  digitalWrite(motor1[1], HIGH);
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor2[1], LOW);
  digitalWrite(motor2[0], HIGH);
  delay(TimeDerEje);
}

void izquierdaEje() {
  analogWrite(velM1, speedTurnM1);
  analogWrite(velM2, speedTurnM2);
  digitalWrite(motor1[1], LOW);
  digitalWrite(motor1[0], HIGH);
  digitalWrite(motor2[1], HIGH);
  digitalWrite(motor2[0], LOW);
  delay(TimeIzqEje);
}

void derecha() {
  analogWrite(velM1, speedTurnM1);
  analogWrite(velM2, speedTurnM2);
  digitalWrite(motor1[1], HIGH);
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor2[1], LOW);
  digitalWrite(motor2[0], LOW);
  delay(TimeDer);
}

void izquierda() {
  analogWrite(velM1, speedTurnM1);
  analogWrite(velM2, speedTurnM2);
  digitalWrite(motor1[1], LOW);
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor2[1], HIGH);
  digitalWrite(motor2[0], LOW);
  delay(TimeIzq);
}

void OFF() {
  analogWrite(velM1, 0);
  analogWrite(velM2, 0);
  digitalWrite(motor1[1], LOW);
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor2[1], LOW);
  digitalWrite(motor2[0], LOW);
}

void Giro180() {
  analogWrite(velM1, speedTurnM1);
  analogWrite(velM2, speedTurnM2);
  digitalWrite(motor1[1], HIGH);
  digitalWrite(motor1[0], LOW);
  digitalWrite(motor2[0], HIGH);
  digitalWrite(motor2[1], LOW);
  delay(Time180);
}

bool PuedeAvanzar() {  //Funcion que indica si el robot puede avanzar o no.
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    int distancia = measure.RangeMilliMeter;

    if (distancia > LimiteFrontal && distancia < LimiteLejano) {  //Se pone un intervalo para que el umbral de la distancia sea el correcto para saber si el robot
                                                                  //Puede avanzar o no.
      digitalWrite(LEDW, HIGH);                                   //LED que indica que la distancia esta en un rango aceptable para avanzar
      digitalWrite(LEDR, LOW);
      return true;
    } else {
      digitalWrite(LEDR, HIGH);  //LED que indica cuando el robot esta muy cerca de una pared o la pared esta demaciado lejos para ser detectada.
      digitalWrite(LEDW, LOW);
      return false;
    }
  } else {  //Este solo en caso de que supere el limite de medicion del sensor(VL53L0x) de 2000mm o 2m.
    return false;
  }
}

bool On_Off() {                        //Lee el estado del push button que activa el robot.
  int presente = digitalRead(on_off);  //Estos if se usan para evitar un efecto de rebote en el push botton,
  if (pasado == 1 && presente == 0) {
    count++;
    if (count == 1) {
      return true;
    } else if (count > 1 || count <= 0) {
      count = 0;
      return false;
    }
  }
  pasado = presente;
  return false;
}

bool LimiteDerecho() {  //Encargada de detectar paredes del lado derecho.
  return (analogRead(SensorDerecho) >= LimiteUmbralDerecho && analogRead(SensorDerecho) >= LimiteUmbralDerechoCritico);
}

bool LimiteIzquierdo() {  //Encargada de detectar paredes del lado izquierdo.
  return (analogRead(SensorIzquierdo) >= LimiteUmbralIzquierdo && analogRead(SensorIzquierdo) >= LimiteUmbralIzquierdoCritico);
}

bool Camino() {  //Encargada de detectar la meta o el camino.
  return (analogRead(SensorSuelo) >= 100);
}

void setup() {  //Se declara la funcion para los pines.
  for (int i = 0; i < 2; i++) {
    pinMode(motor1[i], OUTPUT);
    pinMode(motor2[i], OUTPUT);
  }
  pinMode(on_off, INPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDW, OUTPUT);

  pinMode(velM2, OUTPUT);
  pinMode(velM1, OUTPUT);

  Serial.begin(9600);

  if (!lox.begin()) {
    digitalWrite(LEDR, HIGH);
    while (1)
      ;
  }
}

void loop() {
  Iniciar = On_Off();     //Valor que indica si iniciar con el recorido o no.
  bool Suelo = Camino();  //Checa el suelo.

  if (Iniciar && !Suelo) {
    adelante();                                    //se le da la orde al robot de avanzar hacia adelante
    encenderRobot();                               //enciende un lED azul que indica que el robot esta en funcionamiento.
    if (PuedeAvanzar()) {                          //Se hace el cuestionamiento de si hay una pared justo en frente de el robot.
      if (LimiteDerecho() && LimiteIzquierdo()) {  //Si la respuesta fue falsa, se procede a preguntar si hay paredes a ambos lados del robot.
                                                   //En coso de ser verdadera se avanzara hacia adelante.
        adelante();
        CriticoDerecha();
        CriticoIzquierda();
        Serial.println("Pared frontal lejos, dos paredes a los lados.");

      } else if (!LimiteDerecho() && LimiteIzquierdo()) {  //Caso en donde no hay pared a la derecha pero si a la izquierda, y la pared de en frente se encuentra alejada.
        derecha();                                         //Gira 90° a la derecha.
        Serial.println("Pared frontal lejos, pared solo en lado izquierdo.");

      } else if (LimiteDerecho() && !LimiteIzquierdo()) {  //Caso donde no hay pared a la izquierda pero si a la izquierda, y la pared de en frente se encuentra alejada.
        izquierda();                                       //Gira 90° a la izquieda.
        Serial.println("Pared frontal lejos, pared solo en lado derecho.");

      }else if (!LimiteDerecho() && !LimiteIzquierdo()) {  //Este caso es especial ya que es unicamente cuando el robot detecta que la pared frontal esta lejos de su rango critico pero no detecta paredes en sus costados.
        adelante();
        CriticoDerecha();
        CriticoIzquierda();                      
        Serial.println("Pared frontal lejos, no se detectan paredes a los lados.");        

      }
    } else {                                       //Hay una pared justo en frente de robot.
      if (LimiteDerecho() && LimiteIzquierdo()) {  //Caso donde  hay una pared en ambos lados y una enfrente.
        Giro180();                                 //Se gira 180°m todos los giros los da "sobre su eje".
        Serial.println("Pared frontal cerca, dos paredes a los lados.");

      } else if (!LimiteDerecho() && LimiteIzquierdo()) {  //Caso donde hay una pared en el lado izquierdo pero en el derecho no y tambien una pared en frente.
        derechaEje();                                         // Se gira a la derecha, 90°.
        Serial.println("Pared frontal cerca, pared solo a la izquierda.");

      } else if (LimiteDerecho() && !LimiteIzquierdo()) {   //Caso donde hay una pared a la derecha pero no a la izquierda y una pared en frente tambien.
        izquierdaEje();                                        //Se gira a la izquierda, 90°
        Serial.println("Pared frontal cerca, pared solo a la derecha.");

      } else if (!LimiteDerecho() && !LimiteIzquierdo()) {  //Caso donde solo hay una pared de frente y se toma como prioridad el lado izquierdo para ir.
        derecha();
                                             //Se gira a la izquierda, 90°
        Serial.println("Pared frontal cerca, no se detectan paredes a los costados, es un cuse T.");

      }
    }

  } else if (Iniciar && Suelo) {  //Condicion que indica cuando se llega a la meta.
    OFF();
    apagarRobot();
    Parpadeo();
  } else if (!Iniciar && !Suelo) {  //Todo apagado.
    OFF();
    apagarRobot();
  }
}

void Parpadeo() {  //Funcion que indica cuando el robot llega a su meta.
  digitalWrite(LEDY, HIGH);
  delay(1000);
  digitalWrite(LEDY, LOW);
  delay(1000);
}

void encenderRobot() {  //Indica que el robot esta en funcionamiento.
  digitalWrite(LEDB, HIGH);
}

void apagarRobot() {  //Indica que el robot esta apagado.
  digitalWrite(LEDB, LOW);
}