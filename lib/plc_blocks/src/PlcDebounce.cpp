
#include "PlcDebounce.h"

// ============================================================================
// PlcDebounce.cpp
// Librería local: plc_blocks
//
// Descripción:
//   Implementación de un bloque de antirrebote reutilizable.
//
// Bloques incluidos:
//   - DEBOUNCE : filtro de estabilidad para señales booleanas.
//
// Notas:
//   - El bloque no lee directamente GPIO.
//   - Recibe una señal booleana ya procesada.
//   - La salida Q representa el último estado estable aceptado.
//   - No detecta flancos.
//   - Para detectar pulsaciones o liberaciones válidas, combinar con
//     R_TRIG o F_TRIG.
// ============================================================================


// ============================================================================
// DEBOUNCE - Filtro de antirrebote para señal booleana
// ============================================================================

/***************************************************************
 * Rutina      : DEBOUNCE()    Tipo        : Constructor
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - Ninguno
 * Descripción :
 *       Inicializa el bloque de antirrebote en estado de reposo.
 ***************************************************************/
DEBOUNCE::DEBOUNCE(){
    q = false;
    last_input = false;
    stable_state = false;
    last_change_time = 0;
}

/***************************************************************
 * Rutina      : update()
 * Tipo        : Método
 * Parámetros  :
 *      - in : señal booleana de entrada.
 *      - pt : tiempo mínimo de estabilidad, en milisegundos.
 *
 * Retorno     :
 *      - Ninguno
 *
 * Descripción :
 *      Actualiza el estado interno del bloque DEBOUNCE.
 *
 *      Debe llamarse de forma cíclica desde loop().
 *
 *      Funcionamiento:
 *
 *      - Si la entrada in cambia respecto a last_input:
 *          - se guarda el instante actual en last_change_time;
 *          - se actualiza last_input.
 *
 *      - Si la entrada se mantiene sin cambios durante al menos pt:
 *          - se acepta como nuevo estado estable;
 *          - stable_state toma el valor de in;
 *          - Q toma el valor de stable_state.
 *
 *      El bloque no usa delay().
 ***************************************************************/
void DEBOUNCE::update (bool in, unsigned long pt)
{
    unsigned long current_time = millis();

    if (in != last_input)
    {
        last_change_time = current_time;
        last_input = in;
    }

    if (current_time - last_change_time >= pt)
    {
        if (stable_state != last_input)
        {
            stable_state = last_input;
            q = stable_state;
        }
    }
}

/***************************************************************
 * Rutina      : Q()
 * Tipo        : Método de consulta
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - Estado estable actual de la señal.
 *
 * Descripción :
 *      Devuelve la salida estable del bloque DEBOUNCE.
 *
 *      Este método no modifica el estado interno del objeto.
 ***************************************************************/
bool DEBOUNCE::Q() const
{
  return q;
}