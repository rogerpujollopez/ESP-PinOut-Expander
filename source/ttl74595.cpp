#include <stdint.h>
#include <vector>
#include <Arduino.h>
#include "Ttl74595.h"

std::vector<uint8_t> registros;

// Pines del 74HC595
const int DS = 0;       // GPIO0 -> DS (Data in)
const int SH_CP = 1;    // GPIO1 -> SH_CP (Shift Clock)
const int ST_CP = 2;    // GPIO2 -> ST_CP (Store Clock)

void Ttl74595::Instanciar() {
  pinMode(DS, OUTPUT);
  pinMode(SH_CP, OUTPUT);
  pinMode(ST_CP, OUTPUT);
  digitalWrite(DS, LOW);
  digitalWrite(SH_CP, LOW);
  digitalWrite(ST_CP, LOW);
}

void Ttl74595::Inicializar(uint8_t num){
  for(int i=0;i<num;i++){
    registros.push_back(0x00);   
  }
}

byte Ttl74595::GetHex(String hexString)
{
  return strtol(hexString.c_str(), NULL, 16);
}

void Ttl74595::SetBit(int numBit, bool valorBit) {
  // Calcular el índice del byte en el vector y la posición del bit dentro de ese byte
  int byteIndex = numBit / 8;
  int bitIndex = numBit % 8;

  // Verificar si numBit está dentro del rango permitido
  if (byteIndex >= registros.size()) {
    return; // Salir si numBit está fuera del rango
  }

  // Establecer o borrar el bit
  if (valorBit) {
    registros[byteIndex] |= (1 << bitIndex);  // Establecer el bit
  } else {
    registros[byteIndex] &= ~(1 << bitIndex); // Borrar el bit
  }
  SetRegisters();
}

void Ttl74595::SetAllOn(){
  size_t pos = registros.size();
  for (size_t t = 0; t < pos; t++) {
    registros[t]=255;
  }
  SetRegisters();
}

void Ttl74595::SetAllOff(){
  size_t pos = registros.size();
  for (size_t t = 0; t < pos; t++) {
    registros[t]=0;
  }
  SetRegisters();
}

void Ttl74595::SetRegisters() 
{
  size_t pos = registros.size();
  for (size_t t = 0; t < pos; t++) {
    uint8_t data = registros[(pos-1)-t];
      for (int i = 7; i >= 0; i--) {
        shiftOutBit((data >> i) & 1);
      }
  }

  // Controla el latch pin (ST_CP)
  digitalWrite(ST_CP, HIGH);
  delayMicroseconds(1);
  digitalWrite(ST_CP, LOW);
}

String Ttl74595::GetRegisters() 
{
  String binaryString = "";
  size_t pos = registros.size();
  for (size_t t = 0; t < pos; t++) {
    uint8_t data = registros[(pos-1)-t];
    for (int i = 7; i >= 0; i--) {
      binaryString += ((data >> i) & 1) ? '1' : '0';
    }
  }
  return binaryString;
}

// Función para enviar un bit al 74HC595
void Ttl74595::shiftOutBit(int bit) {
  digitalWrite(DS, bit);
  digitalWrite(SH_CP, HIGH);
  delayMicroseconds(1);
  digitalWrite(SH_CP, LOW);
  delayMicroseconds(1);
}

// Función para enviar un byte al 74HC595
void Ttl74595::shiftOutByte(byte data) {
  for (int i = 7; i >= 0; i--) {
    shiftOutBit((data >> i) & 1);
  }
  digitalWrite(ST_CP, HIGH);
  delayMicroseconds(1);
  digitalWrite(ST_CP, LOW);
}
