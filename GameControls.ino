#include<Wire.h> //Wire.h header file

// all pin assignments and variable declariation

const int MPU_addr=0x68;  // I2C address of gyroscope
int16_t GyX,GyY;
int buzzer = 4;
int dg = 0;
char ch;
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
int x;
int y;
int dj = 0;

void setup(){
  
  // pin mode initialization
  //enabling the gyroscope
  //enabling the serial monitor
  
  pinMode(buzzer, OUTPUT);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

// buzz() function activates buzzer for 60 ms

void buzz(){
   for(int i=0; i<10; i++){
    digitalWrite(buzzer, HIGH);
    delay(3);
    digitalWrite(buzzer, LOW);
    delay(3);
  }
}

// main loop() function

void loop(){
  
  // while() loop used in place of main loop() so that continue/pass statements can be used
  
  while(true){
    
    // begins transmission between the gyroscope and the arduino
    
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
    GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    
    //reading analog data at the analog input pins from the joystick
    
    x = analogRead(X_pin);
  
    y = analogRead(Y_pin);
    
    // polls for serial monitor outputs from corresponding python code
    
    if(Serial.available()){  
      ch = Serial.read();
    }
    
    // if 'E' flag then buzz
    
    if( ch == 'E' ){
      buzz();
      ch = 'O';
    }
    
    // if 'R' flag then allow any input
    
    if( ch == 'R' ){
      dg = 0;
      dj = 0;
      ch = 'O';
    }
    
    // conditional statements ignore inputs where threshold is reached in x and y direction
    
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
    
    // if last input was not up or down then read and print an up or down input
    
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
    
    // if last input was not left or right then read and print a left of right input
    
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
    
    // ignore inputs where thresholds are reached in both the x and y directions
    
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
    
    // if last input was not left or right then read and print left or right input
    
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
    
    // if last input was not up or down then read and print up or down input
    
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
