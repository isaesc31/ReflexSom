int PINO_BOTAO_PE    = 24;   // push button pressionado
int PINO_BUZZER      = 22;   // buzzer (o "tiro de partida")
int PINO_LED_PRONTO  = 23;   // LED  
int PINO_BOTAO_JUIZ  = 25;   // botão que o juiz aperta pra iniciar o teste
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------- CONFIGURAÇÕES DO TESTE ----------
unsigned long FALSA_LARGADA_MS = 100; 
unsigned long ATRASO_MIN_MS = 500;    // menor delay aleatório antes do tiro
unsigned long ATRASO_MAX_MS = 1000;   // maior delay aleatório antes do tiro
unsigned long TIMEOUT_MS = 5000;      // tempo máximo esperando reação
unsigned long DEBOUNCE_MS = 15;       // tempo de estabilização do contato
unsigned long esperaAleatoria = random(ATRASO_MIN_MS, ATRASO_MAX_MS);
unsigned long FALSA_LARGADA_MS = 100;
unsigned long ATRASO_MIN_MS = 500;
unsigned long ATRASO_MAX_MS = 1000;
unsigned long TIMEOUT_MS = 5000;
unsigned long DEBOUNCE_MS = 15;


// ---------- VARIÁVEIS DE CONTROLE ----------
unsigned long tempoEstimulo = 0;

void telaInicial() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema pronto");
  lcd.setCursor(0, 1);
  lcd.print("Juiz: aperte");
}


void telaAguardandoAtleta() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aperte o");
  lcd.setCursor(0, 1);
  lcd.print("Botão");
}

void setup() {
  Serial.begin(9600);
void telaNaSuaMarca() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NA SUA MARCA...");
  lcd.setCursor(0, 1);
  lcd.print("");
}

void telaTimeout() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMPO ESGOTADO");
  lcd.setCursor(0, 1);
  lcd.print("Sem reacao");
}


void telaResultado(unsigned long tempoMs, bool falsaLargada) {
  char buf[17];
  sprintf(buf, "Reacao: %lu ms", tempoMs);

  // INPUT_PULLUP: pino fica em HIGH quando o botão está solto.
  // O pé pressiona o botão (contato fecha) e o pino vai para LOW.
  pinMode(PINO_BOTAO_PE, INPUT_PULLUP);
  pinMode(PINO_BOTAO_JUIZ, INPUT_PULLUP);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(buf);
  lcd.setCursor(0, 1);
  lcd.print(falsaLargada ? "LARGADA FALSA!" : "VALIDO");
}


// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);


  pinMode(PINO_BOTAO_PE, INPUT_PULLUP);
  pinMode(PINO_BOTAO_JUIZ, INPUT_PULLUP);
  pinMode(PINO_BUZZER, OUTPUT);
  pinMode(PINO_LED_PRONTO, OUTPUT);


  // Com I2C o A0 está livre de novo, então volta a servir para ruído
  randomSeed(analogRead(A0));


  randomSeed(analogRead(A0)); // semente aleatória a partir de ruído elétrico


  lcd.init();        // em algumas versões da biblioteca use lcd.begin()
  lcd.backlight();
  telaInicial();


  Serial.println("Sistema pronto. Aperte o botao do juiz para iniciar um teste.");
}




// ---------- LOOP ----------
void loop() {
  if (lerBotaoComDebounce(PINO_BOTAO_JUIZ) == LOW) {
    iniciarTeste();
  }
}




void iniciarTeste() {
  Serial.println("\n--- NOVO TESTE ---");
  Serial.println("Aguardando atleta apoiar o pe no botao...");
  Serial.println("Aguardando atleta apoiar botao...");
  digitalWrite(PINO_LED_PRONTO, HIGH);
  telaAguardandoAtleta();




  // Espera o botão ficar pressionado (LOW = pé apoiado) antes de começar
  while (digitalRead(PINO_BOTAO_PE) == HIGH) {
    // aguarda o atleta se posicionar
  }


  Serial.println("Botão pressionado. Na sua marca... pronto..");
  telaNaSuaMarca();  // escreve ANTES do delay aleatório


  Serial.println("Pe apoiado. Na sua marca... pronto..");
  // Delay aleatório antes do tiro, pra impedir antecipação
  unsigned long esperaAleatoria = random(ATRASO_MIN_MS, ATRASO_MAX_MS);
  delay(esperaAleatoria);




  // ---------- ESTÍMULO SONORO (TIRO) ----------
  tone(PINO_BUZZER, 2000, 150);
  tempoEstimulo = micros();
  digitalWrite(PINO_LED_PRONTO, LOW);




  Serial.println("Cronometrando reacao...");


  Serial.println("BANG! Cronometrando reacao...");


  unsigned long tempoLimite = millis() + TIMEOUT_MS;
  bool reagiu = false;




  // Fica monitorando o botão até ele soltar (pé saiu) ou até o timeout
  while (!reagiu && millis() < tempoLimite) {
    if (digitalRead(PINO_BOTAO_PE) == HIGH) {
      reagiu = true;
      void iniciarTeste() {
  }




  if (!reagiu) {
    Serial.println("Tempo esgotado - nenhuma reacao detectada.");
    telaTimeout();
  }




  Serial.println("--- FIM DO TESTE ---");
}




void mostrarResultado(unsigned long tempoAgora) {
  unsigned long tempoReacaoMs = (tempoAgora - tempoEstimulo) / 1000UL;


  bool falsaLargada = (tempoReacaoMs < FALSA_LARGADA_MS);


  Serial.print("Tempo de reacao: ");
  Serial.print(tempoReacaoMs);
  Serial.print(" ms");
  Serial.println(falsaLargada ? "  -> LARGADA FALSA (reagiu em menos de 100ms)" : "  -> Valido");




  if (tempoReacaoMs < FALSA_LARGADA_MS) {
    Serial.println("  -> LARGADA FALSA (reagiu em menos de 100ms)");
  } else {
    Serial.println("  -> Valido");
  }
  telaResultado(tempoReacaoMs, falsaLargada);
}




// Debounce usado só no botão do juiz (apertado manualmente).
// No botão do pé não é necessário: o pé demora naturalmente mais
// que alguns milissegundos para sair por completo.
int lerBotaoComDebounce(int pino) {
  int leitura1 = digitalRead(pino);
  if (leitura1 == LOW) {
    delay(DEBOUNCE_MS);
    int leitura2 = digitalRead(pino);
    if (leitura2 == LOW) {
      while (digitalRead(pino) == LOW) { } // espera soltar
      while (digitalRead(pino) == LOW) { }
      return LOW;
    }
  }
  return HIGH;
}
