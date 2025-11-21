#ifndef MEMORIA_H
#define MEMORIA_H

#include <systemc.h>

SC_MODULE(Memoria) {
    // --- Puertos ---
    sc_in<bool> clk;          // Reloj
    sc_in<bool> enable;       // Habilita el chip
    sc_in<bool> write;        // 1 = Escribir, 0 = Leer
    sc_in<sc_uint<8>> address;// Dirección (8 bits -> 256 posiciones)
    sc_in<sc_uint<8>> data_in;// Dato de entrada
    sc_out<sc_uint<8>> data_out; // Dato de salida

    // --- Almacenamiento Interno ---
    // Array de 256 posiciones, cada una de 8 bits
    sc_uint<8> mem[256];

    // --- Proceso Principal ---
    void operacion() {
        if (enable.read() == true) { // Solo funciona si enable está activo
            if (write.read() == true) {
                // Operación de ESCRITURA
                // Escribimos el dato de entrada en la posición indicada
                mem[address.read()] = data_in.read();
                
                // Opcional: Al escribir, podemos dejar la salida en alta impedancia o 0,
                // aquí simplemente mantenemos el valor anterior o ponemos 0.
            } else {
                // Operación de LECTURA
                // Leemos de la memoria y lo ponemos en el puerto de salida
                data_out.write(mem[address.read()]);
            }
        }
    }

    // --- Constructor ---
    SC_CTOR(Memoria) {
        // Declaramos que 'operacion' es un método sensible al flanco positivo del reloj
        SC_METHOD(operacion);
        sensitive << clk.pos();
        
        // Inicializamos la memoria en 0 para evitar basura (buena práctica)
        for (int i = 0; i < 256; i++) {
            mem[i] = 0;
        }
    }
};

#endif