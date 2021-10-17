/*
 * Parte A del Trabajo Grupal - TEC
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

const int STBY =  13; // habilitación de motores (Standby)

const int PhotoSensorLeft = A0;  // pin del Arduino conectado al sensor óptico izquierdo
const int PhotoSensorRight = A1; // pin del Arduino conectado al sensor óptico derecho

// Constantes para los sensores ópticos
const int BLANCO = 0;
const int NEGRO = 1;

// Velocidad a establecer en las ruedas
int velocidad;

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
  
  // Configuración de pines para los sensores ópticos
  pinMode(PhotoSensorLeft, INPUT);
  pinMode(PhotoSensorRight, INPUT);

  // Velocidad por defecto
  velocidad = 120;
  
  // Establece la velocidad en las ruedas y prepara el movimiento
  analogWrite(PWMA, velocidad);
  analogWrite(PWMB, velocidad);
  digitalWrite(STBY, HIGH);
}

/*
 * Acción que se repite en bucle (detección de luz)
 */
void loop() {
  sensor();
}

/**
 * Modifica la trayectoria del robot en función de la luz percibida
 */
void sensor() {
	
  // Se toman los valores externos y se ajustan sus umbrales por histéresis
  int izq = blancoNegro(analogRead(PhotoSensorLeft));
  int der = blancoNegro(analogRead(PhotoSensorRight));
  
  // Si detecta blanco a la izquierda...
  if (izq == BLANCO) {
    if (der == BLANCO) {
	  // Si a la derecha también hay blanco, retrocede
      retroceder();
    } else if (der == NEGRO) {
	  // En caso contrario, se desplaza hacia la derecha
      derecha();
    }
  }
  else { // Si detecta negro a la izquierda...
    if (der == BLANCO) {
	  // Si a la izquierda hay blanco, se desplaza hacia la izquierda
      izquierda();
    } else {
	  // En caso contrario, se desplaza hacia adelante
      adelante();
    }
  }
}

/**
 * Devuelve blanco o negro en función de los valores leídos del exterior (con histéresis):
 * - BLANCO si el valor supera el valor de 450
 * - NEGRO si el valor está por debajo del anterior
 */
int blancoNegro(int valor) {
  if (valor > 450) {
    return BLANCO;
  }  
  else if (valor < 800) {
    return NEGRO;
  }
}

/**
 * Avanza hacia adelante
 */
void adelante() {
  // Se activa la rueda derecha para avanzar
  analogWrite(PWMA, velocidad);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);

  // Se activa la rueda izquierda para avanzar
  analogWrite(PWMB, velocidad);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

/**
 * Avanza hacia atrás
 */
void retroceder() {
  // Se activa la rueda derecha para avanzar hacia atrás
  analogWrite(PWMA, velocidad);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);

  // Se activa la rueda izquierda para avanzar hacia atrás
  analogWrite(PWMB, velocidad);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

/**
 * Avanza hacia la derecha
 */
void derecha() {
  // Establece las velocidades en las ruedas
  analogWrite(PWMA, velocidad);
  analogWrite(PWMB, velocidad);

  // Configura la rueda derecha para avanzar hacia atrás
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  
  // Configura la rueda izquierda para avanzar hacia adelante
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void izquierda() {
  // Establece las velocidades en las ruedas
  analogWrite(PWMB, velocidad);
  analogWrite(PWMA, velocidad);

  // Configura la rueda izquierda para avanzar hacia atrás
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
    
  // Configura la rueda derecha para avanzar hacia adelante
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
}

/**
 * Establece un valor de velocidad válido
 * a partir del valor pasado como parámetro
 */
void setVelocidad(int speed) {
  if (speed > 255) {
    velocidad = 255;  
  } else if (speed < 0) {
    velocidad = 0;  
  } else {
    velocidad = speed;
  }
}