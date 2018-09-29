/* clicks */
unsigned long clickFirst;
unsigned long clickSecond;
int clickState = 0;
/*
 * 0: no button was clicked yet
 * 1: the deceleration button was the first to be clicked
 * 2: the acceleration button was the first to be clicked
 * 3: the two buttons were clicked
 */

/* led */
const int ledPin = 13;
int ledState = LOW;
int ledInterval = 1000;
int ledIntervalMax = 2600;
int ledIntervalDt = 200;
unsigned long ledPreviusMillis;
unsigned long ledCurrentMillis;

/* deceleration button */
const int desPin = 2;
int desReading;
int desLastState = LOW;
int desState;
int desLock = 1;
unsigned long desLastDebounceTime = 0;

/* acceleration button */
const int accPin = 4;
int accReading;
int accLastState = LOW;
int accState;
int accLock = 1;
unsigned long accLastDebounceTime = 0;

/* buttons debounce */
unsigned long debounceDelay = 25;


void setup() {
  //pinMode(ledPin, OUTPUT);
  //pinMode(desPin, INPUT);
  //pinMode(accPin, INPUT);
  DDRB |= 1<<5; //PB5 = 1
  DDRD &= ~(1 << 2); //PD2 = 0
  DDRD &= ~(1 << 4); //PD4 = 0

  ledPreviusMillis = millis();
  
  Serial.begin(9600);
}

void loop() {
  // deceleration button
  //desReading = digitalRead(desPin);
  desReading = PIND & (1 << 2);

  if (desReading != desLastState)
    desLastDebounceTime = millis();

  if ((millis() - desLastDebounceTime) > debounceDelay) {    
    if (desReading != desState) {
      desState = desReading;

      if (!desState && desLock == 0) 
          desLock = 1;
    
      if (desState && desLock == 1){
          // increase the ledInterval
          ledInterval += ledInterval <= ledIntervalMax - ledIntervalDt ? ledIntervalDt : 0;
      
          // update clickState
          if (clickState == 0 || clickState == 1){
            // clickState == 0: it is the first time that a button was clicked
            // clickState == 1: the user is clicking deceleration button for the second time => reset clickFirst and maintain the clickState
            clickFirst = millis();
            clickState = 1;
            
          } else if (clickState == 2){
            // the acelerate button was already clicked
            clickSecond = millis();
            clickState = 3;
          }
    
          desLock = 0;   
      }
    }
  } 
  desLastState = desReading;

  
  // acceleration button 
  //accReading = digitalRead(accPin);
  accReading = PIND & (1 << 4);

  if (accReading != accLastState)
    accLastDebounceTime = millis();

  if ((millis() - accLastDebounceTime) > debounceDelay) {    
    if (accReading != accState) {
      accState = accReading;

      if (!accState && accLock == 0)
          accLock = 1;
    
      if (accState && accLock == 1){
        // decrease the ledInterval
        ledInterval -= (ledInterval >= ledIntervalDt) ? ledIntervalDt : 0;
    
        // update clickState
        if (clickState == 0 || clickState == 2){
          // clickState == 0: it is the first time that a button was clicked
          // clickState == 2: the user is clicking acceleration button for the second time => reset clickFirst and maintain the clickState     
          clickFirst = millis();
          clickState = 2;
          
        } else if (clickState == 1){
          // the deceleration button was already clicked
          clickSecond = millis();
          clickState = 3;
        } 
    
        accLock = 0;
      }
    }
  } 
  accLastState = accReading;   
   

  // check the time between the clicks of the two buttons
  if (clickState == 3){
    if (clickSecond - clickFirst <= 500){
      // the time between the clicks of the two buttons was less, or equal, to 500ms => the program gets stuck in the while loop
      while(1);
    }else{
      // the time between the clicks of the two buttons was greater than 500ms, so the clicks are disregarded. In other words, it is considered that no button was clicked yet
      clickState = 0;
    }
  }

  // blink led
  ledCurrentMillis = millis();
  if (ledCurrentMillis - ledPreviusMillis > ledInterval){
    ledState = !ledState;
    
    //digitalWrite(ledPin, ledState);
    if (ledState)
      PORTB |= 1 << 5;
    else
      PORTB &= ~(1 << 5);
      
    ledPreviusMillis = ledCurrentMillis;
  }
  
  
  
}


