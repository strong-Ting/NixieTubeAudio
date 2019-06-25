unsigned char i=0,data[24]={	1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1};
  
unsigned char ledData[24] = {1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1};

void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);

  resetTubeAndLed();

  Serial.begin(9600);
}
void resetTubeAndLed(){
  //led
  digitalWrite(3,LOW); //data
	digitalWrite(6,LOW); //shift
	digitalWrite(5,LOW); //storge
	digitalWrite(4,LOW); //output en 0==output enable
	digitalWrite(7,LOW); //rest low==rest
  digitalWrite(7,HIGH);
  
  //tube
  digitalWrite(8,LOW); //data
	digitalWrite(9,LOW); //shift
	digitalWrite(10,LOW); //storge
	digitalWrite(11,LOW); //output en 0==output enable
	digitalWrite(12,LOW); //rest low==rest
	digitalWrite(12,HIGH);
	

	for(i=0;i<6;i++){
		ControlTube(i,0xf);
	}
	updateTube();
}




void updateTube(void){
	digitalWrite(10,LOW);//latch pin to low
	for(i=0;i<24;i++){
		digitalWrite(8,data[23-i]); //data
		digitalWrite(9,HIGH); //shift
		digitalWrite(9,LOW);  //shift
	}
	digitalWrite(10,HIGH);  //latch
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
/*
void RGBledPWM(unsigned char led,unsigned char color,unsigned char bright){


	if(bright==cmt1Count){
		RGBledControl(led,0);
	
	}else if(cmt1Count==0){
		RGBledControl(led,color);

	}
	
}*/
void RGBledUpdate(void){
	
	
	digitalWrite(5,0);//latch pin to low
	for(i=0;i<24;i++){
		digitalWrite(3,ledData[23-i]); //data
		digitalWrite(6,1); //shift
		digitalWrite(6,0); //shift
	}
	digitalWrite(5,1); //latch pin to high 
}

void ledRest(void){
	digitalWrite(3,0); //data
	digitalWrite(6,0); //shift
	digitalWrite(5,0); //storge
	digitalWrite(4,0); //output en 0==output enable
	digitalWrite(7,0); //rest low==rest
	digitalWrite(7,1);
	
}






unsigned char x;
void loop() {
  // put your main code here, to run repeatedly:
  
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
   RGBledControl(63,x);
  delay(1000);
}
