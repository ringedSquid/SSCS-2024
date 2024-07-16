#define DAC_OUT PIN_PA4

void setup() {
  DACReference(INTERNAL1V1);
  pinMode(DAC_OUT, OUTPUT);
}

void loop() {
  analogWrite(DAC_OUT, 255);
  // put your main code here, to run repeatedly:

}
