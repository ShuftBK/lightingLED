#include <SoftwareSerial.h>

const int rxPin = 3;const int txPin = 9;
const int diff = 0.25*1000;
char controlword;//現在のモードを格納する変数
char stack;//シリアルデータを一時保管する為の変数
SoftwareSerial SignalSerial(rxPin,txPin);

typedef struct{
  bool pulse;
  int bpm;
  unsigned int time;
} SensorData_t;

SensorData_t sensorData[2];
int data_1;int data_2;

void sensorDataInit(){
  for(int i = 0; i < 2; ++i){
    sensorData[i].pulse = 0;
    sensorData[i].bpm = 0;
    sensorData[i].time = millis();
  }
}

void setup(){
  Serial.begin(9600);//シリアルポートの通信速度を9600bpsに設定  
  SignalSerial.begin(9600);
  pinMode(4, OUTPUT); // small
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT); // big 
  sensorDataInit();
}

void loop(){
  get_word();//モード命令を取得

  switch(controlword){
    case '0':
      all_off();
      break;
    case '1':
      all_on();
      break;
    case 'P':
      if(get_data())
        pattern();
      else
        pattern2();
      break;
    case 'C':
      if(get_data())
        combination();
     // else
       // combination2();
      break;
    default:
      break;
  }
}

void all_off(){
  //全消灯の処理を記述
  for(int i = 4; i <= 7; ++i){
    digitalWrite(i, LOW);
  }
  d_delay(500);
}


void all_on(){
  //全点灯の処理を記述
  for(int i = 4; i <= 7; ++i){
    digitalWrite(i, HIGH);
  }
  d_delay(1000);
}

void dededen(int x = 7){
  for(int j = 0; j < 2; ++j){
    digitalWrite(4, HIGH);
    d_delay(100);
    for(int i = 5; i <= 7; i++){
      digitalWrite(i, HIGH);
      d_delay(100);
      digitalWrite(i-1, LOW);      
    }
    d_delay(100);
    digitalWrite(7, LOW);
    d_delay(100);
  }
  for(int i = 4; i <= x; ++i){
    digitalWrite(i, HIGH);
    d_delay(100);
  }
  d_delay(1000);
  all_off();
}

void pattern2(){
  Serial.println("Pattern");
  all_on();
}

void pattern(){
  all_off();
  if(sensorData[0].pulse | sensorData[1].pulse){
    int i = 4;
    Serial.println("Pattern hoge");
    while(true){
      digitalWrite(i, HIGH);
      d_delay(200);
      digitalWrite(i, LOW);
      ++i;
      if(i > 7) break;
      digitalWrite(i, HIGH);
    }
  }
}

void combination2(){
  Serial.println("Combination");
  //sensorDataInit();
  digitalWrite(4, HIGH);
  d_delay(100);
  for(int i = 5; i <= 7; i++){
    digitalWrite(i, HIGH);
    d_delay(100);
    digitalWrite(i-1, LOW);      
  }
  d_delay(100);
  digitalWrite(7, LOW);
  d_delay(100);
}

void combination(){
  //連携モードの処理を記述
  //連携モードに関係のないオブジェは記述する必要なし
  all_off();
  Serial.println("Combination hoge");
  if(sensorData[0].pulse & sensorData[1].pulse){
    int pulseDelay = abs(sensorData[0].time - sensorData[1].time);
    if(pulseDelay < diff){
      dededen();
    }
    else{
      randomSeed(millis());
      dededen(random(4,7));
    }
  }
}


bool d_delay(unsigned long delay_time){//delay関数の代替
  unsigned long start_time;
  start_time = millis();//d_delay関数の実行が開始された時間を記憶
  while(millis() <= start_time + delay_time){//指定時間だけ遅延させる
  if(get_word())
    return true;//モードが変更された場合、trueを返し、遅延処理を中止  
  }
  return false;//処理が完了(モードが変更されなかった)場合、falseを返す
}

bool get_word(){//モード命令(シリアルデータ)を取得する為の関数
  while(Serial.available() > 0)//最後に受信したデータだけを
    stack = Serial.read();//stackに格納する処理
  if(stack != controlword&&stack != -1){
      controlword=stack;//モード状態を記憶している変数を書き換える
      return true;//モードが変更された場合、trueを返す
    }
  else
    return false;//モードが変更されなかった場合、falseを返す
}

bool get_data(){
  if(SignalSerial.available() >= sizeof(byte)+sizeof(int))

  if(SignalSerial.read()=='H'){
    data_1 = SignalSerial.read();
    data_2 = SignalSerial.read();
    Serial.println("get_data");
    int id = bitRead(data_1,4);
    
    sensorData[id].pulse = bitRead(data_1,1);
    sensorData[id].bpm = data_2;
    sensorData[id].time = millis();

    Serial.println(data_1, BIN);
    Serial.println(data_2, BIN);
    
    return true;
  }
  
  return false;
}
