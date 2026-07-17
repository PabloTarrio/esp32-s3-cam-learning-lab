#ifndef PLC_DEBOUNCE_H
#define PLC_DEBOUNCE_H

#include <Arduino.h>

// ============================================================================
// PlcDebounce.h
// Librería local: plc_blocks
//
// Descripción:
//   Declaración de un bloque de antirrebote reutilizable inspirado en PLC.
//
// Bloques incluidos:
//   - DEBOUNCE : filtro de estabilidad para señales booleanas.
//
// Notas:
//   - El bloque no lee directamente GPIO.
//   - Recibe una señal booleana ya procesada.
//   - La salida Q representa el último estado estable aceptado.
//   - No detecta flancos por sí mismo.
//   - Para detectar pulsaciones o liberaciones válidas, combinar con
//     R_TRIG o F_TRIG.
// ============================================================================


/************************************************************************
 * Bloque       : DEBOUNCE
 * Tipo         : Filtro de antirrebote para señal booleana
 * Interfaz     :
 *    - IN : señal booleana de entrada.
 *    - PT : tiempo mínimo de estabilidad en milisegundos.
 *    - Q  : salida booleana estable.
 *
 * Descripción :
 *  Implementa un bloque de antirrebote reutilizable.
 *
 *  El bloque recibe una señal booleana que puede cambiar de forma
 *  inestable debido a rebotes mecánicos o ruido.
 *
 *  Cuando IN cambia respecto a la última lectura, se reinicia el
 *  tiempo de estabilidad.
 *
 *  Si IN permanece sin cambios durante al menos PT milisegundos,
 *  ese valor se acepta como nuevo estado estable y se actualiza Q.
 *
 *  El bloque no usa delay() y debe actualizarse de forma cíclica
 *  mediante llamadas repetidas a update().
 ***********************************************************************/
class DEBOUNCE{
    private:
        bool q;
        bool last_input;
        bool stable_state;
        unsigned long last_change_time;

    public:
        DEBOUNCE();
        void update(bool in, unsigned long pt);
        bool Q() const;
};

#endif