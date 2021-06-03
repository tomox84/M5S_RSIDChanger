#include "TMFutabaRS.h"
#include <M5Stack.h>

// Serial2(M5StackはSerial1が使えない)
const int PinRX = 16; // From BTE094 TXD (Blue)
const int PinTX = 17; // From BTE094 RXD (GREEN)

// サーボの定義
TMFutabaRS servo(1);

// モード
int CrntMode = 1;
int PrevMode = 0;

// 新しい(書き換える)ID
unsigned char NewId = 1;

// -- Setup --
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

// -- Loop --
void loop() {
  Disp();
  Buttons();

  M5.update();
  delay(50);
}

void Buttons(void) {
  if (CrntMode == 1) {
    // ボタン入力
    if (M5.BtnA.wasPressed()) {
      // IDを増やす
      unsigned char newId = servo.Id() + 1;
      if (newId > 100) newId = 1;
      servo.ChangeId(newId);
      DispCurrentID();
      DispConnect();
    }
    if (M5.BtnB.wasPressed()) {
      // IDを減らす
      unsigned char newId = servo.Id() - 1;
      if (newId < 1) newId = 100;
      servo.ChangeId(newId);
      DispCurrentID();
      DispConnect();
    }
    if (M5.BtnC.wasPressed()) {
      if (servo.Ack()) CrntMode = 2;
    }
  }
  else if (CrntMode == 2) {
    // ボタン入力
    if (M5.BtnA.wasPressed()) {
      // IDを増やす
      NewId++;
      if (NewId > 100) NewId = 1;
      DispNewID();
    }
    if (M5.BtnB.wasPressed()) {
      // IDを減らす
      NewId--;
      if (NewId < 1) NewId = 100;
      DispNewID();
    }
    if (M5.BtnC.wasPressed()) {
      CrntMode = 3;
    }
  }
  else if (CrntMode == 3) {
    // ボタン入力
    if (M5.BtnB.wasPressed()) {
      M5.Lcd.fillScreen(BLUE);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setCursor(54, 150);
      M5.Lcd.print("Changing...");
      servo.RewriteID(NewId);
      CrntMode = 1;
    }
    if (M5.BtnC.wasPressed()) {
      CrntMode = 1;
    }
  }
}

void Disp(void) {
  if (CrntMode == PrevMode) return;

  PrevMode = CrntMode;
  if (CrntMode == 1) {
    // タイトル
    M5.Lcd.fillScreen(BLUE);
    M5.Lcd.fillRect(0, 0, 240, 30, WHITE);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.setCursor(46, 8);
    M5.Lcd.print("RS ID Changer");

    // ボタンのメニュー表示
    M5.Lcd.setTextColor(BLUE, WHITE);
    M5.Lcd.fillRect(1, 34, 30, 60, WHITE);
    M5.Lcd.setCursor(4, 57);
    M5.Lcd.print(">>");

    M5.Lcd.fillRect(1, 130, 30, 60, WHITE);
    M5.Lcd.setCursor(4, 153);
    M5.Lcd.print("<<");

    M5.Lcd.fillRect(1, 226, 30, 60, WHITE);
    M5.Lcd.setCursor(5, 249);
    M5.Lcd.print("Go");

    // ID表示
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(64, 100);
    M5.Lcd.print("Current ID");
    M5.Lcd.drawRect(78, 132, 92, 54, WHITE);
    DispCurrentID();

    // 接続状況表示
    DispConnect();

    // メッセージ表示
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(64, 240);
    M5.Lcd.print(">>  ID+1");
    M5.Lcd.setCursor(64, 260);
    M5.Lcd.print("<<  ID-1");
    M5.Lcd.setCursor(64, 280);
    M5.Lcd.print("Go  Go New ID");
  }
  if (CrntMode == 2) {
    // タイトル
    M5.Lcd.fillScreen(BLUE);
    M5.Lcd.fillRect(0, 0, 240, 30, WHITE);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.setCursor(46, 8);
    M5.Lcd.print("RS ID Changer");

    // ボタンのメニュー表示
    M5.Lcd.setTextColor(BLUE, WHITE);
    M5.Lcd.fillRect(1, 34, 30, 60, WHITE);
    M5.Lcd.setCursor(4, 57);
    M5.Lcd.print(">>");

    M5.Lcd.fillRect(1, 130, 30, 60, WHITE);
    M5.Lcd.setCursor(4, 153);
    M5.Lcd.print("<<");

    M5.Lcd.fillRect(1, 226, 30, 60, WHITE);
    M5.Lcd.setCursor(5, 249);
    M5.Lcd.print("Go");

    // ID表示
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(90, 100);
    M5.Lcd.print("New ID");
    M5.Lcd.drawRect(78, 132, 92, 54, WHITE);
    DispNewID();

    // メッセージ表示
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(64, 240);
    M5.Lcd.print(">>  ID+1");
    M5.Lcd.setCursor(64, 260);
    M5.Lcd.print("<<  ID-1");
    M5.Lcd.setCursor(64, 280);
    M5.Lcd.print("Go  Go Change");
  }
  if (CrntMode == 3) {
    // タイトル
    M5.Lcd.fillScreen(BLUE);
    M5.Lcd.fillRect(0, 0, 240, 30, WHITE);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.setCursor(46, 8);
    M5.Lcd.print("RS ID Changer");

    // ボタンのメニュー表示
    M5.Lcd.setTextColor(BLUE, WHITE);
    M5.Lcd.fillRect(1, 130, 30, 60, WHITE);
    M5.Lcd.setCursor(4, 153);
    M5.Lcd.print("Do");

    M5.Lcd.fillRect(1, 226, 30, 60, WHITE);
    M5.Lcd.setCursor(5, 249);
    M5.Lcd.print("No");

    // ID表示
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(40, 100);
    M5.Lcd.printf("Current ID:%3d", servo.Id());
    M5.Lcd.setCursor(110, 135);
    M5.Lcd.print("\\/");
    M5.Lcd.setCursor(116, 133);
    M5.Lcd.print("|");
    M5.Lcd.setCursor(116, 125);
    M5.Lcd.print("|");
    M5.Lcd.setCursor(40, 160);
    M5.Lcd.printf("   New  ID:%3d", NewId);

    // メッセージ表示
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(50, 260);
    M5.Lcd.print("Do  Do Change");
    M5.Lcd.setCursor(50, 280);
    M5.Lcd.print("No  Cancel");
  }
}


// 現在のIDの表示
void DispCurrentID(void) {
  M5.Lcd.setTextColor(WHITE, BLUE);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setCursor(90, 144);
  M5.Lcd.printf("%3d", servo.Id());
  M5.Lcd.setTextSize(2);
}

// NewIDの表示
void DispNewID(void) {
  M5.Lcd.setTextColor(WHITE, BLUE);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setCursor(90, 144);
  M5.Lcd.printf("%3d", NewId);
  M5.Lcd.setTextSize(2);
}

// 接続状況表示
void DispConnect(void) {
  M5.Lcd.setCursor(46, 200);
  if (servo.Ack()) {
    M5.Lcd.setTextColor(WHITE, BLUE);
    M5.Lcd.print("  Connected  ");
  }
  else {
    M5.Lcd.setTextColor(RED, BLUE);
    M5.Lcd.print("Not connected");
  }
}
