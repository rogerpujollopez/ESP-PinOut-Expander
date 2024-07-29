#include <stdint.h>

#ifndef TTL74595_H
#define TTL74595_H

class Ttl74595 {
public:
    static void Instanciar();
    static void Inicializar(uint8_t num);
    static void SetAllOn();
    static void SetAllOff();
    static void SetBit(int numBit, bool valorBit);
    static String GetRegisters();

private:
    // Constructor privado para evitar la instanciación
    Ttl74595() {}

    // Declaración del destructor también como privado (opcional)
    ~Ttl74595() {}

    static byte GetHex(String hexString);
    static void SetRegisters();
    static void shiftOutBit(int bit);
    static void shiftOutByte(byte data);
};

#endif // TTL74595_H
