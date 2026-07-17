#ifndef PLC_EDGES_H
#define PLC_EDGES_H

#include <Arduino.h>

// ============================================================================
// PlcEdges.h
// Librería local: plc_blocks
//
// Descripción:
//   Declaración de detectores de flanco inspirados en bloques PLC.
//
// Bloques incluidos:
//   - R_TRIG : Detector de flanco ascendente.
//   - F_TRIG : Detector de flanco descendente.
//
// Notas:
//   - Los detectores no leen directamente GPIO.
//   - Reciben una señal booleana ya procesada.
//   - La salida Q se activa solo durante un ciclo de update().
//   - No implementan antirrebote.
// ============================================================================

/************************************************************************
 * Bloque       : R_TRIG
 * Tipo         : Detector de flanco ascendente
 * Interfaz     :
 *    - CLK : señal booleana de entrada.
 *    - Q   : salida booleana activa durante un ciclo.
 *
 * Descripción :
 *  Implementa un detector de flanco ascendente inspirado en PLC.
 *
 *  Cuando la señal CLK pasa de FALSE a TRUE, la salida Q pasa a TRUE
 *  durante una única ejecución de update().
 *
 *  En la siguiente llamada a update(), si CLK permanece en TRUE,
 *  Q vuelve a FALSE.
 *
 *  Para detectar un nuevo flanco, CLK debe volver primero a FALSE
 *  y después pasar otra vez a TRUE.
 *
 *  El bloque guarda internamente el estado anterior de CLK para
 *  comparar la señal actual con la señal del ciclo anterior.
 ***********************************************************************/
class R_TRIG {
  private:
    bool q;
    bool previous_clk;
  public:
    R_TRIG();
    void update (bool clk);
    bool Q() const;
};

/************************************************************************
 * Bloque       : F_TRIG
 * Tipo         : Detector de flanco descendente
 * Interfaz     :
 *    - CLK : señal booleana de entrada.
 *    - Q   : salida booleana activa durante un ciclo.
 *
 * Descripción :
 *  Implementa un detector de flanco descendente inspirado en PLC.
 *
 *  Cuando la señal CLK pasa de TRUE a FALSE, la salida Q pasa a TRUE
 *  durante una única ejecución de update().
 *
 *  En la siguiente llamada a update(), si CLK permanece en FALSE,
 *  Q vuelve a FALSE.
 *
 *  Para detectar un nuevo flanco descendente, CLK debe volver primero
 *  a TRUE y después pasar otra vez a FALSE.
 *
 *  El bloque guarda internamente el estado anterior de CLK para
 *  comparar la señal actual con la señal del ciclo anterior.
 ***********************************************************************/
class F_TRIG {
  private:
    bool q;
    bool previous_clk;
  public:
    F_TRIG();
    void update (bool clk);
    bool Q() const;
};

#endif