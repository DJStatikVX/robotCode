/*
 * Parte B del Trabajo Grupal - TEC
 * Autores: Eloy Alfredo Schmidt Rodríguez (UO271588) y Samuel Rodríguez Ares (UO271612)
 */

#include <SoftwareSerial.h>

// Pines de control del motor derecho
const int AIN1 =  2;
const int AIN2 =  3;

// Pines de control del motor izquierdo
const int BIN1 =  4;
const int BIN2 =  5;

const int PWMA =  9;  // velocidad del motor derecho   (PWM)
const int PWMB =  10; // velocidad del motor izquierdo (PWM)

const int STBY =  13; // habilitacion de motores (Standby)

// Velocidad a establecer en las ruedas
int velocidad;

// Estructura de la trama
struct __attribute__((packed)) frame_t {
  byte AX;
  byte AY;
};

// Declaración de la trama a recibir
frame_t frame;

// Configuración del puerto serie para recibir la trama
SoftwareSerial mySerial(12, 11);

/*
 * Configuración de pines y parámetros antes de ejecutar
*/
void setup() {
	
  // Configuración de pines para las ruedas y sus velocidades
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Establece la velocidad en las ruedas y prepara el movimiento
  digitalWrite(STBY, HIGH);
  velocidad = 120;

  // Inicializa la conexión serie
  mySerial.begin(9600);  //     pin 12 para la recepción XBEE
  Serial.begin(9600);    //     UART para depuración (USB)
}

/*
 * Acción que se repite en bucle (control teledirigido)
 */
void loop() {
  control();  
}

/**
 * Establece el movimiento de las ruedas
 * en función de la trama XBEE recibida
 */
void control() {
	
  // Se recibe y se guardan los bytes de la trama cuando está disponible
  if (mySerial.available() > 1) {
    // read the incoming bytes:                                                                                                                                                                                                                              
    frame.AX  = mySerial.read();    
    frame.AY = mySerial.read();
  }
  
  /*
   * MOVIMIENTOS COMPUESTOS
   */

  // Caso 1: No hay movimiento en el joystick. El robot no se mueve
  if (frame.AY > 120 && frame.AY < 135 && frame.AX > 110 && frame.AX < 140) {
	  
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
	
  // Caso 2: El joystick se desplaza hacia arriba a la izquierda
  } else if (frame.AY > 135 && frame.AX > 140) {
	  
    analogWrite(PWMA, ((frame.AX - 140.0) / 115.0) * 130.0);
    analogWrite(PWMB, ((frame.AY - 135.0) / 120.0) * 180.0);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
	
  // Caso 3: El joystick se desplaza hacia arriba a la derecha
  } else if (frame.AY < 120 && frame.AX > 140) {
  
    analogWrite(PWMA, ((120.0 - frame.AY) / 120.0) * 180.0);
    analogWrite(PWMB, ((frame.AX - 140.0) / 115.0) * 130.0);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
	
  // Caso 4: El joystick se desplaza hacia atrás a la izquierda
  } else if (frame.AY > 135 && frame.AX < 110) {
  
    analogWrite(PWMA, ((110.0 - frame.AX) / 110.0) * 130.0);
    analogWrite(PWMB, ((frame.AY - 135.0) / 120.0) * 180.0);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
	
  // Caso 5: El joystick se desplaza hacia atrás a la derecha
  } else if (frame.AY < 120 && frame.AX < 110) {
  
    analogWrite(PWMA, ((110.0 - frame.AX) / 110.0) * 130.0);
    analogWrite(PWMB, ((120.0 - frame.AY) / 120.0) * 180.0);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
	
  /**
   * MOVIMIENTOS SIMPLES
   */
	
  // Caso 6: El joystick se desplaza hacia la izquierda
  } else if (frame.AY > 135) {
  
    analogWrite(PWMA, 0);
    analogWrite(PWMB, ((frame.AY - 135.0) / 120.0) * 180.0);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
	
  // Caso 7: El joystick se desplaza hacia la derecha
  } else if (frame.AY < 120) {
  
    analogWrite(PWMA, ((120.0 - frame.AY) / 120.0) * 180.0);
    analogWrite(PWMB,0);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
	
  // Caso 8: El joystick se desplaza hacia adelante
  } else if(frame.AX > 140) {
  
    analogWrite(PWMA, ((frame.AX - 140.0) / 115.0) * 255.0);
    analogWrite(PWMB, ((frame.AX - 140.0) / 115.0) * 255.0);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
	
  // Caso 9: El joystick se desplaza hacia atrás
  } else if (frame.AX < 110) {
  
    analogWrite(PWMA, ((110.0 - frame.AX) / 110.0) * 255.0);
    analogWrite(PWMB, ((110.0 - frame.AX) / 110.0) * 255.0);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
	
  }
}

/**
 * Establece un valor de velocidad válido
 * a partir del valor pasado como parámetro
 */
void setVelocidad(int speed){
  if (speed > 255) {
    velocidad = 255;  
  } else if (speed < 0) {
    velocidad = 0;  
  } else {
    velocidad = speed;
  }
}