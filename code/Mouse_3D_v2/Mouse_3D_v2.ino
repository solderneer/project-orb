#include<Wire.h>
const int MPU=0x68;  // I2C address of the MPU-6050
int16_t GyX, GyY, GyZ, AcX, AcY, AcZ, BAcX, BAcY, BAcZ, TAcX, TAcY, TAcZ;
int buttonPinLeft = 5;
int buttonPinRight = 7;
int buttonGyro = 6;
int buttonAccel = 4;
int buttonState = 0;
int mousePress = 0;
int mouseRelease = 0;
int gyroLock = 0;
int AccelLock = 0;
int AccelLockToggle = 0;
int AccelLockSide = 0;
int GyroLockToggle = 0;
int GyroLockSide = 0;
int i = 0;
int i2 = 0;
int w = 0, a = 0, j = 0;
int threshold = 20;
int X, Y, Z;
int minX, maxX, minY, maxY, minZ, maxZ;
int accelLock = 0;
int TGY = 0;
void setup(){
  pinMode(buttonPinLeft, INPUT);
  digitalWrite(buttonPinLeft, HIGH);
  pinMode(buttonPinRight, INPUT);
  digitalWrite(buttonPinRight, HIGH);
  pinMode(buttonGyro, INPUT);
  digitalWrite(buttonGyro, HIGH);
  pinMode(buttonAccel, INPUT);
  digitalWrite(buttonAccel, HIGH);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  Serial.begin(9600);
  Mouse.begin();
}
void loop(){
  gyroLock = digitalRead(buttonGyro);
  if (gyroLock == LOW) {
    if (GyroLockSide == 0) {
      GyroLockSide = 1;
      if (GyroLockToggle == 0) {
        GyroLockToggle = 1;
      }
      else {
        GyroLockToggle = 0;
      }
    }
  }
  else {
    if (GyroLockSide == 1) {
      GyroLockSide = 0;
    }
  }
  if (GyroLockToggle == 0) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);  // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU,6,true);// request a total of 14 registers
    GyX = Wire.read()<<8|Wire.read();
    GyY = Wire.read()<<8|Wire.read();
    GyZ = Wire.read()<<8|Wire.read();// 0x48
    if (AccelLockToggle == 1) {
      Mouse.move( GyZ / 500, -GyX / 500, 0 );
    }
    else {
      Mouse.move( GyZ / 1000, -GyX / 1000, 0 );
    }
    TGY -= GyY / 500;
    if (AccelLockToggle == 1) {
      if (TGY > 500) {
        Keyboard.write('d');
      }
      else if (TGY < -500) {
        Keyboard.write('a');
      }
    }
    else {
      TGY = 0;
    }
    if (GyX > 2000 || GyY > 2000 || GyZ > 2000) {
      accelLock = 0;
    }
    else {
      accelLock = 1;
    }
  }
  AccelLock = digitalRead(buttonAccel);
  if (AccelLock == LOW) {
    if (AccelLockSide == 0) {
      AccelLockSide = 1;
      if (AccelLockToggle == 0) {
        AccelLockToggle = 1;
      }
      else {
        AccelLockToggle = 0;
      }
    }
  }
  else {
    if (AccelLockSide == 1) {
      AccelLockSide = 0;
    }
  }
  if (AccelLockToggle == 1) {
    if (true) {
      if (i == 0) {
        Wire.beginTransmission(MPU);
        Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
        Wire.endTransmission(false);
        Wire.requestFrom(MPU, 6, true);  // request a total of 14 registers
        BAcX=Wire.read()<<8|Wire.read();// 0x3B     
        BAcY=Wire.read()<<8|Wire.read();
        BAcZ=Wire.read()<<8|Wire.read();// 0x40
        i = 1;
        minX = BAcX;
        maxX = BAcX;
        minY = BAcY;
        maxY = BAcY;
        minZ = BAcZ;
        maxZ = BAcZ;
        BAcX /= 500;
        BAcY /= 500;
        BAcZ /= 500;
      }
      Wire.beginTransmission(MPU);
      Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU, 6, true);  // request a total of 14 registers
      AcX=Wire.read()<<8|Wire.read();// 0x3B     
      AcY=Wire.read()<<8|Wire.read();
      AcZ=Wire.read()<<8|Wire.read();// 0x40
      if (AcX < minX) {
        minX = AcX;
      }
      else if (AcX > maxX) {
        maxX = AcX;
      }
      if (AcY < minY) {
        minY = AcY;
      }
      else if (AcY > maxY) {
        maxY = AcY;
      }
      if (AcZ < minZ) {
        minZ = AcZ;
      }
      else if (AcZ > maxZ) {
        maxZ = AcZ;
      }
//      for (i2 = 0; i2 < 20; i2++) {
//        Serial.println(" ");
//      }
//      Serial.print("Min-X: ");
//      Serial.print(minX);
//      Serial.print(" | Max-X: ");
//      Serial.print(maxX);
      AcX /= 500;
      TAcX = AcX;
      AcX -= BAcX;
      BAcX = TAcX;
      AcY /= 500;
      TAcY = AcY;
      AcY -= BAcY;
      BAcY = TAcY;
      AcZ /= 500;
      TAcZ = AcZ;
      AcZ -= BAcZ;
      BAcZ = TAcZ;
      Y = -AcX;
      X = -AcY;
      Z = AcZ;
      Y = 0;
      //Serial.print(X);Serial.print(" ");
      //Serial.print(Y);Serial.print(" ");
      //Serial.println(Z);
      if (X > threshold) {
        if (w == 0) {
          w = 1;
        }
        else if (w == -1) {
          w = -2;
        }
        else if (w == 4) {
          w = 0;
        }
        else if (w == -3) {
          w = -4;
        }
      }
      else if (X < -threshold) {
        if (w == 1) {
          w = 2;
        }
        else if (w == 0) {
          w = -1;
        }
        else if (w == 3) {
          w = 4;
        }
        else if (w == -4) {
          w = 0;
        }
      }
      else {
        if (w == 2) {
          w = 3;
        }
        else if (w == -2) {
          w = -3;
        }
      }
      if (Y > threshold) {
        if (a == 0) {
          a = 1;
        }
        else if (a == -1) {
          a = -2;
        }
        else if (a == 4) {
          a = 0;
        }
        else if (a == -3) {
          a = -4;
        }
      }
      else if (Y < -threshold) {
        if (a == 1) {
          a = 2;
        }
        else if (a == 0) {
          a = -1;
        }
        else if (a == 3) {
          a = 4;
        }
        else if (a == -4) {
          a = 0;
        }
      }
      else {
        if (a == 2) {
          a = 3;
        }
        else if (a == -2) {
          a = -3;
        }
      }
      if (Z > threshold) {
        if (j == 0) {
          j = 1;
        }
        else if (j == -1) {
          j = -2;
        }
        else if (j == 4) {
          j = 0;
        }
        else if (j == -3) {
          j = -4;
        }
      }
      else if (Z < -threshold) {
        if (j == 1) {
          j = 2;
        }
        else if (j == 0) {
          j = -1;
        }
        else if (j == 3) {
          j = 4;
        }
        else if (j == -4) {
          j = 0;
        }
      }
      else {
        if (j == 2) {
          j = 3;
        }
        else if (j == -2) {
          j = -3;
        }
      }
    }
    //Serial.print(w);Serial.print(" ");
    if (w == 3) {
      Keyboard.press('w');
      Keyboard.release('w');
    }
    if (w == -3) {
      Keyboard.press('s');
      Keyboard.release('s');
    }
    if (a == 3) {
      Keyboard.press('a');
      Keyboard.release('a');
    }
    if (a == -3) {
      Keyboard.press('d');
      Keyboard.release('d');
    }
    if (j == 3) {
      Keyboard.press(' ');
      Keyboard.release(' ');
    }
    if (j == -3) {
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.release(KEY_LEFT_CTRL);
    }
  }
  else {
    i = 0;
    w = 0;
    a = 0;
    j = 0;
  }
  buttonState = digitalRead(buttonPinLeft);
  if (buttonState == LOW) {     
    //Serial.print("Button State: ON");
    mousePress = 1;
  } 
  if (buttonState == HIGH) {
    //Serial.print("Button State: OFF");
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
    //Serial.print("Button State: ON");
    mousePress = 1;
  } 
  if (buttonState == HIGH) {
    //Serial.print("Button State: OFF");
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
  delay(1);
}
