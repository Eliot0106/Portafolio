//Arduino Levid
#include <VarSpeedServo.h>    //Libreria con la cual controlamos a los servomotores y su velocidad.
#include <Wire.h>             //Libreria para la comunicacion con el maestro, por medio de comunicacion I2C

//Declaracion de los nombres para los servomotores.
VarSpeedServo base;    //amarillo.    
VarSpeedServo hombro;  //rojo.
VarSpeedServo codo;    //verde.
VarSpeedServo hand;    //negro.

/*En este espacio se declararan las variables necesarias para la obtencion de los angulos del robot por medio de cinematica inversa*/
float longs[3] = {8,9,15.5}; //Valores de longitudes entre ejes de rotacion(cm).
float qn[3] = {0,0,0};       //Valores de angulos de los ejes de rotacion.
float Pn[3] = {12,0,1};      //Valores de posicion del efector final X,Y,Z.
/////////////////////////////////////////////////////////////////
const int V[2] ={50,50};  //Valor de la velocidad de los motores
float se,h,a1,a2,a3,z1,co,y1,O = 0;  //variables globales.

const int botonPin = 13; // Pin conectado al botón

bool estadoBoton;        // Variable para almacenar el estado del botón
bool ultimoEstadoBoton;  // Variable para almacenar el último estado del botón
bool botonPresionado = false; // Variable para controlar el estado del botón
/////////
const int rele = 11; //Pin que activara un relevador 

//Pines para comunicacion I2C
const int datos = 6;
byte Auxiliar[datos];
byte Coordenadas[datos/2];
byte ArregloAuxiliar[datos/2];
float valor1Recibido,valor2Recibido = 0;

void setup() {
  Wire.begin(1); // Configuramos el esclavo con dirección 1
  Wire.onReceive(receiveEvent); // Configuramos la función para manejar eventos de recepción
  
  //asignacion de los pines para los servomotores.
  base.attach(5);  
  hombro.attach(6);
  codo.attach(9);
  hand.attach(10); 

  //inicializacion de comunicacion serial.
  Serial.begin(9600);

  //configuracion de pines
  pinMode(botonPin, INPUT_PULLUP);
  pinMode(rele, OUTPUT);
  //Acciones que se ejecutaran una unica vez.
  digitalWrite(rele, HIGH);
  Posicion_inicialC();
}

void loop() {
   estadoBoton = digitalRead(botonPin);  //Lectura del estado del boton

  // Comprueba si el botón ha sido presionado
  if (estadoBoton != ultimoEstadoBoton && estadoBoton == HIGH){
    Serial.println("Posicion de trabajo.");
    if(Coordenadas[0] != ArregloAuxiliar[0] && Coordenadas[1] != ArregloAuxiliar[1] && Coordenadas[2] > 4){
      Serial.println("Trabajando.");
      Pn[1] = 22- (valor1Recibido/10);
      Pn[0] = 41 - (valor2Recibido/10);
      Serial.println(valor1Recibido);
      Serial.println(valor2Recibido);
      cinematica_inversa(longs,Pn,qn);
      Angulos();
      Serial.println(Pn[0]);
      Serial.println(Pn[1]);
      Posicion_IK();
      delay(1500);
      Posicion_trabajo();
      delay(1500);
      Posicion_final();
      imprimirCoordenadas();
      for(int i = 0; i < 3; i++){
        Coordenadas[i] = ArregloAuxiliar[i];
      }
    } 
  }
  else{
    Serial.println("Posicion de descanso.");
  }
  // Almacena el último estado del botón
  ultimoEstadoBoton = estadoBoton;
}

//Funcion de cinematica inversa.
void cinematica_inversa(float longs[], float Pn[], float qn[]){ //datos que se ingresan a la funcion para que pueda realizar los calculos(longitudes, posicion del efector final y arreglo de los angulos para guardarlos).
  for(int k = 0;k<3;k++){
    if (!isnan(qn[k])){
       //calculos para el primer angulo.
       qn[0] = degrees(atan2(Pn[0],Pn[1]));  //Valor para el angulo de rotacion de la base. 

       //calculos para el segundo angulo del robot.
       z1 = Pn[2] - longs[0];                    //representa la altura relativa del efector final con respecto a la segunda articulacion.
       y1 = sqrt(pow(Pn[0],2) + pow(Pn[1],2));   //representa la distancia relativa en Y ya que es mas facil realizar los calculos en dos dimenciones.
       h = sqrt(pow(z1,2) + pow(y1,2));          //representa la distancia que existe entre el efector final y la 2da articulacion.

       a1 = degrees(atan2(z1,y1));                                                          //angulo que se genera entre la altura y la distancia relativas del efector final con respecto a la segunda articulacion.
       a2 = degrees(acos((pow(longs[1],2) + pow(h,2) - pow(longs[2],2))/(2*longs[1]*h)));   //angulo generado entre la recta h y la segunda longitud.
       O = a1 + a2;                                                                         //angulo en el que se encuentra en sentido de las manesillas del reloj.
       qn[1] = 180 - O;                                                                     //angulo para el servomotor, esto dado por la construccion del robot.

       //calculos para el tercer angulo del robot.
       a3 = degrees(acos((pow(longs[1],2) + pow(longs[2],2) - pow(h,2))/(2*longs[1]*longs[2])));   //angulo que se crea entre la segunda longitud y la tercera.
       qn[2] = 180 - a3;                                                                           //angulo para el servo motor igual dado por construccion del robot. 
    }
  } 
}                                                                       
  

void Posicion_IK(){                   //Posicion deseada por medio de cinematica inversa
  base.write(qn[0],V[1],false);
  hombro.write(qn[1],V[1],false);
  codo.write(qn[2],V[1],false);
  hand.write(90,V[0],false);
  digitalWrite(rele,LOW);      //Activacion del rele para recoger el objeto;
  delay(1500);  //Se da una pausa de un segundo y medio.
}

//Funcion que contiene la posicion de descanso del robot.
void Posicion_inicialC(){          
  hand.write(90,V[0],true);
  base.write(0,V[1],true);
  hombro.write(0,V[1],true);
  codo.write(180,V[1],true);
}

//Funcion para una posicion final para todos los objetos.
void Posicion_final(){
  base.write(0,V[1],true);
  hombro.write(120,V[1],true);
  codo.write(130,V[1],true);
  hand.write(150,V[0],true);
  digitalWrite(rele,HIGH);   //apaga el relevador despues de haber llegado.
  delay(500);                //Se da medio segundo de descanso al programa.
}

//Funcion para una posicion de trabajo.
void Posicion_trabajo(){
  base.write(45,V[1],true);
  hombro.write(90,V[1],true);
  codo.write(90,V[1],true);
  hand.write(180,V[0],true);
}

//Funcion para visualizar los angulos obtenidos mediante la cinematica inversas.
void Angulos(){                                      
  Serial.print("Angulo de primera articulacion: ");
  Serial.println(qn[0]);
  Serial.print("Angulo de segunda articulacion: ");
  Serial.println(qn[1]);
  Serial.print("Angulo de tercera articulacion: ");
  Serial.println(qn[2]);
  delay(1500);
}

void receiveEvent(int howMany) {
  if (howMany >= sizeof(Auxiliar)) { // Verificamos si se recibieron suficientes datos
    for (int i = 0; i < datos; i++) {
      Auxiliar[i] = Wire.read(); // Leemos los datos como bytes
    }
    for (int i = 0; i < 6; i++) {
      if (Auxiliar[i] != 0){
        Coordenadas[i/2]= Auxiliar[i];
      } 
    }
    Coordenadas[0] = 45 - Coordenadas[0];
    if(Coordenadas[2] == 7){
      Coordenadas[1] = -1 * Coordenadas[1];
      Coordenadas[2] = 6;
    }
  }
}


void imprimirCoordenadas(){
  Serial.print("Coordenadas: ");
  for(int i = 0; i < 3; i++){
    Serial.print(Coordenadas[i]);
    Serial.print(" ");
  }
  Serial.println();
}

