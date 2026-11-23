#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// Endereço I2C: o mais comum é 0x27; se nada aparecer, tente 0x3F
LiquidCrystal_I2C lcd(0x27, 16, 2);


// ---------- PINOS ----------
int PINO_BOTAO_PE    = 2;  
int PINO_BUZZER      = 22;  
int PINO_LED_PRONTO  = 9;  
int PINO_BOTAO_JUIZ  = 4;  


// ---------- CONFIGURAÇÕES DO TESTE ----------
unsigned long FALSA_LARGADA_MS = 100;
unsigned long ATRASO_MIN_MS = 500;
unsigned long ATRASO_MAX_MS = 1000;
unsigned long TIMEOUT_MS = 5000;
unsigned long DEBOUNCE_MS = 15;


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
  Serial.println("Aguardando atleta apoiar botao...");
  digitalWrite(PINO_LED_PRONTO, HIGH);
  telaAguardandoAtleta();


  while (digitalRead(PINO_BOTAO_PE) == HIGH) {
    // aguarda o atleta se posicionar
  }


  Serial.println("Botão pressionado. Na sua marca... pronto..");
  telaNaSuaMarca();  // escreve ANTES do delay aleatório


  unsigned long esperaAleatoria = random(ATRASO_MIN_MS, ATRASO_MAX_MS);
  delay(esperaAleatoria);


  // ---------- ESTÍMULO SONORO (TIRO) ----------
  tone(PINO_BUZZER, 2000, 150);
  tempoEstimulo = micros();
  digitalWrite(PINO_LED_PRONTO, LOW);


  Serial.println("BANG! Cronometrando reacao...");


  unsigned long tempoLimite = millis() + TIMEOUT_MS;
  bool reagiu = false;



  while (!reagiu && millis() < tempoLimite) {
    if (digitalRead(PINO_BOTAO_PE) == HIGH) {
      reagiu = true;
      mostrarResultado(micros());
    }
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


  telaResultado(tempoReacaoMs, falsaLargada);
}


int lerBotaoComDebounce(int pino) {
  int leitura1 = digitalRead(pino);
  if (leitura1 == LOW) {
    delay(DEBOUNCE_MS);
    int leitura2 = digitalRead(pino);
    if (leitura2 == LOW) {
      while (digitalRead(pino) == LOW) { }
      return LOW;
    }
  }
  return HIGH;
}
