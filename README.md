# QMC5883L
An Arduino library for QMC5883L magnetsensor.

## Overview
This I2C library features:
- Configurable averaging to increase accuracy (OFF per default)
- Zero-value-adjustment for each axis considering min/max interval (OFF per default)
- Calculate sendor orientation/direction in XY-plane
- Get Temperature

## Usage
Basic example without adjustment:
```
#include <qmc5883l.h>
QMC5883L QMC;

int    x, y, z;
int    t;
double a;

void setup() {
  Serial.begin(115200);
  QMC.begin();
}

void loop() {
  QMC.getMagnetfield(x, y, z);
  Serial.print("H = (");
  Serial.print(x); Serial.print(", ");
  Serial.print(y); Serial.print(", ");
  Serial.print(z); Serial.print(")  \t");

  QMC.getDirectionXY(a);
  Serial.print("Dir = ");
  Serial.print(a, 1);
  Serial.print(" \t");

  QMC.getTemperature(t);
  Serial.print("T = ");
  Serial.print(t / 100.0 + 34.0);
  Serial.print(" \t");

  Serial.println("");
  delay(500);
}
```
Basic example with averaging:
```
#include <qmc5883l.h>
QMC5883L QMC;

int    x, y, z;
int    t;
double a;

void setup() {
  Serial.begin(115200);
  QMC.begin();
}

void loop() {
  QMC.getMagnetfield(x, y, z, 5); // <-- Averagig over 5 seperate sensor readings
  Serial.print("H = (");
  Serial.print(x); Serial.print(", ");
  Serial.print(y); Serial.print(", ");
  Serial.print(z); Serial.print(")  \t");

  Serial.println("");
  delay(500);
}
```
Basic example with zero-value-adjustment:
```
#include <qmc5883l.h>
QMC5883L QMC;

int    x, y, z;
int    t;
double a;

void setup() {
  Serial.begin(115200);
  QMC.begin(true); // true --> enable zero-value-adjustment
}

void loop() {
  QMC.getMagnetfield(x, y, z);
  Serial.print("H = (");
  Serial.print(x); Serial.print(", ");
  Serial.print(y); Serial.print(", ");
  Serial.print(z); Serial.print(")  \t");

  Serial.println("");
  delay(500);
}
```
