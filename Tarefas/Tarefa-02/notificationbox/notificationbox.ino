#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define RX 9
#define TX 10

#define LED_R 3
#define LED_G 4
#define LED_B 5

#define TOUCH_SENSOR 12


struct noti{
  boolean has_notification = false;
  char str[10];
  int r = 0;
  int g = 0;
  int b = 0; 
  int cont = 0;
};
struct noti noti_temp;
struct noti notificacoes[4];
int noti_i      = 0; // 0,1,2,3


unsigned long momentoAtual;
unsigned long momentoAnterior;
int noti_i_loop = 0; // 0,1,2,3
int timeRange = 1000; // 1s - intervalo de tempo

char str[10];
int str_i = 0;
char c;
int r = 0;
int g = 0;
int b = 0;
int readState = 0;
/*
 * 0 - estou lendo a str
 * 1 - estou lendo o R
 * 2 - estou lendo o G
 * 3 - estou lendo o B
 * 
 */


SoftwareSerial bluetooth(RX, TX);
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

 
void setup() {
  Serial.begin(9600); // Inicializa a serial nativa do Arduino
  
  pinMode(LED_R,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(LED_B,OUTPUT);

  pinMode(TOUCH_SENSOR,INPUT);
  
  analogWrite(LED_R, LOW);
  analogWrite(LED_G, LOW);
  analogWrite(LED_B, LOW);
  
  bluetooth.begin(9600); // Inicializa a serial via software (Onde está conectado o nosso módulo bluetooth)   
  lcd.begin (16,2);

  noti_i = 0; // 0,1,2,3

  noti_i_loop = 0; 
  momentoAtual    = millis();
  momentoAnterior = millis();
}

 
void loop (){

  boolean has_notification = false;
  for (int i=0; i<4; i++){
     if (notificacoes[i].has_notification == true){
        has_notification=true;
        break;
     }
  }

  if (digitalRead(TOUCH_SENSOR) && has_notification){
    for (int i=0; i<4; i++){
      notificacoes[i].has_notification=false;
    }

    has_notification = false;
  }

  momentoAtual = millis();
  if (has_notification){
    if (momentoAtual - momentoAnterior > timeRange){
      while (!notificacoes[noti_i_loop].has_notification){
        noti_i_loop = (noti_i_loop+1)%4;
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(notificacoes[noti_i_loop].str);
      lcd.setCursor(0,1);
      lcd.print(notificacoes[noti_i_loop].cont);         
      
      analogWrite(LED_R, notificacoes[noti_i_loop].r);
      analogWrite(LED_G, notificacoes[noti_i_loop].g);
      analogWrite(LED_B, notificacoes[noti_i_loop].b);
          
      momentoAnterior = momentoAtual;
      noti_i_loop = (noti_i_loop+1)%4;
    }      
  }else{
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("Nenhuma");
    lcd.setCursor(0,1);
    lcd.print("notificacao");    
  }

  
  // verifica se existem bytes a serem lidos da porta serial virtual
  if (bluetooth.available()){

    // Lê 1 byte da porta serial
    c = bluetooth.read();

    //Lendo str
    if (readState == 0){
      if (c == ';'){
        noti_temp.r=0;
        noti_temp.g=0;
        noti_temp.b=0;
        readState = 1;
      }else if (str_i < 10){
        noti_temp.str[str_i] = c;
        noti_temp.str[str_i+1] = '\0';
        str_i++;  
      }
    }
    else if (readState == 1){ // Lendo r
      if (c==',')
        readState = 2;
      else
        noti_temp.r = 10*noti_temp.r + (c-'0');
    }
    
    else if (readState == 2){ // Lendo g
      if (c==',')
        readState = 3;
      else
        noti_temp.g = 10*noti_temp.g + (c-'0');
    }

    else if (readState == 3){ // Lendo b
      if (c==3)
        readState = 4;
      else
        noti_temp.b = 10*noti_temp.b + (c-'0');
    }
  }

  if (readState == 4){
    Serial.println(notificacoes[noti_i].str);
    Serial.println(notificacoes[noti_i].r);
    Serial.println(notificacoes[noti_i].g);
    Serial.println(notificacoes[noti_i].b);

   
    //Já existe com o mesmo nome?

    boolean already_exists = false;
    int i;
    for (i=0; i<4; i++){
      
      if ( strcmp(  (notificacoes[i].str), (noti_temp.str) ) == 0){
        already_exists = true;
        break;
      }
    }

    int insert;
    if (already_exists){
      insert = i;
      notificacoes[insert].cont += 1;
    }else{
      insert = noti_i;
      
      strcpy(notificacoes[insert].str, noti_temp.str);  
      
      notificacoes[insert].cont = 1;
      
      noti_i = (noti_i + 1)%4;
    }

    notificacoes[insert].r = noti_temp.r;
    notificacoes[insert].g = noti_temp.g;
    notificacoes[insert].b = noti_temp.b;
    
    notificacoes[insert].has_notification = true;
    
    str_i = 0;
    readState = 0;
  }
  
}
