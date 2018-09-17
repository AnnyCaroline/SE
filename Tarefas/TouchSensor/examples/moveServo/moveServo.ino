#include <Servo.h>

Servo myservo;

#define SERVO_BEGIN 90
#define SERVO_END   180

#define SWITCH_PIN  12
#define SERVO_PIN   9

#define RED_LED_PIN  2
#define BLUE_LED_PIN 3

int btnState;
int servoState;

void setup() {
  btnState = 0;
  servoState = 0;

  pinMode(SWITCH_PIN, INPUT);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  
  myservo.attach(SERVO_PIN);
  myservo.write(SERVO_BEGIN);

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, HIGH);  
}

void loop() {
  if (digitalRead(SWITCH_PIN) && btnState == 0)
    btnState = 1;

  if (!digitalRead(SWITCH_PIN) && btnState == 1){
    if (servoState){
      myservo.write(SERVO_BEGIN);
      digitalWrite(RED_LED_PIN, LOW);
      digitalWrite(BLUE_LED_PIN, HIGH);
    }else{
      myservo.write(SERVO_END);
      digitalWrite(RED_LED_PIN, HIGH);
      digitalWrite(BLUE_LED_PIN, LOW);
    }
    
    servoState = !servoState;
    btnState   = 0;
  }
}

