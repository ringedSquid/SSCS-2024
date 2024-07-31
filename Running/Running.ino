#include <avr/sleep.h>

//Pinout
#define DAC_OUT PIN_PA6
#define DISCHARGE PIN_PA5
#define CHARGE PIN_PA7
#define ADC_IN PIN_PA4

#define PTN_IN PIN_PA2
#define RED_LED PIN_PB1
#define GRN_LED PIN_PB0

#define AVERAGE_RES 20

//Coefficients for linear model
#define DRY 620.0
#define WET 30.0

//user defined moisture ranges (acceptable range)
#define RL 0.3
#define RH 0.7

double M = 0;
double B = 0;

void setup() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  Serial.begin(57600);
  DACReference(INTERNAL1V5);
  analogReference(INTERNAL1V5);

  M = 1/(WET-DRY);
  B = -DRY*M;

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
  pinMode(PIN_PA3, OUTPUT);
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
  double moistureLevel = getMoistureLevel(getRawData());
  //Serial.println(moistureLevel, 8);
  //Serial.println(M, 8);
  if (moistureLevel <= RL) {
    digitalWrite(RED_LED, HIGH);
  }
  else if (moistureLevel >= RH) {
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
  digitalWrite(DISCHARGE, HIGH);
  digitalWrite(CHARGE, LOW);
  digitalWrite(GRN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  analogWrite(DAC_OUT, 0);                                                                                                                                                                                                                                                                                                                                                                                                           
  //Disable ADC
  ADC0.CTRLA &= ~ADC_ENABLE_bm;
  //Interrupt on rising
  PORTA.PIN2CTRL = 0b00000010;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_cpu();
}

ISR(PORTA_PORT_vect) {
  //Clear flags
  PORTA.INTFLAGS = 255;
  //Disable interrupt
  PORTA.PIN2CTRL &= ~(0b00000111);
  //enable ADC
  ADC0.CTRLA |= ADC_ENABLE_bm;
}

double getMoistureLevel(double x) {
  return M * x + B;
}

double getRawData() {
  double sum = 0;
  analogWrite(DAC_OUT, 255);
  delay(10);
  for (int i = 0; i < AVERAGE_RES; i++) {
     digitalWrite(CHARGE, HIGH);
     digitalWrite(DISCHARGE, LOW);
     int val = analogRead(ADC_IN);
     digitalWrite(CHARGE, LOW);
     digitalWrite(DISCHARGE, HIGH);
     sum += val;
     delay(10);     
  }
  //Serial.println(sum/AVERAGE_RES);
  return sum/AVERAGE_RES;   
}
