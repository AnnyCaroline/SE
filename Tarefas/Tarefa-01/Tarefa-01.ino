#define BTN_DESACELERAR 2
#define BTN_ACELERAR 4

const int ledPin = 13;
const int maxInt = 32727;
const int dt = 5;

int btnDes;
int btnAce;
int ledState = LOW;
int timeRange = 1000; // 1s - intervalo de tempo entre mudança de estado do led

unsigned long momentoAtual;
unsigned long momentoAnterior;

unsigned long primeiroClique;
unsigned long segundoClique;
int clickState = 0;
/*
 * 0: não cliquei em ninguém ainda
 * 1: cliquei primeiro no btnDes
 * 2: cliquei primiero no btnAce
 * 3: cliquei nos 2 botões
 */

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(BTN_DESACELERAR, INPUT);
  pinMode(BTN_ACELERAR, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(BTN_DESACELERAR)){

    Serial.println("clicou");
    
    // AUMENTAR TEMPO ENTRE MUDANÇA DE ESTADO DO LED
    timeRange += timeRange <= maxInt - dt ? dt : 0;

    // ATUALIZA O ESTADO DO BOTÃO
    
    if (clickState == 0 || clickState == 1){
      // clickState == 0: CASO SEJA A PRIMEIRA VEZ QUE SE CLICA EM UM BOTÃO 
      // clickState == 1: CASO O USUÁRIO ESTEJA CLICANDO PELA SEGUNDA VEZ NO BOTÃO DE DESACELERAR => RESETA A CONTAGEM DE TEMPO E SE MANTÉM O ESTADO
      primeiroClique = millis();
      clickState = 1;
      
    } else if (clickState == 2){
      // O BOTÃO DE ACELERAÇÃO JÁ FOI CLICADO
      segundoClique = millis();
      clickState = 3;
    }  
  }
  
  if (digitalRead(BTN_ACELERAR)){

    // DIMINUI TEMPO ENTRE MUDANÇA DE ESTADO DO LED
    timeRange -= (timeRange >= dt) ? dt : 0;

    // ATUALIZA O ESTADO DO BOTÃO
    
    if (clickState == 0 || clickState == 2){
      // clickState == 0: CASO SEJA A PRIMEIRA VEZ QUE SE CLICA EM UM BOTÃO 
      // clickState == 2: CASO O USUÁRIO ESTEJA CLICANDO PELA SEGUNDA VEZ NO BOTÃO DE ACELERAR => RESETA A CONTAGEM DE TEMPO E SE MANTÉM O ESTADO
      primeiroClique = millis();
      clickState = 2;
      
    } else if (clickState == 1){
      // O BOTÃO DE DESACELERAÇÃO JÁ FOI CLICADO
      segundoClique = millis();
      clickState = 3;
    } 
  }

  // VERIFICA-SE O TEMPO ENTRE O CLIQUE DOS DOIS BOTÕES
  if (clickState == 3){
    if (segundoClique - primeiroClique <= 500){
      // O TEMPO DE CLIQUE ENTRE OS DOIS BOTÕES FOI MENOR, OU IGUAL, A 500ms => O LED É ACESSO E A O PROGRAMA FICA PRESO NO LOOP WHILE(1)
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("fim");
      while(1);
    }else{
      // O TEMPO DE CLIQUE ENTRE OS DOIS BOTÕES FOI MAIOR QUE 500ms, ENTÃO OS CLIQUES SÃO DESCONSIDERADO (ISTO É, CONSIDERA-SE QUE NENHUM BOTÃO FOI CLICADO)
      clickState = 0;
    }
  }
  
  momentoAtual = millis();
  if (momentoAtual - momentoAnterior > timeRange){
    ledState = !ledState;
    
    digitalWrite(LED_BUILTIN, ledState);
    momentoAnterior = momentoAtual;
  }
  
  
  
}
