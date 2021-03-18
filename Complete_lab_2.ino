// MPU-6050 Short Example Sketch
//www.elegoo.com
//2016.12.9

#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t GyX,GyY;
int buzzer = 4;
int dg = 0;
char ch;
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
int x;
int y;
int dj = 0;

void setup(){
  pinMode(buzzer, OUTPUT);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  pinMode(SW_pin, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
}

void buzz(){
   for(int i=0; i<10; i++){
    digitalWrite(buzzer, HIGH);
    delay(3);
    digitalWrite(buzzer, LOW);
    delay(3);
  }
}

void loop(){
  while(true){
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    
    x = analogRead(X_pin);
  
    y = analogRead(Y_pin);
    
    if(Serial.available()){  
      ch = Serial.read();
    }
    if( ch == 'E' ){
      buzz();
      ch = 'O';
    }
    if( ch == 'R' ){
      dg = 0;
      dj = 0;
      ch = 'O';
    }
    if( GyX > 20000 && GyY > 20000 ){
      continue;
    }
    if( GyX > 20000 && GyY < -20000 ){
      continue;
    }
    if( GyX < -20000 && GyY > 20000 ){
      continue;
    }
    if( GyX < -20000 && GyY < -20000 ){
      continue;
    }
    if( dg != 1 ){
      if( GyX > 20000 ){
        Serial.print("down\n");
        dg = 1;
      }
      if( GyX < -20000 ){
        Serial.print("up\n");
        dg = 1;
      }
    }
    if( dg != 2 ){
      if( GyY > 20000 ){
        Serial.print("right\n");
        dg = 2;
      }
      if( GyY < -20000 ){
        Serial.print("left\n");
        dg = 2;
      }
    }
    if( x == 1023 && y == 1023){
      continue;
    }
    if( x == 1023 && y == 0){
      continue;
    }
    if( x == 0 && y == 1023){
      continue;
    }
    if( x == 0 && y == 0){
      continue;
    }
    if(dj != 1){
       if(x == 1023){
           Serial.print("right\n");
           dj = 1;
       }
       if( x == 0 ){
         Serial.print("left\n");
         dj = 1;
       }
    }
    if(dj != 2 ){
      if( y == 1023 ){
         Serial.print("down\n");
         dj = 2;
      }
      if( y == 0 ){
         Serial.print("up\n");
         dj =2;
      }
    }
  }
}
