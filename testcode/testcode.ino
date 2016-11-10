const unsigned int data[] = {0x8096, 0xE064, 0x4000, 0xA04C, 0x0000};
const int LEDpin[14] = {-1,-1, -1, -1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
unsigned int data_i = 0;

/* Debug mode */
bool isDebugMode = true;

/* pulse data */
bool pulse;
int objectID;
int mode;
int bpm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEDpin[4], OUTPUT); // 
  pinMode(LEDpin[5], OUTPUT); //
  pinMode(LEDpin[6], OUTPUT); // 
  pinMode(LEDpin[7], OUTPUT); //
}

void parseData(unsigned int d){
  pulse = (0x8000&d) >> 15;
  int mode1 = (0x4000&d) >> 14;
  int mode2 = (0x2000&d) >> 13;
  objectID = (0x1000&d) >> 12;
  bpm = (0x00FF&d);
  mode = 10*mode1 + mode2;
  if(isDebugMode){
    Serial.println("pulse : ");
    Serial.println(pulse);
    Serial.println("mode : ");
    Serial.println(mode, DEC);
    Serial.println("objectID : ");
    Serial.println(objectID, DEC);
    Serial.println("bpm : ");
    Serial.println(bpm, DEC);
  }
}

void all_off(){
  for(int i = 4; i <= 7; ++i){
    digitalWrite(i, LOW);
  }
  delay(100);
}

void dededen(int x = 7){
  for(int j = 0; j < 2; ++j){
    digitalWrite(4, HIGH);
    delay(100);
    for(int i = 5; i <= 7; i++){
      digitalWrite(LEDpin[i], HIGH);
      delay(100);
      digitalWrite(LEDpin[i-1], LOW);      
    }
    delay(100);
    digitalWrite(7, LOW);
    delay(100);
  }
  for(int i = 4; i <= x; ++i){
    digitalWrite(LEDpin[i], HIGH);
    delay(100);
  }
  delay(1000);
  all_off();
}

void combination(){
  randomSeed(millis());
  double diff = 0.5*1000;
  int pulseDelay = random(0, 1000);
  Serial.println("pulseDelay:");
  Serial.println(pulseDelay);
  if(pulseDelay < diff){
    dededen();
  }
  else{
    int rnd = random(4,7);
    Serial.println("random:");
    Serial.println(rnd);
    dededen(rnd);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  combination();
}
