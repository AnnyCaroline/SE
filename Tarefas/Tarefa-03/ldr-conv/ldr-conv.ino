#define LED 13

void setup() {
  Serial.begin(9600);

  ADMUX = 0;
  ADMUX |= (1 << REFS0); //Vcc como referência - ADMUX, REFS1 = 0 e REFS0 = 1
  
  // LDR está conectado no A5 - MUX[3:0] = 0101
  // ADMUX |= (1 << MUX0);
  // ADMUX |= (1 << MUX2);

  ADCSRA |= (1 << ADEN); //habilita o ADC
}

void loop() {
  ADCSRA |= (1<<ADSC); //start conversion

  while( ADCSRA & (1<<ADSC) );

  int value = ADCL;
  value = (ADCH << 8) + value;
    
  Serial.println(value);
}
