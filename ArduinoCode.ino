#include <TimerOne.h>
#include <Wire.h>
#include "HX711.h"
#define DOUT  A0
#define CLK  A1
#define proximity_Pin 2

HX711 scale(DOUT, CLK);

unsigned long jumlah = 0;
unsigned long oldJumlah = 0;

void counter() {
  jumlah = millis();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  scale.set_scale();
  scale.tare();
  pinMode(proximity_Pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(proximity_Pin), counter, FALLING);
}

void loop() {
  String minta = "";
  while(Serial.available()>0)
  {
    minta += char(Serial.read());
  }

  minta.trim();
  if(minta == "Ya")
  {
    hitung();
  }

  minta = "";
  delay(1000);
}

int count;
float selisih;
unsigned long old;
float TetesPerMenit;

float calibration_factor = 468.70;
int Ml;

void hitung () {
  unsigned long tunda = millis();
  if (tunda - old > 250) {
    old = tunda;
    if (oldJumlah != jumlah) {
      selisih = jumlah - oldJumlah;
      oldJumlah = jumlah;
    }
    
    count++;
    if (count % 4 == 0) {
      float waktPerTetes = selisih / 1000;
      float oldTetesPM;
      TetesPerMenit = 60 / waktPerTetes;
      if (TetesPerMenit < 300) {
        oldTetesPM = TetesPerMenit;
      } 
      else TetesPerMenit = oldTetesPM;
    }
  }
  scale.set_scale(calibration_factor);
  Ml = scale.get_units(), 4;
  String datakirim = String(TetesPerMenit) + "#" + String(Ml);
  Serial.println(datakirim);
}
