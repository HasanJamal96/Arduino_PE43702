#include <SPI.h>
#include <Wire.h>
#include "config.h"
#include <oled.h>
#include "RotaryEncoder.h"


RotaryEncoder encoder(OUT_A, OUT_B, RotaryEncoder::LatchMode::TWO03);
OLED display(A4,A5,-1,OLED::W_128,OLED::H_32,OLED::CTRL_SH1106,0x3C); //CTRL_SSD1306


void checkPosition() {
  static int pos = 0;
  encoder.tick();
  int newPos = encoder.getPosition();
  if (pos != newPos) {
    if(newPos > pos){
      CurrentAttanuation += ATTAN_STEP;
      if(CurrentAttanuation > ATTAN_MAX) {
        CurrentAttanuation = ATTAN_MAX;
      }
    }
    else{
      CurrentAttanuation -= ATTAN_STEP;
      if(CurrentAttanuation < ATTAN_MIN) {
        CurrentAttanuation = ATTAN_MIN;
      }
    }
    /*#ifdef DEBUGGING
      Serial.print("Current Attanuation: ");
      Serial.println(CurrentAttanuation);
      Serial.print("Set Attanuation: ");
      Serial.println(SetAttanuation);
    #endif*/
    if(LastAttanuation != CurrentAttanuation) {
      LastAttanuation = CurrentAttanuation;
      display.clear();
      updateDisplay(35, 0, 1, "Attanuation");
      updateAttanDisplay();
       #ifdef DEBUGGING
         Serial.print("Current Attanuation: ");
         Serial.println(CurrentAttanuation);
       #endif
    }
    pos = newPos;
  }
}



void setup() {
  #ifdef DEBUGGING
    Serial.begin(BAUDRATE);
    Serial.println("[Main] Setup start");
  #endif

  attachInterrupt(digitalPinToInterrupt(OUT_A), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(OUT_B), checkPosition, CHANGE);
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(ATT1_SS_PIN, OUTPUT);
  pinMode(ATT2_SS_PIN, OUTPUT);

  display.begin();
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  
  isDisplay = true;
  display.clear();
  updateDisplay(35, 0, 1, "Attanuation");
  updateDisplay(40, 20, 1, "Adjuster");
  delay(3000);
  display.clear();
  updateDisplay(35, 0, 1, "Attanuation");
  updateAttanDisplay();
    


  #ifdef DEBUGGING
    Serial.println("[Main] Setup complete");
  #endif
}

void loop() {
  if(digitalRead(SWITCH) && SwitchReleased) {
    SwitchReleased = false;
    SetAttanuation = CurrentAttanuation;
    #ifdef DEBUGGING
      Serial.print("Set Attanuation: ");
      Serial.println(SetAttanuation);
    #endif
    display.clear();
    updateDisplay(35, 0, 1, "Attanuation");
    updateAttanDisplay();
    sendAttanuation();
  }
  else if(!digitalRead(SWITCH) && !SwitchReleased) {
    SwitchReleased = true;
  }
}

void updateDisplay(int x, int y, uint8_t ts, String Text){ // x location, y location, text size, text to display
  char Buf[50];
  Text.toCharArray(Buf, 50);
  display.draw_string(x,y,Buf,OLED::NORMAL_SIZE,OLED::WHITE);
  display.display();
}


void updateAttanDisplay(){
  String CA = "Current: " + String(CurrentAttanuation);
  String SA = "Set: " + String(SetAttanuation);
  updateDisplay(10,10,1,CA);
  updateDisplay(10,25,1,SA);
}


void sendAttanuation() {
  digitalWrite(ATT1_SS_PIN, HIGH);
  delay(10);
  SPI.transfer(SetAttanuation*4);
  digitalWrite(ATT1_SS_PIN, LOW);
  delay(10);
  digitalWrite(ATT2_SS_PIN, HIGH);
  delay(10);
  SPI.transfer(SetAttanuation*4);
  digitalWrite(ATT2_SS_PIN, LOW);
}
