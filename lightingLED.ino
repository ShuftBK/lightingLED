int LEDpin[14] = {-1,-1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int codes[4] = {'1', '0', 'P', '0'};
int code_i = 0;
unsigned long time;
int i = 3;
int enablePin = 2;
int minPinNum = 2;
int code = '1';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEDpin[2], OUTPUT);
  pinMode(LEDpin[3], OUTPUT);
  time = millis();
}

void allon(){
  for(int it = minPinNum; it < minPinNum+enablePin; it++){
    digitalWrite(LEDpin[it], HIGH);
  }
  delay(2000);
}

void alloff(){
  for(int it = minPinNum; it < minPinNum+enablePin; it++){
    digitalWrite(LEDpin[it], LOW);
  }
  delay(1000);
}

void normal(){
  digitalWrite(LEDpin[i], HIGH);
  delay(1000);
  digitalWrite(LEDpin[i], LOW);
  i++;
  if(i > 1+enablePin) i = 2;
  digitalWrite(LEDpin[i], HIGH);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(codes[code_i], HEX);
  switch(codes[code_i]){
    case '1':
      allon();
      break;
    case '0':
      alloff();
      break;
    case 'P':
      normal();
      break;
  }
  code_i++;
  if(code_i > 3)code_i = 0;
}


