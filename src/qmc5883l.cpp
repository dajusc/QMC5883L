/*
Author:      David Schaefer -
Email:       github@schaeferdavid.de
Tested for:  Arduino Uno
License:     MIT
*/

#include "qmc5883l.h"
#include "Wire.h"
#include "math.h"

//QMC5883L::QMC5883L(void) {
//
//}

void QMC5883L::begin(bool enableAdjust=false) {
  Wire.begin();

  Wire.beginTransmission(QMC5883L_I2C_ADDRESS);
  Wire.write(QMC5883L_REG_CTRL1);
  Wire.write(QMC5883L_CFG_DEFAULT);
  Wire.endTransmission();

  Wire.beginTransmission(QMC5883L_I2C_ADDRESS);
  Wire.write(QMC5883L_REG_PERIOD);
  Wire.write(QMC5883L_CFG_PERIOD);
  Wire.endTransmission();

  this->enableAdjust = enableAdjust;
}

void QMC5883L::doAdjust(int &x, int &y, int &z) {
  if (!this->enableAdjust) return;

  if (x < this->hxyzminmax[0]) this->hxyzminmax[0] = x;
  if (x > this->hxyzminmax[1]) this->hxyzminmax[1] = x;
  if (y < this->hxyzminmax[2]) this->hxyzminmax[2] = y;
  if (y > this->hxyzminmax[3]) this->hxyzminmax[3] = y;
  if (z < this->hxyzminmax[4]) this->hxyzminmax[4] = z;
  if (z > this->hxyzminmax[5]) this->hxyzminmax[5] = z;

  x -= (this->hxyzminmax[0] + this->hxyzminmax[1]) / 2;
  y -= (this->hxyzminmax[2] + this->hxyzminmax[3]) / 2;
  z -= (this->hxyzminmax[4] + this->hxyzminmax[5]) / 2;
}

bool QMC5883L::getMagnetfieldSingle(int &x, int &y, int &z) {
  Wire.beginTransmission(QMC5883L_I2C_ADDRESS);
  Wire.write(QMC5883L_REG_DOUTLSB);
  Wire.endTransmission();

  Wire.requestFrom(QMC5883L_I2C_ADDRESS, 6);
  if (Wire.available() != 6) return false;

  x  = Wire.read();      // LSB x
  x |= Wire.read() << 8; // MSB x
  y  = Wire.read();      // LSB y
  y |= Wire.read() << 8; // MSB y
  z  = Wire.read();      // LSB z
  z |= Wire.read() << 8; // MSB z

  this->doAdjust(x, y, z);

  return true;
}

bool QMC5883L::getTemperatureSingle(int &t) {
  Wire.beginTransmission(QMC5883L_I2C_ADDRESS);
  Wire.write(QMC5883L_REG_TEMP);
  Wire.endTransmission();

  Wire.requestFrom(QMC5883L_I2C_ADDRESS, 2);
  if (Wire.available() == 2) {
    t  = Wire.read();      // LSB t
    t |= Wire.read() << 8; // MSB t
    return false;
  }

  return true;
}

bool QMC5883L::getMagnetfield(int &x, int &y, int &z, int navg=1) {
  x = 0;  y = 0;  z = 0;

  for (int i=0; i<navg; i++) {
    int xt, yt, zt;
    if (this->getMagnetfieldSingle(xt, yt, zt)) {
      x += xt;  y += yt;  z += zt;
    } else {
      x = 0;  y = 0;  z = 0;
      return false;
    }
  }

  x /= navg;   y /= navg;  z /= navg;
  return true;
}

bool QMC5883L::getTemperature(int &t, int navg=1) {
  t = 0;

  for (int i=0; i<navg; i++) {
    int tt;
    if (this->getTemperatureSingle(tt)) {
      t += tt;
    } else {
      t = 0;
      return false;
    }
  }

  t /= navg;
  return true;
}

bool QMC5883L::getDirectionXY(double &a, int navg=1) {
  int x, y, z;
  if (this->getMagnetfield(x, y, z, navg)) {
    a = atan2(-y, x) * 180.0 / 3.141 + 180.0;
    return true;
  } else {
    a = 0.0;
    return false;
  }
}
