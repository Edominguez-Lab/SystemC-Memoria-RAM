#include <systemc.h>
#include "memoria.h"
#include <iostream>

int sc_main(int argc, char* argv[]) {
    // 1. Instanciación de Señales y Reloj
    sc_clock clk("clk", 10, SC_NS); // Reloj de 10ns
    sc_signal<bool> enable;
    sc_signal<bool> write;
    sc_signal<sc_uint<8>> address;
    sc_signal<sc_uint<8>> data_in;
    sc_signal<sc_uint<8>> data_out;

    // 2. Instanciación del Módulo (DUT - Design Under Test)
    Memoria memoria1("MemoriaRAM");
    
    // 3. Conexión de puertos
    memoria1.clk(clk);
    memoria1.enable(enable);
    memoria1.write(write);
    memoria1.address(address);
    memoria1.data_in(data_in);
    memoria1.data_out(data_out);

    // 4. Archivo de trazas (Waveform) para ver en GTKWave (Opcional pero recomendado)
    sc_trace_file *wf = sc_create_vcd_trace_file("simulacion_memoria");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, enable, "enable");
    sc_trace(wf, write, "write");
    sc_trace(wf, address, "address");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, data_out, "data_out");

    // 5. Inicio de la Simulación
    cout << "@" << sc_time_stamp() << " Inicio de la simulacion" << endl;
    
    enable = 0; // Deshabilitado inicialmente
    sc_start(10, SC_NS);

    // --- PRUEBA 1: ESCRITURA ---
    cout << "--- Escribiendo datos ---" << endl;
    enable = 1;
    write = 1; // Modo escritura

    // Escribir 100 en dirección 0
    address = 0;
    data_in = 100;
    sc_start(10, SC_NS); // Esperar un ciclo

    // Escribir 200 en dirección 5
    address = 5;
    data_in = 200;
    sc_start(10, SC_NS);

    // Escribir 50 en dirección 255
    address = 255;
    data_in = 50;
    sc_start(10, SC_NS);

    // --- PRUEBA 2: LECTURA ---
    cout << "--- Leyendo datos ---" << endl;
    write = 0; // Modo lectura
    
    // Leer dirección 0
    address = 0;
    sc_start(10, SC_NS); // Importante: esperar un ciclo para que se actualice data_out
    cout << "@" << sc_time_stamp() << " Leido Dir 0: " << data_out.read() << " (Esperado: 100)" << endl;

    // Leer dirección 5
    address = 5;
    sc_start(10, SC_NS);
    cout << "@" << sc_time_stamp() << " Leido Dir 5: " << data_out.read() << " (Esperado: 200)" << endl;

    // Leer dirección 255
    address = 255;
    sc_start(10, SC_NS);
    cout << "@" << sc_time_stamp() << " Leido Dir 255: " << data_out.read() << " (Esperado: 50)" << endl;

    // 6. Finalizar
    cout << "@" << sc_time_stamp() << " Fin de la simulacion" << endl;
    sc_close_vcd_trace_file(wf);
    
    return 0;
}