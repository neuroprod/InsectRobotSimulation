#include "AnalogInput.h"

//#define LOGSERIAL
#define SERIALDATA
AnalogInput lx("lx", A1, 1);
AnalogInput ly("ly", A0, -1);
AnalogInput lz("lz", A2, 1);

AnalogInput rx("rx", A4, 1);
AnalogInput ry("ry", A3, -1);
AnalogInput rz("rz", A5, 1);
AnalogInput inputs[] = {lx, ly, lz, rx, ry, rz};
 byte output[18];
void setup() {

  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  delay(500);
  for (int i = 0; i < 6; i++)
  {
    inputs[i].setCenter();
  }

}

void loop() {

  for (int i = 0; i < 6; i++)
  {
    inputs[i].update();
  }
#ifdef LOGSERIAL
  for (int i = 0; i < 6; i++)
  {
    inputs[i].serialLog();
  }
  Serial.print(" ");

  Serial.print(  !digitalRead(8));

  Serial.print(" ");

  Serial.print(  !digitalRead(9));
  Serial.println("");
#endif
#ifdef SERIALDATA
  int count =0;
  Serial.write(100);
  Serial.write(100);
  for (int i = 0; i < 6; i++)
  {
    Serial.write(inputs[i].endVal.byte.c1);
    Serial.write(inputs[i].endVal.byte.c2);
  }
  Serial.write(!digitalRead(8));
  Serial.write(!digitalRead(9));



   
#endif

  delay(32);
}
