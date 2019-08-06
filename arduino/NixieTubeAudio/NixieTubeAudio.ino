#include <IRremote.h>
#include <TimerOne.h>
#include <SPI.h>

//the higher speed than digitalWrite
#define setPin(b) ( (b)<8 ? PORTD |=(1<<(b)) : PORTB |=(1<<(b-8)) )
#define clrPin(b) ( (b)<8 ? PORTD &=~(1<<(b)) : PORTB &=~(1<<(b-8)) )
#define tstPin(b) ( (b)<8 ? (PORTD &(1<<(b)))!=0 : (PORTB &(1<<(b-8)))!=0 )


#define TubeData 3
#define TubeShift 4
#define TubeLatch 5
#define TubeEn 6
#define TubeReset 7

#define LedData 11
#define LedShift 13
#define LedLatch 10
#define LedEn 9
#define LedRest 8

unsigned char i=0,data[24]={  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1};
  
unsigned long LedSPIValue = (0xffffffff);
unsigned char LedSPIValueArray[3] = {0xff,0xff,0xff};
const int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {
  // put your setup code here, to run once:
  pinMode(TubeData,OUTPUT);
  pinMode(TubeShift,OUTPUT);
  pinMode(TubeLatch,OUTPUT);
  pinMode(TubeEn,OUTPUT);
  pinMode(TubeReset,OUTPUT);

  pinMode(LedData,OUTPUT);
  pinMode(LedShift,OUTPUT);
  pinMode(LedLatch,OUTPUT);
  pinMode(LedEn,OUTPUT);
  pinMode(LedRest,OUTPUT);

  resetTubeAndLed();

  Timer1.initialize(50); //1us
  Timer1.attachInterrupt(RGBledPWM);

  SPI.setBitOrder(LSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();

  irrecv.enableIRIn();
  irrecv.blink13(true);

  //RGBledControl(63,7);
 
  //open bluetooth audio close FM radio
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  digitalWrite(A4,0);
  digitalWrite(A5,0); //bluetooth


  Serial.begin(9600);
  //TestLed();
  //FastLedControl(63,1);
 // RGBledUpdate();
 
}

void TestLed(){
  int j,k;
  for(j=0;j<63;j++){
    for(k=0;k<8;k++){
      FastLedControl(j,k);
      RGBledUpdate();
      delay(100);
    }
  }
}
void resetTubeAndLed(){
  //led
  digitalWrite(LedData,LOW); //data
  digitalWrite(LedShift,LOW); //shift
  digitalWrite(LedLatch,LOW); //storge
  digitalWrite(LedEn,LOW); //output en 0==output enable
  digitalWrite(LedRest,LOW); //rest low==rest
  digitalWrite(LedRest,HIGH);
  
  //tube
  digitalWrite(TubeData,LOW); //data
  digitalWrite(TubeShift,LOW); //shift
  digitalWrite(TubeLatch,LOW); //storge
  digitalWrite(TubeEn,LOW); //output en 0==output enable
  digitalWrite(TubeReset,LOW); //rest low==rest
  digitalWrite(TubeReset,HIGH);
  

  for(i=0;i<6;i++){
    ControlTube(i,0xf);
  }
  updateTube();
}



unsigned long startTime=0,endTime=0,durationTime=0;

void updateTube(void){
  
 /*  
  digitalWrite(TubeLatch,LOW);//latch pin to low
  for(i=0;i<24;i++){
    digitalWrite(TubeData,data[23-i]); //data
    digitalWrite(TubeShift,HIGH); //shift
    digitalWrite(TubeShift,LOW);  //shift
  }
  digitalWrite(TubeLatch,HIGH);  //latch
  */
  
  /* 
  clrPin(TubeLatch);//latch pin to low
  for(i=0;i<24;i++){
    if(data[23-i]){
      setPin(TubeData);
    }else{
      clrPin(TubeData);
    }
    setPin(TubeShift); //shift
    clrPin(TubeShift);  //shift
  }
  setPin(TubeLatch);  //latch
  */

  PORTD &=~(32); //2^5 = 32 latch pin to low
  for(i=0;i<24;i++){
    if(data[23-i]){
      PORTD |= 8; //2^3 dataPin set 1
    }else{
      PORTD &=~(8); //2^3 dataPin set 0 
    }
    PORTD |= 16; //2^4 = 16 shift high
    PORTD &=~(16);  //shift
  }
  PORTD |= 32;  //2^5 = 32 latch pin to high

}

void ControlTube(unsigned int tube,unsigned int num){
  if(num == 0xf){
    data[tube*4] = 1;
    data[tube*4+1] = 1;
    data[tube*4+2] = 1;
    data[tube*4+3] = 1;

  }else{
    unsigned int A,B,C,D;
    D=num/8;
    C=num/4-D*2;
    B=num/2-C*2-D*4;
    A=num%2;

    data[tube*4] = C;
    data[tube*4+1] = B;
    data[tube*4+2] = D;
    data[tube*4+3] = A;

  }

}

const unsigned char colorTable[8] = {0xF,0xE,0xD,0xC,0xB,0xA,0x9,0x8};
const unsigned char colorTableShift4bit[8] = {0xF0,0xE0,0xD0,0xC0,0xB0,0xA0,0x90,0x80};
void bit_ctrl_0(unsigned long* pflag, unsigned char bitNum) {
  *pflag &= ~((unsigned long)1 << bitNum);
} 
void bit_ctrl_1(unsigned long* pflag, unsigned char bitNum) {
  *pflag |=  ((unsigned long)1 << bitNum);
}

void ledControl(unsigned char led,unsigned char colorValue){
  //static unsigned long LedSPIValue = (0xffffffff);
  /*   
  0x0 off
  0x1 Blue
  0x2 Green
  0x3 Sky Blue
  0x4 Red
  0x5 Purple
  0x6 Yellow
  0x7 White
  */  

  if(colorValue&0x1){
    bit_ctrl_0(&LedSPIValue,led*4);
  }else{
    bit_ctrl_1(&LedSPIValue,led*4);
  }
  
  
  if(colorValue&0x2){
    bit_ctrl_0(&LedSPIValue,led*4+1);
  }else{
    bit_ctrl_1(&LedSPIValue,led*4+1);
  }
  
    
  if(colorValue&0x4){
    bit_ctrl_0(&LedSPIValue,led*4+2);
  }else{
    bit_ctrl_1(&LedSPIValue,led*4+2);
    
  }
  LedSPIValueArray[0] = LedSPIValue;
  LedSPIValueArray[1] = LedSPIValue>>8;
  LedSPIValueArray[2] = LedSPIValue>>16;
 
}

void FastLedControl(unsigned char led,unsigned char colorValue){

  if(led & 0x1){
    LedSPIValueArray[0] &= ~(B00001111);
    LedSPIValueArray[0] |= colorTable[colorValue];
  }
  if(led & 0x2 ){
    LedSPIValueArray[0] &= ~(B11110000);
    LedSPIValueArray[0] |= colorTableShift4bit[colorValue];
  }
  if(led & 0x4 ){
    LedSPIValueArray[1] &= ~(B00001111);
    LedSPIValueArray[1] |= colorTable[colorValue];
  }
  if(led & 0x8 ){
    LedSPIValueArray[1] &= ~(B11110000);
    LedSPIValueArray[1] |= colorTableShift4bit[colorValue];
  }
  if(led & 16 ){
    LedSPIValueArray[2] &= ~(B00001111);
    LedSPIValueArray[2] |= colorTable[colorValue];
  }
  if(led & 32 ){
    LedSPIValueArray[2] &= ~(B11110000);
    LedSPIValueArray[2] |= colorTableShift4bit[colorValue];
  }
  
}
unsigned char brightRed[6]={0},brightBlue[6]={0},brightGreen[6]={0};
unsigned char led;
void ledControlIndividualBright(unsigned char WhichLed,unsigned char color,unsigned char bright){
  if(color == 'r'){
    brightRed[WhichLed] = bright;
  }
  if(color == 'g'){
    brightGreen[WhichLed] = bright;
  }
  if(color == 'b'){
    brightBlue[WhichLed] = bright;
  }
}

void RGBledUpdate(void){
  /*
  digitalWrite(LedLatch,0);//latch pin to low
  SPI.transfer(&LedSPIValue,3);
  digitalWrite(LedLatch,1); //latch pin to high 
  */
  clrPin(LedLatch);//latch pin to low
  SPI.transfer(&LedSPIValueArray,3);
  setPin(LedLatch); //latch pin to high 
  
}

unsigned char j=0;
void RGBledPWM(void){
  static unsigned char TimerOneCount=0;
  TimerOneCount++;
   /*   
  0x0 off
  0x1 Blue
  0x2 Green
  0x3 Sky Blue
  0x4 Red
  0x5 Purple
  0x6 Yellow
  0x7 White
  */  

  //startTime = micros();
  
 
  for(j=0;j<6;j++){
    switch (j)
    {
    case 0:
      led=1;
      break;
    case 1:
      led=2;
      break;
    case 2:
      led=4;
      break;
    case 3:
      led=8;
      break;
    case 4:
      led=16;
      break;
    case 5:
      led=32;
      break;
    default:
      led=0;
      break;
    }
    if(TimerOneCount == (256-brightBlue[j])){
      FastLedControl(led,1);
      RGBledUpdate();

    }
    if(TimerOneCount == (256-brightGreen[j])){
      FastLedControl(led,2);
      RGBledUpdate();

    }
    if(TimerOneCount == (256-brightRed[j])){
      FastLedControl(led,4);
      RGBledUpdate();

    }
  }
   if(TimerOneCount==0){
    LedSPIValueArray[0] = B11111111;
    LedSPIValueArray[1] = B11111111;
    LedSPIValueArray[2] = B11111111; 
    RGBledUpdate();

  }
  
  //endTime = micros();
  //durationTime = endTime - startTime;

  //Serial.println(LedSPIValueArray[0]);
}

unsigned char x;
void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        irrecv.resume();
  }



 // RGBledControl(63,5);
  
  for(i=0;i<6;i++){
      ControlTube(i,9);
  }
  //updateTube();
  //startTime = micros();
  //FastLedControl(63,7);
  //RGBledUpdate();
  //endTime = micros();
 // durationTime = endTime - startTime;
 // Serial.println(durationTime);
    if(x>=9){
      x=0;}
    else{
      x++;
    }
 
 

  
  //delay(1000);
}
