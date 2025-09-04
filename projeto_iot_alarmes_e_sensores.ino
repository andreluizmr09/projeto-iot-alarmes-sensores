#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LDR = A0;
const int botaoJanela = 2;
const int botaoPorta = 3;
const int botaoGaragem = 4;
const int piezo = 8;
const int pirPin = 11;
const int ledJanela = 5;
const int ledPorta = 6;
const int ledGaragem = 7;

const int limiarLuz = 500;

void setup() {
  pinMode(botaoJanela, INPUT_PULLUP);
  pinMode(botaoPorta, INPUT_PULLUP);
  pinMode(botaoGaragem, INPUT_PULLUP);
  pinMode(piezo, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(ledJanela, OUTPUT);
  pinMode(ledPorta, OUTPUT);
  pinMode(ledGaragem, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.print("Sistema Pronto");
}

void loop() {
  int valorLuz = analogRead(LDR);
  
  if(valorLuz < limiarLuz) {
    lcd.setCursor(0, 1);
    lcd.print("Modo Noturno    ");
    verificarSensores();
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Modo Diurno     ");
    desligarTodosLEDs();
    noTone(piezo);
  }
}

void verificarSensores() {
  if(digitalRead(pirPin) == HIGH) {
    ativarAlarme(2, "INVASAO: GARAGEM");
  }
  
  if(!digitalRead(botaoJanela)) {
    ativarAlarme(0, "ALERTA: JANELA");
    while(!digitalRead(botaoJanela));
  }
  if(!digitalRead(botaoPorta)) {
    ativarAlarme(1, "ALERTA: PORTA");
    while(!digitalRead(botaoPorta));
  }
  if(!digitalRead(botaoGaragem)) {
    ativarAlarme(2, "ALERTA: GARAGEM");
    while(!digitalRead(botaoGaragem));
  }
}

void ativarAlarme(int tipoAlarme, const char* mensagem) {
  desligarTodosLEDs();
  
  switch(tipoAlarme) {
    case 0: 
      digitalWrite(ledJanela, HIGH);
      tone(piezo, 1000);
      break;
    case 1: 
      digitalWrite(ledPorta, HIGH);
      tone(piezo, 800);
      break;
    case 2: 
      digitalWrite(ledGaragem, HIGH);
      tone(piezo, 600);
      break;
  }
  
  lcd.clear();
  lcd.print(mensagem);
  delay(5000);
  
  noTone(piezo);
  desligarTodosLEDs();
  lcd.clear();
  lcd.print("Sistema Pronto");
}

void desligarTodosLEDs() {
  digitalWrite(ledJanela, LOW);
  digitalWrite(ledPorta, LOW);
  digitalWrite(ledGaragem, LOW);
}