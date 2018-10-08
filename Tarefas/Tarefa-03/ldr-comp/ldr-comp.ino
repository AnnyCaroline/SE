#define LDR 7
#define LED 13

void setup() {
  Serial.begin(9600);  
  DDRB |= (1<<PB5); //LED
  ACSR = 0; //habilita o conversor analógico
}

void loop() {
  int value = ACSR & (1 << ACO);
  
  //Serial.println(value);
  
  if (value){
    PORTB |=  (1<<PB5);
  }else{
    PORTB &= ~(1<<PB5);
  }
  
  //delay(1000); 
}
