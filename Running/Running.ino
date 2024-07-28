#include <avr/sleep.h>

//Pinout
#define DAC_OUT PIN_PA6
#define DISCHARGE PIN_PA5
#define CHARGE PIN_PA7
#define ADC_IN PIN_PA4

#define PTN_IN PIN_PA3
#define RED_LED PIN_PB1
#define GRN_LED PIN_PB0

#define AVERAGE_RES 10

//Coefficients for linear model
#define M 117.1875014
#define B -101.9550907

//user defined moisture ranges (acceptable range)
#define RL 0.6
#define RH 0.7

void setup() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  DACReference(INTERNAL1V5);
  analogReference(INTERNAL2V5);

  //init used pin modes
  pinMode(ADC_IN, INPUT);
  pinMode(PTN_IN, INPUT);

  pinMode(RED_LED, OUTPUT);
  pinMode(GRN_LED, OUTPUT);
  pinMode(DAC_OUT, OUTPUT);
  pinMode(CHARGE, OUTPUT);
  pinMode(DISCHARGE, OUTPUT);

  //init other pins
  pinMode(PIN_PB3, OUTPUT);
  pinMode(PIN_PB2, OUTPUT);
  pinMode(PIN_PA2, OUTPUT);
  pinMode(PIN_PA1, OUTPUT);
  pinMode(PIN_PA0, OUTPUT);

  //init pin states
  digitalWrite(CHARGE, LOW);
  digitalWrite(DISCHARGE, LOW);
  digitalWrite(GRN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  analogWrite(DAC_OUT, 0);

}

void loop() {
  double moistureLevel = getMoistureLevel(getRiseTime());
  if (moistureLevel <= 0.6) {
    digitalWrite(RED_LED, HIGH);
  }
  else if (moistureLevel >= 0.6) {
    digitalWrite(GRN_LED, HIGH);
    digitalWrite(RED_LED, HIGH);
  }
  else {
    digitalWrite(GRN_LED, HIGH);
  }
  delay(5000);
  sleep();
}

void sleep() {
  digitalWrite(DISCHARGE, LOW);
  digitalWrite(CHARGE, LOW);
  digitalWrite(GRN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  analogWrite(DAC_OUT, 0);
  //Disable ADC
  ADC0.CTRLA &= ~ADC_ENABLE_bm;
  //Interrupt on rising
  PORTA.PIN3CTRL = 0b00000010;
  sleep_cpu();
}

ISR(PORTA_PORT_vect) {
  //Clear flags
  PORTA.INTFLAGS = 1;
  //Disable interrupt
  PORTA.PIN3CTRL &= ~(0b00000111);
  //enable ADC
  ADC0.CTRLA |= ADC_ENABLE_bm;
}

double getMoistureLevel(double x) {
  return M * x + B;
}

double getRiseTime() {
  double tSum = 0;
  for (int i = 0; i < AVERAGE_RES; i++) {
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
    tSum += delta_t;
  }
  return (tSum / AVERAGE_RES) * pow(10, -6);
}

double readADC() {
  double sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(ADC_IN);
  }
  return sum / 5;
}
