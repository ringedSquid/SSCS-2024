#include <avr/sleep.h>

#define DAC_OUT PIN_PA6
#define DISCHARGE PIN_PA5
#define CHARGE PIN_PA7
#define ADC_IN PIN_PA4

#define PTN_IN PIN_PA3
#define RED_LED PIN_PB1
#define GRN_LED PIN_PB0

#define AVERAGE_RES 10
double times[AVERAGE_RES] = {};
int time_i = 0;

void setup() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  DACReference(INTERNAL1V5);
  analogReference(INTERNAL2V5);
  
  //init pin modes
  pinMode(ADC_IN, INPUT);
  pinMode(PTN_IN, INPUT);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GRN_LED, OUTPUT);
  pinMode(DAC_OUT, OUTPUT);
  pinMode(CHARGE, OUTPUT);
  pinMode(DISCHARGE, OUTPUT);
  
  //init pin states
  digitalWrite(CHARGE, LOW);
  digitalWrite(DISCHARGE, LOW);
  analogWrite(DAC_OUT, 0);
}

void loop() {
  
}

void sleep() {
  ADC0.CTRLA &= ~ADC_ENABLE_bm;
  
}

double getRiseTime() {
  digitalWrite(DISCHARGE, HIGH);
  digitalWrite(CHARGE, LOW);
  analogWrite(DAC_OUT, 255);

  //Discharge cap
  while (readADC() > 15) {
  }

  long t0 = micros();
  digitalWrite(DISCHARGE, LOW);
  digitalWrite(CHARGE, HIGH);

  while (readADC() < 637) {
  }

  long delta_t = micros - t0;
  times[time_i] = delta_t;
  time_i++;
  time_i %= AVERAGE_RES;
  return getAvgTime();
}

double readADC() {
  double sum = 0;
  for (int i=0; i<5; i++) {
    sum += analogRead(ADC_IN);
  }
  return sum/5;
}

double getAvgTime() {
  double sum = 0;
  for (int i=0; i<AVERAGE_RES; i++)  {
    sum += times[i]
  }
  return sum/AVERAGE_RES;
}
