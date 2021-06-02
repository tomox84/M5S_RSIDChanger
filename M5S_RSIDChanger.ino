#include "TMFutabaRS.h"
#include <M5Stack.h>

// Serial2(M5StackはSerial1が使えない)
const int PinRX = 16; // From BTE094 TXD (Blue)
const int PinTX = 17; // From BTE094 RXD (GREEN)

// サーボの定義
TMFutabaRS servo(1);

void setup() {
  M5.begin();
  M5.Power.begin();

  // サーボ制御用
  Serial2.begin(115200, SERIAL_8N1, PinRX, PinTX);

  // サーボにサーボ制御用シリアルをセット
  servo.begin(&Serial2);

  // 画面設定
  M5.Lcd.setRotation(0);
  M5.Lcd.setTextFont(1);
  M5.Lcd.setTextSize(2);

}

void loop() {
  // put your main code here, to run repeatedly:

}
