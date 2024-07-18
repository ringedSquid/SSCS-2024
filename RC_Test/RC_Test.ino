#define DAC_OUT PIN_PA6
#define DISCHARGE PIN_PA5
#define CHARGE PIN_PA7
#define ADC_IN PIN_PB1

#define R 100.0;

void setup() {
  Serial.begin(57600);
  DACReference(INTERNAL1V5);
  analogReference(INTERNAL2V5);
  
  //init pin modes
  pinMode(DAC_OUT, OUTPUT);
  pinMode(CHARGE, OUTPUT);
  pinMode(DISCHARGE, OUTPUT);
  pinMode(ADC_IN, INPUT);
  
  //init pin states
  digitalWrite(CHARGE, LOW);
  digitalWrite(DISCHARGE, HIGH);
  analogWrite(DAC_OUT, 0);
}

void loop() {
  //Discharge Capaciator and prep DAC
  digitalWrite(DISCHARGE, HIGH);
  digitalWrite(CHARGE, LOW);
  analogWrite(DAC_OUT, 255);
  delay(50);

  //Charge Capciator
  long initial_us = micros();
  digitalWrite(DISCHARGE, LOW);
  digitalWrite(CHARGE, HIGH);

  //Wait until v(t) = 0.63v_DAC
  while(analogRead(ADC_IN) < 387) {
    //do nothing
  }

  long delta_us = micros() - initial_us;
  double delta_t = delta_us * pow(10, -6);
  double c = delta_t/R;
  Serial.print("c: ");
  Serial.print(c);
  Serial.print(", t:");
  Serial.println(delta_t);


}
