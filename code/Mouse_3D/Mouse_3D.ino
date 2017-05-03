#include<Wire.h>
const int MPU=0x68;  // I2C address of the MPU-6050
int16_t GyX, GyY, GyZ;
int buttonPinLeft = 4;
int buttonPinRight = 5;
int buttonState = 0;
int mousePress = 0;
int mouseRelease = 0;
void setup(){
  pinMode(buttonPinLeft, INPUT);
  digitalWrite(buttonPinLeft, HIGH);
  pinMode(buttonPinRight, INPUT);
  digitalWrite(buttonPinRight, HIGH);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  Mouse.begin();
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x43);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,6,true);// request a total of 14 registers
  GyX = Wire.read()<<8|Wire.read();
  GyY = Wire.read()<<8|Wire.read();
  GyZ = Wire.read()<<8|Wire.read();// 0x48
  buttonState = digitalRead(buttonPinLeft);
  if (buttonState == LOW) {     
    Serial.print("Button State: ON");
    mousePress = 1;
  } 
  if (buttonState == HIGH) {
    Serial.print("Button State: OFF");
    mousePress = 1;
    mouseRelease = 1;
  }
  if ( mousePress == 1 ) {
    if (mouseRelease == 1) {
      Mouse.release(MOUSE_LEFT);
      mouseRelease = 0;
    }
    else {
      Mouse.press(MOUSE_LEFT);
    }
    mousePress = 0;
  }
  buttonState = digitalRead(buttonPinRight);
  if (buttonState == LOW) {     
    Serial.print("Button State: ON");
    mousePress = 1;
  } 
  if (buttonState == HIGH) {
    Serial.print("Button State: OFF");
    mousePress = 1;
    mouseRelease = 1;
  }
  if ( mousePress == 1 ) {
    if (mouseRelease == 1) {
      Mouse.release(MOUSE_RIGHT);
      mouseRelease = 0;
    }
    else {
      Mouse.press(MOUSE_RIGHT);
    }
    mousePress = 0;
  }
  Serial.print(" | GyX = "); Serial.print(GyX / 400);
  Serial.println(" | GyY = "); Serial.println(GyZ / 400);
  Mouse.move( GyZ / 1000, -GyX / 1000, 0 );
  delay(1);
}
