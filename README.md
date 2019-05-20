# QMC5883L
An Arduino library for the QMC5883L magnetsensor used in GY-271 boards.

## Feature Overview
- Averaging to reduce noise *(disabled per default)*
- Zero-value-adjustment for each axis *(disabled per default)*
- Calculate sensor orientation/direction in XY-plane
- Read Temperature
- I2C interface

## Basic example
```cpp
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
## Example with averaging
In the following example five sequential sensor readings are executed and then the average value is returned.
```cpp
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
  QMC.getMagnetfield(x, y, z, 5); // <-- Averagig over 5 sensor readings
  Serial.print("H = (");
  Serial.print(x); Serial.print(", ");
  Serial.print(y); Serial.print(", ");
  Serial.print(z); Serial.print(")  \t");

  Serial.println("");
  delay(500);
}
```
## Example with automatic zero-value-adjustment
In the following example the automatic zero-value-adjustment is enabled.
The adjustment is done by keeping track of the min/max interval of the raw sensor readings for each axis (Hx, Hy, Hz).
Any unsymetry is then compensated by adding the corresponding linear offset to the returned values.
The resulting adjustment vector is printed out at the end of the example.

*PLEASE NOTE*: The sensor needs to be "calibrated" by rotating it around all axis.
Before that, the returned values will be considerably worse than in the non-adjusted mode.
```cpp
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

  QMC.getAdjustment(x, y, z);
  Serial.print("Ha = (");
  Serial.print(x); Serial.print(", ");
  Serial.print(y); Serial.print(", ");
  Serial.print(z); Serial.print(")  \t");

  Serial.println("");
  delay(500);
}
```
## Example with manual zero-value-adjustment
In the following example the manual zero-value-adjustment is enabled.
```cpp
#include <qmc5883l.h>
QMC5883L QMC;

int    x, y, z;
int    t;
double a;

void setup() {
  Serial.begin(115200);
  QMC.begin(120, +540, -590);
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
