#define LED_PIN RGB_BUILTIN   // LED RGB integrado
#define RGB_BRIGHTNESS 255

void setup() {
  pinMode(LED_PIN, OUTPUT);   // Activamos el pin del LED
}

void loop() {

  // S = tres puntos
  morseDot(); morseDot(); morseDot();
  delay(300); // pequeña pausa entre letras

  // O = tres rayas
  morseDash(); morseDash(); morseDash();
  delay(300); // pequeña pausa entre letras

  // S = tres puntos
  morseDot(); morseDot(); morseDot();

  delay(2000); // pausa larga antes de repetir todo

  blink(); blink(); blink(); blink(); blink(); blink(); blink();
  delay(300); // pequeña pausa entre letras

  delay(2000); // pausa larga antes de repetir todo

   morseDot(); morseDot(); morseDot();
  delay(300); // pequeña pausa entre letras

  // O = tres rayas
  morseDash(); morseDash(); morseDash();
  delay(300); // pequeña pausa entre letras

  // S = tres puntos
  morseDot(); morseDot(); morseDot();

  delay(2000); // pausa larga antes de repetir todo

  pulsacionesDeColores();
  delay(2000); // pausa antes de repetir todo

}

// Punto = LED encendido corto
void morseDot() {
  encenderLed();
  delay(200);
  apagarLed();
  delay(200);
}

void blink() {
  encenderLedBlink();
  delay(200);
  apagarLed();
  delay(200);
}

// Raya = LED encendido largo
void morseDash() {
  encenderLed();
  delay(600);
  apagarLed();
  delay(200);
}


// Encender y apagar el LED RGB
void encenderLed() {
  rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 105, 180); // Enciende el LED 
}

void apagarLed() {
  rgbLedWrite(RGB_BUILTIN, 0, 0, 0);   // Apaga el LED
}

void encenderLedBlink() {
  rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, RGB_BRIGHTNESS, 0);
}

void pulsacionesDeColores() {
  // Arreglo con 7 colores distintos 
  int colores[7][3] = {
    {255, 0, 0},     // Rojo
    {255, 127, 0},   // Naranja
    {255, 255, 0},   // Amarillo
    {0, 255, 0},     // Verde
    {0, 0, 255},     // Azul
    {75, 0, 130},    // Morado
    {255, 255, 255}  // Blanco
  };

  // Recorre los 7 colores
  for (int i = 0; i < 7; i++) {
    // Aumenta brillo
    for (int intensidad = 0; intensidad <= 255; intensidad += 5) {
      rgbLedWrite(RGB_BUILTIN, 
                  (colores[i][0] * intensidad) / 255, 
                  (colores[i][1] * intensidad) / 255, 
                  (colores[i][2] * intensidad) / 255);
      delay(10); // control de velocidad del fade
    }

    // Disminuye brillo
    for (int intensidad = 255; intensidad >= 0; intensidad -= 5) {
      rgbLedWrite(RGB_BUILTIN, 
                  (colores[i][0] * intensidad) / 255, 
                  (colores[i][1] * intensidad) / 255, 
                  (colores[i][2] * intensidad) / 255);
      delay(10);
    }

    delay(300); // pausa entre colores
  }

  // Apaga el LED 
  apagarLed();
}


