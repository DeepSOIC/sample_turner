#define PIN_STEP 9 //do not change! tied to OCR1A
#define PIN_DIR 8
#define PIN_SLP 7
float lnfmin;
float lnfmax;

#include <math.h>

void setup() {
  Serial.begin(9600);//for debug output, if needed

  digitalWrite(PIN_STEP, LOW);
  pinMode(PIN_STEP, OUTPUT);
  digitalWrite(PIN_DIR, LOW);
  pinMode(PIN_DIR, OUTPUT);
  digitalWrite(PIN_SLP, LOW);
  pinMode(PIN_SLP, OUTPUT);

  lnfmin = log(10);
  lnfmax = log(60000);

  //set up timer1 into mode 14 (fast pwm with top=ICR1)
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
  unsigned int iperiod = 65535; //the new timer period (to be computed)

  //determine the best prescaler (pick the lowest 
  //division where period fits into 16 bits of counting)
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

  //wait for the timer counter value to be pretty low
  //to apply the new prescaler and top, 
  //so that the timer doesn't miss the new top
  while(TCNT1>4){}; //fixme: revise? 4 is pretty low...
  
  TCCR1B=0b00011000 | prescaler;
  ICR1 = iperiod;
  
  OCR1A=10;//sets pulse width
}
