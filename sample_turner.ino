#define PIN_STEP 9
#define PIN_DIR 8
#define PIN_SLP 7
float lnfmin;
float lnfmax;

#include "GyverTimers.h"
#include <math.h>

void setup() {
  // put your setup code here, to run once:
  digitalWrite(PIN_STEP, LOW);
  pinMode(PIN_STEP, OUTPUT);
  digitalWrite(PIN_DIR, LOW);
  pinMode(PIN_DIR, OUTPUT);
  digitalWrite(PIN_SLP, LOW);
  pinMode(PIN_SLP, OUTPUT);
  Timer1.setFrequencyFloat(100.0);
  Timer1.outputEnable(CHANNEL_A, TOGGLE_PIN);

  lnfmin = log(10);
  lnfmax = log(60000);
  Serial.begin(9600);
}

void loop() {
  float pot = analogRead(A0)/1024.0;
  float f = exp(lnfmin + (lnfmax-lnfmin)*pot);
  Serial.println(f);
  while(TCNT1>4){};
  Timer1.setFrequencyFloat(f);
}
