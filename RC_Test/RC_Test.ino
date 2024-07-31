#define DAC_OUT PIN_PA6
#define DISCHARGE PIN_PA5
#define CHARGE PIN_PA7
#define ADC_IN PIN_PA4

#define PTN_IN PIN_PA3
#define RED_LED PIN_PB1
#define GRN_LED PIN_PB0

#define AVERAGE_RES 5

void setup() {
  Serial.begin(57600);
  DACReference(INTERNAL1V5);
  analogReference(INTERNAL1V5);
  
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
  digitalWrite(DISCHARGE, HIGH);
  analogWrite(DAC_OUT, 0);
}

void loop() {
   double sum = 0; 
   analogWrite(DAC_OUT, 255);
   delay(10);
   for (int i=0; i<20; i++) {
     digitalWrite(CHARGE, HIGH);
     digitalWrite(DISCHARGE, LOW);
     int val = analogRead(ADC_IN);
     digitalWrite(CHARGE, LOW);
     digitalWrite(DISCHARGE, HIGH);
     sum += val;
     delay(10);        
   }
   Serial.println(sum/20);
}
