#define BTN_SOS    4
#define BTN_BLINK  5
#define BTN_COLOR  6
#define BTN_STOP   7  

#define LED_PIN RGB_BUILTIN   // LED RGB integrado en la ESP32-S3
bool stopPressed = false;  // Variable para saber si se presionó STOP

void setup() {
  pinMode(BTN_SOS, INPUT_PULLUP);
  pinMode(BTN_BLINK, INPUT_PULLUP);
  pinMode(BTN_COLOR, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  rgbLedWrite(LED_PIN, 0, 0, 0); // Apagar LED
}

void loop() {
  checkStop(); // Revisa si STOP fue presionado

  if (digitalRead(BTN_SOS) == LOW)   sosMorse();
  if (digitalRead(BTN_BLINK) == LOW) blink();
  if (digitalRead(BTN_COLOR) == LOW) colorPulses();
}

// ---------- FUNCIÓN STOP ----------
void checkStop() {
  if (digitalRead(BTN_STOP) == LOW) {
    stopPressed = true;
    rgbLedWrite(LED_PIN, 0, 0, 0);
  } else {
    stopPressed = false;
  }
}

// Delay que se puede interrumpir con STOP
void delayWithStop(int ms) {
  int steps = ms / 10;
  for (int i = 0; i < steps; i++) {
    checkStop();
    if (stopPressed) return;
    delay(10);
  }
}

// ---------- SOS ----------
void sosMorse() {
  stopPressed = false;

  for (int i = 0; i < 3 && !stopPressed; i++) morseDot();
  delayWithStop(300);
  for (int i = 0; i < 3 && !stopPressed; i++) morseDash();
  delayWithStop(300);
  for (int i = 0; i < 3 && !stopPressed; i++) morseDot();
  delayWithStop(800);
  rgbLedWrite(LED_PIN, 0, 0, 0);
}

void morseDot() {
  if (stopPressed) return;
  rgbLedWrite(LED_PIN, 255, 255, 0);
  delayWithStop(200);
  rgbLedWrite(LED_PIN, 0, 0, 0);
  delayWithStop(200);
}

void morseDash() {
  if (stopPressed) return;
  rgbLedWrite(LED_PIN, 255, 255, 0);
  delayWithStop(600);
  rgbLedWrite(LED_PIN, 0, 0, 0);
  delayWithStop(200);
}

// ---------- 7 BLINKS ----------
void blink() {
  stopPressed = false;

  for (int i = 0; i < 7 && !stopPressed; i++) {
    rgbLedWrite(LED_PIN, 255, 255, 255);
    delayWithStop(200);
    rgbLedWrite(LED_PIN, 0, 0, 0);
    delayWithStop(200);
  }
  rgbLedWrite(LED_PIN, 0, 0, 0);
}

// ---------- 7 PULSOS DE COLORES ----------
void colorPulses() {
  stopPressed = false;

  int colors[7][3] = {
    {255, 0, 0}, {255, 127, 0}, {255, 255, 0},  
    {0, 255, 0}, {0, 0, 255}, {75, 0, 130}, {255, 255, 255}
  };

  for (int c = 0; c < 7 && !stopPressed; c++) {
    // Encendido gradual
    for (int b = 0; b <= 255 && !stopPressed; b += 5) {
      rgbLedWrite(LED_PIN,
                  colors[c][0] * b / 255,
                  colors[c][1] * b / 255,
                  
                  colors[c][2] * b / 255);
      delayWithStop(15);
    }
    // Apagado gradual
    for (int b = 255; b >= 0 && !stopPressed; b -= 5) {
      rgbLedWrite(LED_PIN,
                  colors[c][0] * b / 255,
                  colors[c][1] * b / 255,
                  colors[c][2] * b / 255);
      delayWithStop(15);
    }
  }
  rgbLedWrite(LED_PIN, 0, 0, 0);
}

