#ifndef __PINS_STATE_MANAGER_H
#define __PINS_STATE_MANAGER_H
#include <Arduino.h>
#include "Config.h"

#define D_IN_1  14
#define D_IN_2  12
#define D_OUT_1 15

//if D_IN_MODE is HIGH -> MQTT mode starts, else HTTP mode starts
#define D_IN_MODE  13

const int A_IN = A0;  // ESP8266 Analog Pin ADC0 = A0

class PinsStateManager
{
  public:
    PinsStateManager(){}
    ~PinsStateManager(){}
    void begin()
    {
      pinMode(D_IN_1, INPUT_PULLUP);
      pinMode(D_IN_2, INPUT_PULLUP);
      pinMode(D_IN_MODE, INPUT_PULLUP);
      pinMode(D_OUT_1, OUTPUT);
      digitalWrite(D_OUT_1, LOW);

      state_A_IN = analogRead(A_IN)*3.3F/1024.0F;
      state_D_IN_1 = digitalRead(D_IN_1);
      state_D_IN_2 = digitalRead(D_IN_2);
      state_D_IN_Mode = digitalRead(D_IN_MODE);
    }

    void loop()
    {  
      state_A_IN = analogRead(A_IN)*3.3F/1024.0F;
      state_D_IN_1 = digitalRead(D_IN_1);
      state_D_IN_2 = digitalRead(D_IN_2);
      //state_D_IN_Mode = digitalRead(D_IN_MODE);
      //if(0)digitalWrite(D_OUT_1, HIGH);
    }

    float getAINState(){return state_A_IN;}
    uint8_t getDIN1State(){return state_D_IN_1;}
    uint8_t getDIN2State(){return state_D_IN_2;}
    uint8_t getDINModeState(){return state_D_IN_Mode;}
    void setDOUT1State(uint8_t state)
    {
      if(state)digitalWrite(D_OUT_1, HIGH);
      else digitalWrite(D_OUT_1, LOW);
    }
    uint8_t getDOUT1State(){return digitalRead(D_OUT_1);}
  private:
    
    float state_A_IN = 0;
    uint8_t state_D_IN_1 =0;
    uint8_t state_D_IN_2 =0;
    uint8_t state_D_IN_Mode =0;
};


#endif
