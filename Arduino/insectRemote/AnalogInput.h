#include <Arduino.h>
union U {
  int16_t s; 
  //   vs.
  struct Byte {
    int8_t c1, c2;  
  }
  byte;
};


class AnalogInput
{
  public:
    AnalogInput(String name, int pin, int dir)
    {
      mName = name;
      mPin = pin;
      mDir = dir;
    }
    void update()
    {
     int val = (analogRead(mPin)-mCenter)* mDir;
     
      if( val>0)
      {
         val =max(0, val-100);
      }else
      {
         val =min(0, val+100);
      }
      if(val>1800)val =1800;
      if(val<-1800)val =-1800;
      endVal.s  =val*1000.f/1800.f ;
 
    }
    void setCenter()
    {
      mCenter = analogRead(mPin);
    }
    void serialLog()
    {
      Serial.print(  mName);
      Serial.print(":");
      Serial.print(endVal.s);
      Serial.print(" ");
    }
    U endVal;
    String mName;
    int mPin;
    int mDir = 0;
    int mCenter =512;
    

};
