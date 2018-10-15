/*
 * toggle a led using timer0 overflow
 */

int ledState = 0;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //Compare Output Mode: Normal mode
  TCCR0A &= ~(1<<COM0A0); //0
  TCCR0A &= ~(1<<COM0A1); //0

  // CS - 011: prescaler de 1024 bits
  TCCR0B |= (1<<CS00);  //1
  TCCR0B &= ~(1<<CS01); //0
  TCCR0B |= (1<<CS02);  //1

  pinMode(13, OUTPUT);
}

void loop() {


  bool t = TIFR0 & (1<<TOV0);

  if (t){
    count++;
    Serial.println(count);
  }
  
  if (count == 10){
    //Serial.println("overflow");
    ledState = !ledState;
    digitalWrite(13,ledState);
    count=0;
  }else{
   //Serial.println("no overflow");
  }
}
