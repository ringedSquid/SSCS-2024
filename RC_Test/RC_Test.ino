#define DAC_OUT PIN_PA6
#define DISCHARGE PIN_PA5
#define CHARGE PIN_PA7
#define ADC_IN PIN_PA4

#define PTN_IN PIN_PA3
#define RED_LED PIN_PB1
#define GRN_LED PIN_PB0

double times[10] = {};
int time_i = 0;

void setup() {
  Serial.begin(57600);
  DACReference(INTERNAL1V5);
  analogReference(INTERNAL2V5);
  
  //init pin modes
  pinMode(DAC_OUT, OUTPUT);
  pinMode(CHARGE, OUTPUT);
  pinMode(DISCHARGE, OUTPUT);
  pinMode(ADC_IN, INPUT);

  pinMode(PTN_IN, INPUT);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GRN_LED, OUTPUT);
  
  //init pin states
  digitalWrite(CHARGE, LOW);
  digitalWrite(DISCHARGE, LOW);
  analogWrite(DAC_OUT, 0);
}

void loop() {
  //Discharge Capaciator and prep DAC
  digitalWrite(DISCHARGE, HIGH);
  digitalWrite(CHARGE, LOW);
  analogWrite(DAC_OUT,255);
  
  while(adc_read() > 15) {
   // Serial.println(adc_read());
    //do nothing
  }
  
  //Charge Capciator
  long initial_us = micros();
  digitalWrite(DISCHARGE, LOW);
  digitalWrite(CHARGE, HIGH);

  //Wait until v(t) = 0.63v_DAC
  while(adc_read() < 637) {
    //do nothing
  }

  long delta_us = micros() - initial_us;
  double delta_t = delta_us * pow(10, -6);
  times[time_i] = delta_t;
  time_i++;
  time_i %= 10;
  double tavg = get_time_average();
  Serial.println(tavg, 8);
}

double adc_read() {
  double sum = 0;
  for (int i=0; i<5; i++) {
    sum += analogRead(ADC_IN);
  }
  return sum/5;
}

double get_time_average() {
  double sum = 0;
  for (int i=0; i<10; i++) {
    sum += times[i];
  }
  return sum/10;
}
