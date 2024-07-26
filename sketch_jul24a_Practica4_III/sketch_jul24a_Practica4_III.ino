/*
   Fundacion Kinal
   Centro educativo tecnico laboral kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico: EB5AV
   Curso: Taller de electronica digital y reparacion de computadoras I
   Proyecto: Practica 4, unica parte, Bimestre 3
   Dev: Diego Alejandro Felipe Chis
   Profesor: Alejandro Navas
   Fecha: 26 de julio de 2024
*/
#include <SPI.h>
#include <MFRC522.h>
#define reset 9
#define slave 10
#define verde 3
#define rojo 4
#define buzzer 5
MFRC522 mfrc522(slave, reset);
byte LecturaUID[4];
byte tag1[4]= {0xD3, 0x79, 0x07, 0x14};
byte tag2[4]= {0xC3, 0x2E, 0x4D, 0x34};
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.println("Sistema iniciado. Esperando tarjeta...");
}
void acceso() {
  digitalWrite(verde, HIGH);
  tone(buzzer, 200);
  delay(4000);
  digitalWrite(verde, LOW);
  noTone(buzzer);
  delay(100);
}
void denegado() {
  digitalWrite(rojo, HIGH);
  tone(buzzer, 400);
  delay(300);
  digitalWrite(rojo, LOW);
  noTone(buzzer);
}
void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("UID leído: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      Serial.print("0");
    }
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
    LecturaUID[i] = mfrc522.uid.uidByte[i];
  }
  Serial.println();
  if (comparaUID(LecturaUID, tag1)) {
    Serial.println("Acceso permitido: usuario 1");
    acceso();
  } else if (comparaUID(LecturaUID, tag2)) {
    Serial.println("Acceso permitido: usuario 2");
    acceso();
  } else {
    Serial.println("Usuario no reconocido");
    denegado();
  }
  mfrc522.PICC_HaltA();
}
boolean comparaUID(byte lectura[], byte usuario[]) {
  for (byte i = 0; i < 4; i++) {
    if (lectura[i] != usuario[i]) {
      return false;
    }
  }
  return true;
}