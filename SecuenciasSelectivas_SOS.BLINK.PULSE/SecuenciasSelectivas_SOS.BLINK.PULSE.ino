#define BTN_SOS   4
#define BTN_BLINK 5
#define BTN_COLOR 6

#define LED_PIN RGB_BUILTIN   // LED RGB integrado del ESP32-S3

void setup() {
  Serial.begin(115200);
  pinMode(BTN_SOS, INPUT_PULLUP);
  pinMode(BTN_BLINK, INPUT_PULLUP);
  pinMode(BTN_COLOR, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  rgbLedWrite(LED_PIN, 0, 0, 0); // Apagar LED
}

void loop() {
  if (digitalRead(BTN_SOS) == LOW)   sosMorse();
  if (digitalRead(BTN_BLINK) == LOW) blink();
  if (digitalRead(BTN_COLOR) == LOW) colorPulses();
}


void sosMorse() {
  for (int i = 0; i < 3; i++) morseDot();
  delay(300);
  for (int i = 0; i < 3; i++) morseDash();
  delay(300);
  for (int i = 0; i < 3; i++) morseDot();
  delay(800);
  rgbLedWrite(LED_PIN, 0, 0, 0);
}

void morseDot() {
  rgbLedWrite(LED_PIN, 255, 255, 0);
  delay(200);
  rgbLedWrite(LED_PIN, 0, 0, 0);
  delay(200);
}

void morseDash() {
  rgbLedWrite(LED_PIN, 255, 255, 0);
  delay(600);
  rgbLedWrite(LED_PIN, 0, 0, 0);
  delay(200);
}


void blink() {
  for (int i = 0; i < 7; i++) {
    rgbLedWrite(LED_PIN, 255, 255, 255);
    delay(200);
    rgbLedWrite(LED_PIN, 0, 0, 0);
    delay(200);
  }
}


void colorPulses() {
  int colors[7][3] = {
    {255, 0, 0}, {255, 127, 0}, {255, 255, 0},
    {0, 255, 0}, {0, 0, 255}, {75, 0, 130}, {255, 255, 255}
  };
  for (int c = 0; c < 7; c++) {
    // Encendido gradual
    for (int b = 0; b <= 255; b += 5) {
      rgbLedWrite(LED_PIN,
                  colors[c][0] * b / 255,
                  colors[c][1] * b / 255,
                  colors[c][2] * b / 255);
      delay(15);
    }
    // Apagado gradual
    for (int b = 255; b >= 0; b -= 5) {
      rgbLedWrite(LED_PIN,
                  colors[c][0] * b / 255,
                  colors[c][1] * b / 255,
                  colors[c][2] * b / 255);
      delay(15);
    }
  }
  rgbLedWrite(LED_PIN, 0, 0, 0);
}
