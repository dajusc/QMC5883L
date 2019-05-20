/*
Author:      David Schaefer -
Email:       github@schaeferdavid.de
Tested for:  Arduino Uno
License:     MIT
*/

#include <Arduino.h>
#include "Wire.h"
#include "math.h"

#ifndef QMC5883L_H
#define QMC5883L_H

// I2C ADDRESS

#define QMC5883L_I2C_ADDRESS   0x0D

// REGISTER ADRESSES

#define QMC5883L_REG_CTRL1     0x09  // Control Register 1
#define QMC5883L_REG_PERIOD    0x0B  // SET/RESET Period Register
#define QMC5883L_REG_DOUTLSB   0x00  // Data Output X LSB Register
#define QMC5883L_REG_TEMP      0x07  // Temperature Sensor Output

// CONFIGS

#define QMC5883L_CFG_DEFAULT   0x1D // 0x1D --> 00 01 11 01 --> OSR = 512, RNG = 8G, ODR = 200Hz, Mode = Continuous
#define QMC5883L_CFG_PERIOD    0x01 // Value recommended in datasheet

class QMC5883L {

  private:
    bool enableAdjust  = false;
    int16_t hxyzminmax[6] = {+9999,-9999,+9999,-9999,+9999,-9999};
    void doAdjust(int16_t &x, int16_t &y, int16_t &z);
    bool getMagnetfieldSingle(int16_t &x, int16_t &y, int16_t &z);
    bool getTemperatureSingle(int16_t &t);

  public:
    //QMC5883L(void);
    void begin(bool enableAdjust);
    void begin(const int16_t x, const int16_t y, const int16_t z);
    bool getMagnetfield(int16_t &x, int16_t &y, int16_t &z, int navg);
    bool getDirectionXY(double &a, int navg);
    bool getTemperature(int16_t &t, int navg);
    bool getAdjustment(int16_t &x, int16_t &y, int16_t &z);
};

#endif
