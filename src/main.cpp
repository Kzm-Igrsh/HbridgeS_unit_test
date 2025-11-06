#include <M5Unified.h>

// M5Stack CoreのPortA ピン定義
const int PWM0_PIN = 21;  // G21
const int PWM1_PIN = 22;  // G22

// PWMチャンネル設定
const int PWM0_CH = 0;
const int PWM1_CH = 1;
const int PWM_FREQ = 1000;  // 1000Hz
const int PWM_RES = 8;      // 8bit (0-255)

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  M5.Display.clear();
  M5.Display.setTextSize(2);
  M5.Display.setCursor(0, 0);
  M5.Display.println("HbridgeS PWM");
  M5.Display.println("");
  M5.Display.println("A: Forward");
  M5.Display.println("B: Reverse");
  M5.Display.println("C: Stop");
  
  // PWM初期化
  ledcSetup(PWM0_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM0_PIN, PWM0_CH);
  ledcWrite(PWM0_CH, 0);  // duty = 0
  
  ledcSetup(PWM1_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM1_PIN, PWM1_CH);
  ledcWrite(PWM1_CH, 0);  // duty = 0
}

void loop() {
  M5.update();
  
  // Button A: PWM0=50, PWM1=0 (Forward)
  if (M5.BtnA.wasPressed()) {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(2);
    M5.Display.println("Forward");
    M5.Display.println("");
    M5.Display.println("PWM0: 50");
    M5.Display.println("PWM1: 0");
    
    ledcWrite(PWM0_CH, 50);
    ledcWrite(PWM1_CH, 0);
  }
  
  // Button B: PWM0=0, PWM1=50 (Reverse)
  if (M5.BtnB.wasPressed()) {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(2);
    M5.Display.println("Reverse");
    M5.Display.println("");
    M5.Display.println("PWM0: 0");
    M5.Display.println("PWM1: 250");
    
    ledcWrite(PWM0_CH, 0);
    ledcWrite(PWM1_CH, 250);
  }
  
  // Button C: PWM0=0, PWM1=0 (Stop)
  if (M5.BtnC.wasPressed()) {
    M5.Display.clear();
    M5.Display.setCursor(0, 0);
    M5.Display.setTextSize(2);
    M5.Display.println("Stop");
    M5.Display.println("");
    M5.Display.println("PWM0: 0");
    M5.Display.println("PWM1: 0");
    
    ledcWrite(PWM0_CH, 0);
    ledcWrite(PWM1_CH, 0);
  }
  
  delay(10);
}