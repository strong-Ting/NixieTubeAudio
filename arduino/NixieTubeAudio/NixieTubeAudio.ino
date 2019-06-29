#include <IRremote.h>
#include <TimerOne.h>
#include <SPI.h>

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
/*
  Timer1.initialize(1); //1us
  Timer1.attachInterrupt(RGBledPWM);
*/
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
  generateLedTable();
    
 
}

void generateLedTable(){
   int j,k;
  Serial.print("{");
  for(j=0;j<63;j++){
    Serial.print("{");
    for(k=0;k<8;k++){
      RGBledControl(j,k);
      RGBledUpdate(LedSPIValue);
      if(k!=0){
      Serial.print(",");
      }
       Serial.print("0x");
      Serial.print(LedSPIValue,HEX);
      delay(100);
    }
     Serial.print("}");
    if(j!=63){
     Serial.print(",\n");
    }
  }
  Serial.print("}\n");

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
  

  digitalWrite(TubeLatch,LOW);//latch pin to low
  for(i=0;i<24;i++){
    digitalWrite(TubeData,data[23-i]); //data
    digitalWrite(TubeShift,HIGH); //shift
    digitalWrite(TubeShift,LOW);  //shift
  }
  digitalWrite(TubeLatch,HIGH);  //latch

  
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

unsigned long LedTable[6][8] = {{0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFD,0xFFFFFFFC,0xFFFFFFFB,0xFFFFFFFA,0xFFFFFFF9,0xFFFFFFF8},
{0xFFFFFFF8,0xFFFFFFE8,0xFFFFFFD8,0xFFFFFFC8,0xFFFFFFB8,0xFFFFFFA8,0xFFFFFF98,0xFFFFFF88},
{0xFFFFFF88,0xFFFFFE88,0xFFFFFD88,0xFFFFFC88,0xFFFFFB88,0xFFFFFA88,0xFFFFF988,0xFFFFF888},
{0xFFFFF888,0xFFFFE888,0xFFFFD888,0xFFFFC888,0xFFFFB888,0xFFFFA888,0xFFFF9888,0xFFFF8888},
{0xFFFF8888,0xFFFE8888,0xFFFD8888,0xFFFC8888,0xFFFB8888,0xFFFA8888,0xFFF98888,0xFFF88888},
{0xFFF88888,0xFFE88888,0xFFD88888,0xFFC88888,0xFFB88888,0xFFA88888,0xFF988888,0xFF888888}}; 

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
  startTime=0,endTime=0,durationTime=0;
  startTime = micros();
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
  endTime = micros();
  durationTime = endTime - startTime;
  Serial.println(durationTime);
  //RGBledUpdate(LedSPIValue);
}

void FastLedControl(unsigned char led,unsigned char colorValue){
  LedSPIValue &= (LedTable[led][colorValue]);
  //RGBledUpdate(LedTable[led][colorValue]);
}

void RGBledControl(unsigned char led,unsigned char color){
  if(led & 0x1){
    ledControl(0,color);
  }
  if(led & 0x2 ){
    ledControl(1,color);
  }
  if(led & 0x4 ){
    ledControl(2,color);
  }
  if(led & 0x8 ){
    ledControl(3,color);
  }
  if(led & 16 ){
    ledControl(4,color);
  }
  if(led & 32 ){
    ledControl(5,color);
  }
}


void FastRGBledControl(unsigned char led,unsigned char color){
  if(led & 0x1){
    FastLedControl(0,color);
  }
  if(led & 0x2 ){
    FastLedControl(1,color);
  }
  if(led & 0x4 ){
    FastLedControl(2,color);
  }
  if(led & 0x8 ){
    FastLedControl(3,color);
  }
  if(led & 16 ){
    FastLedControl(4,color);
  }
  if(led & 32 ){
    FastLedControl(5,color);
  }
}



unsigned char ledValue,colorValue,brightValue,microSecCount;
void RGBledPWM(){

  if(microSecCount<255){
    microSecCount++; 
  }else{
    microSecCount=0;
  }
  
  if(brightValue==microSecCount){
    //RGBledControl(ledValue,0);
    digitalWrite(4,1); //output en 0==output enable
  }else if(microSecCount==0){
    //RGBledControl(ledValue,colorValue);
    digitalWrite(4,0); //output en 0==output enable
  }
  
}
void RGBledUpdate(unsigned long LedSPIValue){
  digitalWrite(LedLatch,0);//latch pin to low
  SPI.transfer(&LedSPIValue,3);
  digitalWrite(LedLatch,1); //latch pin to high 
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
      ControlTube(i,x);
  }
  
    updateTube();
    if(x>=9){
      x=0;}
    else{
      x++;
    }
 
  
  delay(1000);
}
