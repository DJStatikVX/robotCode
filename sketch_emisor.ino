/*
 * Programa para el emisor XBEE, que transmite una trama con la posición del joystick para los ejes X e Y
 * Autores: Eloy Alfredo Schmidt Rodríguez (UO271588) y Samuel Rodríguez Ares (UO271612)
*/

// Formato de la trama
struct __attribute__((packed)) frame_t {
  byte Data_0;
  byte Data_1;
};

// Declaración de la trama a emitir
frame_t frame;

/**
 * Configuración de pines de entrada para el joystick
 */
void setup() {
    pinMode(A0, INPUT);		// componente VRx del mando
    pinMode(A1, INPUT);		// componente VRy del mando
	
    Serial.begin(9600);   // conexión con el puerto serie 
}

/**
 * Acción a repetir con la emisión de trama
 */
void loop() {
    // Se leen los valores analógicos del joystick, se convierten y se introducen en cada byte de la trama
    frame.Data_0  = (byte) (analogRead(A0) / 4);
    frame.Data_1  = (byte) (analogRead(A1) / 4);
    
    // Transmisión de la trama a través del puerto serie
    Serial.write((const byte *)&frame, sizeof(frame_t));  
    Serial.flush();

    // Lapso de espera hasta la siguiente emisión
    delay(25);
}