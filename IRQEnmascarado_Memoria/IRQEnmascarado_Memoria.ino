#define BTN_SOS    4
#define BTN_BLINK  5
#define BTN_COLOR  6
#define BTN_STOP   7

#define LED_PIN    RGB_BUILTIN

#define DOT_DURATION    200
#define DASH_DURATION   600
#define SYMBOL_SPACE    200
#define LETTER_SPACE    600

const char* morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

enum TaskType { NONE, SOS, BLINK, COLOR };

struct TaskState {
  TaskType type;
  int step;
  String msg;
};

#define MAX_TASKS 5
TaskState taskQueue[MAX_TASKS];
int queueStart = 0;
int queueEnd = 0;

TaskState currentTask = { NONE, 0, "" };
volatile bool stopPressed = false;

// ------------------------
// FUNCIONES BÁSICAS
// ------------------------
void IRAM_ATTR stopISR() {
  stopPressed = true;
}

void clearLED() {
  rgbLedWrite(LED_PIN, 0, 0, 0);
}

bool safeDelay(int totalMs) {
  int waited = 0;
  const int slice = 10;
  while (waited < totalMs) {
    if (stopPressed || digitalRead(BTN_SOS) == LOW ||
        digitalRead(BTN_BLINK) == LOW || digitalRead(BTN_COLOR) == LOW)
      return false;
    delay(slice);
    waited += slice;
  }
  return true;
}

bool blinkColor(int r, int g, int b, int duration) {
  rgbLedWrite(LED_PIN, r, g, b);
  if (!safeDelay(duration)) {
    clearLED();
    return false;
  }
  clearLED();
  return safeDelay(SYMBOL_SPACE);
}

// ------------------------
// COLA DE TAREAS
// ------------------------
bool enqueueTask(TaskState task) {
  int next = (queueEnd + 1) % MAX_TASKS;
  if (next == queueStart) return false;
  taskQueue[queueEnd] = task;
  queueEnd = next;
  return true;
}

bool dequeueTask(TaskState &task) {
  if (queueStart == queueEnd) return false;
  task = taskQueue[queueStart];
  queueStart = (queueStart + 1) % MAX_TASKS;
  return true;
}

bool queueEmpty() {
  return (queueStart == queueEnd);
}

// ------------------------
// TAREAS
// ------------------------
bool runSOS(TaskState &task) {
  String msg = "SOS";
  for (int i = task.step; i < msg.length(); i++) {
    if (stopPressed) {
      task.step = i;
      return false;
    }
    char c = toupper(msg[i]);
    if (c >= 'A' && c <= 'Z') {
      String morse = morseCode[c - 'A'];
      for (int j = 0; j < morse.length(); j++) {
        if (!blinkColor(255, 255, 0, morse[j] == '.' ? DOT_DURATION : DASH_DURATION)) {
          task.step = i;
          return false;
        }
      }
      if (!safeDelay(LETTER_SPACE - SYMBOL_SPACE)) {
        task.step = i;
        return false;
      }
    }
  }
  return true;
}

bool runBlink(TaskState &task) {
  for (int i = task.step; i < 7; i++) {
    if (!blinkColor(255, 255, 255, 200)) {
      task.step = i;
      return false;
    }
    if (!safeDelay(200)) {
      task.step = i;
      return false;
    }
  }
  return true;
}

bool runColor(TaskState &task) {
  int colors[7][3] = {
    {255, 0, 0}, {255, 127, 0}, {255, 255, 0},
    {0, 255, 0}, {0, 0, 255}, {75, 0, 130}, {255, 255, 255}
  };

  for (int t = task.step; t < 7; t++) {
    if (stopPressed) {
      task.step = t;
      return false;
    }
    int r = colors[t][0], g = colors[t][1], b = colors[t][2];
    for (int brightness = 0; brightness <= 255; brightness += 5) {
      if (stopPressed) {
        task.step = t;
        return false;
      }
      rgbLedWrite(LED_PIN, (r * brightness) / 255,
                  (g * brightness) / 255,
                  (b * brightness) / 255);
      if (!safeDelay(15)) {
        task.step = t;
        return false;
      }
    }
    clearLED();
    if (!safeDelay(200)) {
      task.step = t;
      return false;
    }
  }
  return true;
}

// ------------------------
// SETUP Y LOOP
// ------------------------
void setup() {
  pinMode(BTN_SOS, INPUT_PULLUP);
  pinMode(BTN_BLINK, INPUT_PULLUP);
  pinMode(BTN_COLOR, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  clearLED();
  attachInterrupt(digitalPinToInterrupt(BTN_STOP), stopISR, FALLING);
}

void loop() {
  TaskType newTaskType = NONE;
  String newMsg = "";

  if (digitalRead(BTN_SOS) == LOW) {
    newTaskType = SOS;
    newMsg = "SOS";
    delay(200);
  }
  else if (digitalRead(BTN_BLINK) == LOW) {
    newTaskType = BLINK;
    delay(200);
  }
  else if (digitalRead(BTN_COLOR) == LOW) {
    newTaskType = COLOR;
    delay(200);
  }

  if (newTaskType != NONE) {
    if (currentTask.type != NONE) {
      enqueueTask(currentTask);
    }
    currentTask = { newTaskType, 0, newMsg };
    stopPressed = false;
  }

  if (stopPressed) {
    stopPressed = false;
    delay(250);

    if (currentTask.type != NONE) {
      enqueueTask(currentTask);
      currentTask.type = NONE;
      clearLED();
    }
    else if (!queueEmpty()) {
      dequeueTask(currentTask);
    }
  }

  if (currentTask.type != NONE) {
    bool done = false;
    switch (currentTask.type) {
      case SOS:   done = runSOS(currentTask);   break;
      case BLINK: done = runBlink(currentTask); break;
      case COLOR: done = runColor(currentTask); break;
    }
    if (done) {
      currentTask.type = NONE;
      clearLED();
    }
  }

  delay(5);
}