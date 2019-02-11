/* 
 *  Control Rotel Amp with Panasonic TV Remote
 *  Panasonic FXW784 - Rotel RA-12
 */

#include <IRremote.h>

#include "PanasonicIrCodes.h"
#define MODELED 13

int RECV_PIN = 8;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
     Serial.begin(115200,SERIAL_8N1);
     irrecv.enableIRIn();
}

boolean muted=false;

void mute() {
  muted=true;
  Serial.print("mute_on!");
}
void volup() {
  if (muted) Serial.print("mute_off!");
  muted=false;
  Serial.print("volume_up!");
}

boolean amprunning=false;

void ampon()
{
  if (amprunning) return;
  Serial.print("power_on!");
  amprunning=true;
}

void ampoff()
{
  if (!amprunning) return;
  Serial.print("power_off!");
  amprunning=false;
}

boolean amp_mode=false;
boolean attention_mode=false;

void amp_mode_on()
{
    amp_mode=true;
    digitalWrite(MODELED,HIGH);
}

void amp_mode_off()
{
    amp_mode=false;
    digitalWrite(MODELED,LOW);
}

void loop() {
  unsigned long code;

  if (irrecv.decode(&results))
  {
    if (results.decode_type == PANASONIC)
    {
      code=results.value;
      if (code==PanasonicVOLPLUS) { ampon(); volup(); }
      if (code==PanasonicVOLMINUS) Serial.print("volume_down!");
      if (code==PanasonicMUTE) mute();
      if (code==PanasonicPOWER) ampoff(); 
      if (code==PanasonicPRGPLUS) ampon();
      if (code==PanasonicDIGIT1) ampon();
      if (amp_mode) 
      {
        if (code==PanasonicRED) { Serial.print("aux2!"); }
        else if (code==PanasonicGREEN) { Serial.print("opt1!"); }
        else if (code==PanasonicYELLOW) { Serial.print("opt2!"); }
        else if (code==PanasonicBLUE) { Serial.print("coax1!"); }
	      else amp_mode_off();
      }
      else if (code==PanasonicGREEN) // press green twice for mode switch
      { 
        if (attention_mode) 
        {
          amp_mode_on();
          attention_mode=false;
        }
        else 
        {
          delay(100); 
          attention_mode=true; 
        }
      } 
      else attention_mode=false;
    }
    delay(100);
    irrecv.resume();
  }
}
