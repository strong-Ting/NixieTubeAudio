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
  
unsigned char ledData[24] = {1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1};


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
  
  Serial.begin(9600);
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



void ledControl(unsigned char led,unsigned char colorValue){
  /*
  0x0 off
  0x1 R
  0x2 G
  0x3 yellow
  0x4 B
  0x5 purple
  0x6 sky Blue
  0x7 white
   */ 
  ledData[led*4+1] = !(colorValue&0x1);
  ledData[led*4+2] = !(colorValue&0x2);
  ledData[led*4+3] = !(colorValue&0x4);
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
  
  RGBledUpdate();
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
void RGBledUpdate(void){
  
  digitalWrite(LedLatch,0);//latch pin to low
  SPI.transfer(B10001000);
  digitalWrite(LedLatch,1); //latch pin to high 
}


unsigned char x;
void loop() {
  // put your main code here, to run repeatedly:
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        irrecv.resume();
  }

  RGBledUpdate();
  
  for(i=0;i<6;i++){
      ControlTube(i,x);
  }
  
    updateTube();
    if(x>=9){
      x=0;}
    else{
      x++;
    }
   for(i=0;i<24;i++){
      Serial.print(data[i]); 
   }
   Serial.print('\n'); 
  
  delay(1000);
}
