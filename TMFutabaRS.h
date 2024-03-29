/*---------------------------------------
  TMFutabaRS クラス
  ver. 1.0 2021-05-19
  ver. 1.0.1 2021-05-31
  ver. 1.0.2 2021-06-03

  Futabaのサーボモータ　コマンド制御クラス
  動作確認 Futaba サーボ
  RS304MD

  制御ボードBTE094

  動作確認
  M5Stack
  M5StickC
  M5StickCPlus
  M5Atom

  変更内容
  v1.0.1
    Version(バージョン番号文字列)を設定
    Id(設定されているIDの取得)の関数追加
    ChangeID(IDの書き換え)をRewriteIDに名称変更
    ChangeID(IDの変更)の関数追加
    WaitGoal(サーボの移動を待つ)の関数追加

  v1.0.2
    ChangeBaudRate(通信速度の変更)の引数変更
    ResetFactoryDefault(工場出荷値へのリセット)の引数変更
    RewriteID(IDの書き換え)の引数変更
  
  ---------------------------------------*/
#pragma once

#include "Arduino.h"

// BaudRateの定義
#define COM9600 0x00
#define COM14400 0x01
#define COM19200 0x02
#define COM28800 0x03
#define COM38400 0x04
#define COM57600 0x05
#define COM76800 0x06
#define COM115200 0x07

// FUtaba RSモータをコマンドで動かすクラス
class TMFutabaRS {
  private:
    // サーボID
    unsigned char _Id = 0;

    // 制御用シリアル
    HardwareSerial* _SerialServo;

    // 受信時のタイムアウト時間
    const int _rxTimeout = 30;

    // １ループの待機時間
    const int _loopWaitMs = 5;

    // タイムアウトのループカウント
    const int _cntMax = _rxTimeout / _loopWaitMs;

    // データの送信
    void SendBin(unsigned char* txData, size_t sizeData);

    // RXの受信
    boolean rxReadByte(unsigned char* readData, int cntMax, int waitMs);
    boolean rxReadByte(unsigned char* readData);

    // RXから受信した文字と比較
    boolean rxCompare(unsigned char compData);

    // RXから文字列の受信
    boolean rxRead(unsigned char* readData, size_t sizeData, int countMax, int waitMs);
    boolean rxRead(unsigned char* readData, size_t sizeData);

    // RXのクリア
    void rxClear(void);

    // -- [ RS304MDコマンド ] --
    // トルク
    void torque(unsigned char OnOff);

    // Flash ROMへの書き込み
    void writeFlashRom(void);

    // 再起動
    void reboot(void);

  public:
    // バージョン
    const String Version = "1.0.2";
    
    // Memory Map 00-29(詳細はRS304のマニュアルを参照のこと)
    short int ModelNumber = 0;
    String ModelString(void);
    unsigned char FirmwareVersion = 0;
    unsigned char ID = 0;
    boolean isReverse = false;
    unsigned char _BaudRate = -1;
    int BaudRate(void);
    unsigned short int ReturnDelayMicroSec = 0;
    short int LimitAngleR = 0;
    short int LimitAngleL = 0;
    short int LimitTemp = 0;
    unsigned char TorqueInSilence = -1;
    String TorqueInSilenceString(void);
    unsigned char WarmUpTime = 0;
    unsigned char ComplianceMarginR = 0;
    unsigned char ComplianceMarginL = 0;
    unsigned char ComplianceSlopeR = 0;
    unsigned char ComplianceSlopeL = 0;
    short int Punch = 0;

    // Memory Map 30-36(詳細はRS304のマニュアルを参照のこと)
    short int GoalPosition = 0;
    short int GoalTime = 0;
    unsigned char MaxTorque = 0;
    unsigned char TorqueMode = 0;
    String TorqueModeString(void);

    // Memory Map 42-53(詳細はRS304のマニュアルを参照のこと)
    short int CurrentPosition = 0;
    short int CurrentTime = 0;
    short int CurrentSpeed = 0;
    short int CurrentCurrent = 0;
    short int CurrentTemperature = 0;
    short int CurrentVolts = 0;

    // Memory Map 42-53の前回値
    short int PrevPosition = 0;
    short int PrevTime = 0;
    short int PrevSpeed = 0;
    short int PrevCurrent = 0;
    short int PrevTemperature = 0;
    short int PrevVolts = 0;

    // コンストラクタ
    TMFutabaRS(unsigned char Id);

    // IDの取得
    unsigned char Id(void);

    // IDの変更
    void ChangeId(unsigned char newId);

    // 開始
    void begin(HardwareSerial* SerialServo);

    // メモリマップの読み出し関数
    boolean MemoryMap00to29(void);
    boolean MemoryMap30to59(void);
    boolean MemoryMap42to59(int countMax, int waitMs);
    boolean MemoryMap42to59(void);

    // 今回値と前回値の差を取得する関数
    short int DiffPosition(void);
    short int DiffTime(void);
    short int DiffSpeed(void);
    short int DiffCurrent(void);
    short int DiffTemperature(void);
    short int DiffVolts(void);

    // -- [ RS304MDコマンド関数 ] --
    // Ack
    boolean Ack(void);

    // 通信速度の変更
    void ChangeBaudRate(unsigned char ComSpeed);

    // 工場出荷値へのリセット
    void ResetFactoryDefault(void);

    // IDの書き換え
    void RewriteID(unsigned char newID);

    // トルク
    void TorqueOn(void);
    void TorqueOff(void);
    void TorqueBrake(void);

    // サーボ移動(角度・速度)
    void Move(int pos, int Tms);

    // サーボ移動角度
    void Pos(int pos);

    // サーボ移動時間
    void MoveTime(int Tms);

    // サーボのゴール(移動角度に移動したか)を待つ
    boolean WaitGoal(short int margin);
};
