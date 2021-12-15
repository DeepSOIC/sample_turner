#define PIN_STEP 9
#define PIN_DIR 8
#define PIN_SLP 7
float lnfmin;
float lnfmax;

#include <math.h>

void setup() {
  // put your setup code here, to run once:
  digitalWrite(PIN_STEP, LOW);
  pinMode(PIN_STEP, OUTPUT);
  digitalWrite(PIN_DIR, LOW);
  pinMode(PIN_DIR, OUTPUT);
  digitalWrite(PIN_SLP, LOW);
  pinMode(PIN_SLP, OUTPUT);

  lnfmin = log(10);
  lnfmax = log(60000);
  Serial.begin(9600);

  //set up timer1
  TCCR1A=0b10000010;
  TCCR1B=0b00011001;
  ICR1 = 4096;
  OCR1A = 10;
}

void loop() {
  float fcpu=16e6;
  float pot = analogRead(A0)/1024.0;
  float f = exp(lnfmin + (lnfmax-lnfmin)*pot);
  //Serial.println(f);
  float period = 1.0/f;
  float period_cyc = period*fcpu;
  unsigned int iperiod = 65535;
  byte prescaler = 0;
  if(period_cyc>65535.0*256.0){
    prescaler = 0b00000101;
    iperiod = round(period_cyc/1024);
  } 
  else if(period_cyc>65535.0*64.0){
    prescaler = 0b00000100;
    iperiod = round(period_cyc/256);
  }
  else if(period_cyc>65535.0*8.0){
    prescaler = 0b00000011;
    iperiod = round(period_cyc/64);
  }
  else if(period_cyc>65535.0){
    prescaler = 0b00000010;
    iperiod = round(period_cyc/8);
  }
  else {
    prescaler = 0b00000001;
    iperiod = round(period_cyc);
  }

  //wait to apply prescaler
  while(TCNT1>4){};
  TCCR1B=0b00011000 | prescaler;
  ICR1 = iperiod;
  OCR1A=10;
}
