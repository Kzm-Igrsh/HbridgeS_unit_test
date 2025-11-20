#include <M5Unified.h>

// M5Stack CoreのPortA ピン定義
const int PWM0_PIN = 21;  // G21
const int PWM1_PIN = 22;  // G22

// PWMチャンネル設定
const int PWM0_CH = 0;
const int PWM1_CH = 1;
const int PWM_FREQ = 1000;  // 1000Hz
const int PWM_RES = 8;      // 8bit (0-255)

// モード定義
enum Mode {
  MODE_FORWARD,
  MODE_REVERSE,
  MODE_STOP
};

Mode currentMode = MODE_STOP;
int powerValue = 0;  // 0-255

void updateDisplay() {
  M5.Display.clear();
  M5.Display.setTextSize(2);
  M5.Display.setCursor(0, 0);
  
  // モード表示
  switch(currentMode) {
    case MODE_FORWARD:
      M5.Display.println("Forward");
      break;
    case MODE_REVERSE:
      M5.Display.println("Reverse");
      break;
    case MODE_STOP:
      M5.Display.println("Stop");
      break;
  }
  
  M5.Display.println("");
  M5.Display.printf("Power: %d\n", powerValue);
  M5.Display.println("");
  
  if (currentMode != MODE_STOP) {
    M5.Display.printf("PWM0: %d\n", (currentMode == MODE_FORWARD) ? powerValue : 0);
    M5.Display.printf("PWM1: %d\n", (currentMode == MODE_REVERSE) ? powerValue : 0);
  } else {
    M5.Display.println("PWM0: 0");
    M5.Display.println("PWM1: 0");
  }
  
  M5.Display.println("");
  M5.Display.setTextSize(1);
  M5.Display.println("A: -10  B: +10  C: Mode");
}

void applyPWM() {
  switch(currentMode) {
    case MODE_FORWARD:
      ledcWrite(PWM0_CH, powerValue);
      ledcWrite(PWM1_CH, 0);
      break;
    case MODE_REVERSE:
      ledcWrite(PWM0_CH, 0);
      ledcWrite(PWM1_CH, powerValue);
      break;
    case MODE_STOP:
      ledcWrite(PWM0_CH, 0);
      ledcWrite(PWM1_CH, 0);
      break;
  }
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // PWM初期化
  ledcSetup(PWM0_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM0_PIN, PWM0_CH);
  ledcWrite(PWM0_CH, 0);
  
  ledcSetup(PWM1_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM1_PIN, PWM1_CH);
  ledcWrite(PWM1_CH, 0);
  
  updateDisplay();
}

void loop() {
  M5.update();
  
  // Button A: パワー -10
  if (M5.BtnA.wasPressed()) {
    powerValue = max(0, powerValue - 10);
    applyPWM();
    updateDisplay();
  }
  
  // Button B: パワー +10
  if (M5.BtnB.wasPressed()) {
    powerValue = min(255, powerValue + 10);
    applyPWM();
    updateDisplay();
  }
  
  // Button C: モード切り替え (Forward → Reverse → Stop → Forward...)
  if (M5.BtnC.wasPressed()) {
    switch(currentMode) {
      case MODE_FORWARD:
        currentMode = MODE_REVERSE;
        break;
      case MODE_REVERSE:
        currentMode = MODE_STOP;
        powerValue = 0;  // Stopに切り替えたらパワーをリセット
        break;
      case MODE_STOP:
        currentMode = MODE_FORWARD;
        break;
    }
    applyPWM();
    updateDisplay();
  }
  
  delay(10);
}