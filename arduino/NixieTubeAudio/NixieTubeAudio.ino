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

  int j,k;
  /*
  for(j=0;j<6;j++){
    for(k=0;k<8;k++){
      ledControl(j,k);
      delay(300);
    }
  }*/
  ledControl(4,5);
  

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

const unsigned long LedTable[6][8] ={ {0xFFFFFFFF,0xFFFFFFFE,0xFFFFFFFC,0xFFFFFFFC,0xFFFFFFF8,0xFFFFFFF8,0xFFFFFFF8,0xFFFFFFF8},
                                      {0xFFFFFFFF,0xFFFFFFEF,0xFFFFFFCF,0xFFFFFFCF,0xFFFFFF8F,0xFFFFFF8F,0xFFFFFF8F,0xFFFFFF8F},
                                      {0xFFFFFFFF,0xFFFFFEFF,0xFFFFFCFF,0xFFFFFCFF,0xFFFFF8FF,0xFFFFF8FF,0xFFFFF8FF,0xFFFFF8FF},
                                      {0xFFFFFFFF,0xFFFFEFFF,0xFFFFCFFF,0xFFFFCFFF,0xFFFF8FFF,0xFFFF8FFF,0xFFFF8FFF,0xFFFF8FFF},
                                      {0xFFFFFFFF,0xFFFEFFFF,0xFFFCFFFF,0xFFFCFFFF,0xFFF8FFFF,0xFFF8FFFF,0xFFF8FFFF,0xFFF8FFFF},
                                      {0xFFFFFFFF,0xFFEFFFFF,0xFFCFFFFF,0xFFCFFFFF,0xFF8FFFFF,0xFF8FFFFF,0xFF8FFFFF,0xFF8FFFFF}};  
/*
void ledControl(unsigned char led,unsigned char colorValue){
  
  0x0 off
  0x1 Blue
  0x2 Green
  0x3 Sky Blue
  0x4 Red
  0x5 Purple
  0x6 Yellow
  0x7 White
  
  
  static unsigned long LedSPIValue = ~(0xffffffff);
  startTime = micros();
 
  //speed slow
  LedSPIValue |= (unsigned long)(colorValue&0x1)<<(led*4);
  LedSPIValue |= (unsigned long)(colorValue&0x2)<<(led*4);
  LedSPIValue |= (unsigned long)(colorValue&0x4)<<(led*4);
  //LedSPIValue = ~LedSPIValue;
  
  //LedSPIValue = LedTable[led][colorValue];
  endTime = micros();

  RGBledUpdate(~LedSPIValue);
  
  durationTime = endTime - startTime;
  //Serial.println(durationTime);
    Serial.print(",0x");
  Serial.print(~LedSPIValue,HEX);
  


  
}
*/
//unsigned long LedSPIValue = 0xffffffff;
void bit_ctrl_0(unsigned long* pflag, unsigned char bitNum) {
  *pflag &= ~((unsigned long)1 << bitNum);
} 
void bit_ctrl_1(unsigned long* pflag, unsigned char bitNum) {
  *pflag |=  ((unsigned long)1 << bitNum);
}

void ledControl(unsigned char led,unsigned char colorValue){
  static unsigned long LedSPIValue = (0xffffffff);
 

  if(colorValue&0x1){
    bit_ctrl_0(&LedSPIValue,led*4);
  }else{
    bit_ctrl_1(&LedSPIValue,led*4);
  }
   Serial.println(LedSPIValue,HEX);
  
  if(colorValue&0x2){
    bit_ctrl_0(&LedSPIValue,led*4+1);
  }else{
    bit_ctrl_1(&LedSPIValue,led*4+1);
  }
    Serial.println(LedSPIValue,HEX);
    
  if(colorValue&0x4){
    bit_ctrl_0(&LedSPIValue,led*4+2);
  }else{
    bit_ctrl_1(&LedSPIValue,led*4+2);
    
  }
   Serial.println(LedSPIValue,HEX);
  

  Serial.println(LedSPIValue,HEX);
  RGBledUpdate(LedSPIValue);
  
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
