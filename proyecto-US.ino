#include "Mouse.h"
#include "Keyboard.h"
#include <SoftwareSerial.h>
#define RxD 10                             // Usado para conexion rs232 virtual.
#define TxD 11                             // Usado para conexion rs232 virtual.

//Define pines. 
const int joystickX = A1;            // joystick eje X   
const int joystickY = A0;            // joystick eje Y 
const int cambioEmulacion = 2;       // Cambia emulacion de teclas camara/teclado.

//Define variables.
int velocidadCursor = 15;            // Velocidad de movimientos ejes x e y.
int movimientoTeclas = 2;            //  valores recibidos serian 1 = disminuir eje,2 =  posicion reposo, 3 = subir eje.
int retardoRespuesta = 5;            // Retardo respuesta en ms.
int centro = velocidadCursor/2;                      // Para calcular centro del joystick cuando esta en reposo.
int tolerancia = velocidadCursor/4;  // Define tolerancia en el calculo centro.
int ultimoEstado = LOW;              // Ultimo estado del pulsador cambio camara/teclado. 
boolean  ratonActivo = false;        // Variable para control camara/teclado.
boolean teclaActiva = false;         // Variable para saber si hay tecla activa (pulsada).
boolean  teclaActivaA = false;
boolean  teclaActivaS = false;
boolean  teclaActivaD = false;
boolean  teclaActivaW = false;

SoftwareSerial comVirtual(RxD, TxD); 
 
void setup() {
  pinMode(cambioEmulacion, INPUT_PULLUP);   // Activa resistencia pullup en entrada.
  Mouse.begin();                            // Iniciamos emulacion raton.
  Keyboard.begin();                         // Iniciamos emulacion teclado.
  delay (500);
  comVirtual.flush();                  // Vacia el buffer de entrada.
  delay(500);
  comVirtual.begin(9600);             // Inicializa com virtual
  comVirtual.print("Iniializando salida datos ...");

    //pines para botones
  pinMode(1, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
    //pines para botones
}

void loop() {
 
  int estadoPulsador = digitalRead(cambioEmulacion); 
  
  if (estadoPulsador != ultimoEstado) {
    if (estadoPulsador == LOW) {
      ratonActivo = !ratonActivo;
    }
  }
  
  ultimoEstado = estadoPulsador;            // Guarda estado del interruptor para siguiente ciclo.
 
  // if the mouse control state is active, move the mouse:
  if (ratonActivo) {
    int leerX = leerEje(joystickX);
    int leerY = leerEje(joystickY);
    Mouse.move(leerX, leerY, 0); // (x, y, rueda de raton)
   }
  else {
   int leerX = leerTeclas(joystickX);
   int leerY = leerTeclas(joystickY);

          
    if (leerX<3) {
       Keyboard.press('a');
       teclaActivaA = true;
    }
     if (leerX > 2 && leerX < 6) {
      if (teclaActivaA == true) {
        Keyboard.release ('a');
        teclaActivaA = false;
      }
      else {

       Keyboard.release ('d');
       teclaActivaD = false;
      }
    }
     
     if (leerX>5) {
       Keyboard.press('d');
       teclaActivaD = true;
    }
    
    
     if (leerY<3) {
       Keyboard.press('w');
       teclaActivaW = true;
    }
     if (leerY > 2 && leerY < 6) {
      if (teclaActivaW == true) {
        Keyboard.release ('w');
        teclaActivaW = false;
      }
      else {

       Keyboard.release ('s');
       teclaActivaS = false;
      }
    }
     
     if (leerY > 5) {
       Keyboard.press('s');
       teclaActivaS = true;
    } 
   
   } 
  delay (retardoRespuesta);



    //botones
  if (digitalRead(1) == LOW){
    Serial.println("4");
    Keyboard.press('g');
    delay(300);
    Keyboard.releaseAll();
  }

  if (digitalRead(3) == LOW){
    Serial.println("5");
    Keyboard.press('h');
    delay(300);
    Keyboard.releaseAll();
  }

  if (digitalRead(4) == LOW){
    Serial.println("6");
    Keyboard.press('j');
    delay(300);
    Keyboard.releaseAll();
  }

  if (digitalRead(5) == LOW){
    Serial.println("1");
    Keyboard.press('t');
    delay(300);
    Keyboard.releaseAll();
  }

  if (digitalRead(6) == LOW){
    Serial.println("2");
    Keyboard.press('y');
    delay(300);
    Keyboard.releaseAll();
  }
    if (digitalRead(7) == LOW){
    Serial.println("3");
    Keyboard.press('u');
    delay(300);
    Keyboard.releaseAll();
  }

  if (digitalRead(8) == LOW){
    Serial.println("7");
    Keyboard.press('g');
    Keyboard.press('h');
    delay(300);
    Keyboard.releaseAll();
  }


  if (digitalRead(9) == LOW){
    Serial.println("8");

    delay(300);
  }
  //botones
}
  
/* subritina que va leer los datos analogicos de los ejes x e y
   y sacara el valor entre o y el rango definido.
 */

int leerEje(int eje) {
  
  int lectura = analogRead(eje);                   // leer valor analogico.

  // map the reading from the analog input range to the output range:
  lectura = map(lectura, 0, 1023, 0, velocidadCursor);
   int distancia = lectura - centro;
   if (abs(distancia) < tolerancia) {
    distancia = 0;
   }

  // return the distance for this axis:
 return distancia;
 
 }

int leerTeclas(int eje) {
  
  int lectura = analogRead(eje);                   // leer valor analogico.
  lectura = map(lectura, 0, 1023, 0, 10);
   delay(20);
   return lectura;
 }