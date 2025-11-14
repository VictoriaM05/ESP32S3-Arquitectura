
#define LED_PIN RGB_BUILTIN   // Utiliza LED RGB integrado
#define RGB_BRIGHTNESS 255

void setup() {
  pinMode(LED_PIN, OUTPUT);   // Activar el pin del LED
}

void loop() {

  // S = tres puntos
  morseDot(); morseDot(); morseDot();
  delay(300); 

  // O = tres rayas
  morseDash(); morseDash(); morseDash();
  delay(300); 

  // S = tres puntos
  morseDot(); morseDot(); morseDot();

  delay(2000); // pausa larga antes de repetir todo
}

// Punto (encendido corto)
void morseDot() {
  encenderLed();
  delay(200);
  apagarLed();
  delay(200);
}

// Raya (encendido largo)
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
