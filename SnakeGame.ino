#include <LedControl.h>

const int latch = 9;
const int clock = 10;
const int data = 8;

typedef struct Snake Snake;
struct Snake{
  int head[2];
  int body[64][2];
  int len;
  int dir[2];
};

typedef struct Apple Apple;
struct Apple{
  int rPos;
  int cPos;
};

const int DIN = 53;
const int CS = 51;
const int CLK = 52;
LedControl lc = LedControl(DIN, CLK, CS,1);

const int varXPin = A3;
const int varYPin = A4;

byte pic[8] = {0,0,0,0,0,0,0,0};
unsigned char table[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
Snake snake = {{1,5},{{0,5}, {1,5}}, 2, {1,0}};
Apple apple = {(int)random(0,8),(int)random(0,8)};

float oldTime = 0;
float timer = 0;
float updateRate = 3;
int i,j;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,8);
  lc.clearDisplay(0);
  pinMode(varXPin, INPUT);
  pinMode(varYPin, INPUT);
  pinMode(latch, OUTPUT);
  pinMode(clock, OUTPUT);
  pinMode(data, OUTPUT);
  DisplayNumber(0);
}

void loop() {
  float deltaTime = calculateDeltaTime();
  timer += deltaTime;
  int xVal = analogRead(varXPin);
  int yVal = analogRead(varYPin);
  
  if(xVal<100 && snake.dir[1]==0){
    snake.dir[0] = 0;
    snake.dir[1] = -1;
  }else if(xVal >920 && snake.dir[1]==0){
    snake.dir[0] = 0;
    snake.dir[1] = 1;
  }else if(yVal<100 && snake.dir[0]==0){
    snake.dir[0] = -1;
    snake.dir[1] = 0;
  }else if(yVal >920 && snake.dir[0]==0){
    snake.dir[0] = 1;
    snake.dir[1] = 0;
  }
  
  if(timer > 1000/updateRate){
    timer = 0;
    Update();
  }
  
  Render();
  
}

float calculateDeltaTime(){
  float currentTime = millis();
  float dt = currentTime - oldTime;
  oldTime = currentTime;
  return dt;
}

void reset(){
  for(int j=0;j<8;j++){
    pic[j] = 0;
  }
}
void Update(){
  reset();
  
  int newHead[2] = {snake.head[0]+snake.dir[0], snake.head[1]+snake.dir[1]};

  if(newHead[0]==8){
    newHead[0]=0;
  }else if(newHead[0]==-1){
    newHead[0] = 7;
  }else if(newHead[1]==8){
    newHead[1]=0;
  }else if(newHead[1]==-1){
    newHead[1]=7;
  }
  
   for(j=0;j<snake.len;j++){
    if(snake.body[j][0] == newHead[0] && snake.body[j][1] == newHead[1]){
      delay(1000);
      snake = {{1,5},{{0,5}, {1,5}}, 2, {1,0}};
      apple = {(int)random(0,8),(int)random(0,8)};
      DisplayNumber(0);
      return;
    }
  }

  if(newHead[0] == apple.rPos && newHead[1] ==apple.cPos){
    snake.len = snake.len+1;
    DisplayNumber(snake.len - 2);
    apple.rPos = (int)random(0,8);
    apple.cPos = (int)random(0,8);
  }else{
    removeFirst();
  }
  
  snake.body[snake.len-1][0]= newHead[0];
  snake.body[snake.len-1][1]= newHead[1];
  
  snake.head[0] = newHead[0];
  snake.head[1] = newHead[1];
  
  for(j=0;j<snake.len;j++){
    pic[snake.body[j][0]] |= 128 >> snake.body[j][1];
  }
  pic[apple.rPos] |= 128 >> apple.cPos;
  
}

void Render(){
  
   for(i=0;i<8;i++){
    lc.setRow(0,i,pic[i]);
   }
}

void removeFirst(){
  for(j=1;j<snake.len;j++){
    snake.body[j-1][0] = snake.body[j][0];
    snake.body[j-1][1] = snake.body[j][1];
  }
}

void DisplayNumber(int number) {
  int leftDigit = number / 10;
  int rightDigit = number % 10;
  digitalWrite(latch, LOW);
  shiftOut(data, clock, MSBFIRST, table[leftDigit]);
  shiftOut(data, clock, MSBFIRST, table[rightDigit]);
  digitalWrite(latch, HIGH);
}
