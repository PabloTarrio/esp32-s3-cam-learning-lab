#ifndef PLC_TIMERS_H
#define PLC_TIMERS_H

#include <Arduino.h>

// ============================================================================
// PlcTimers.
// Librería local: plc_blocks
//
// Descripción:
//   Declaración de temporizadores inspirados en bloques PLC.
//
// Bloques incluidos:
//   - TON : Temporizador a la conexión.
//   - TOF : Temporizador a la desconexión.
//   - TP  : Temporizador de pulso.
//
// Notas:
//   - Los temporizadores no usan delay().
//   - Todos deben actualizarse de forma cíclica mediante update().
//   - El tiempo se mide en milisegundos usando millis().
// ============================================================================


/*******************************************************************
 * Bloque      : TON
 * Tipo        : Temporizador a la conexión
 * Parámetros  :
 *   - IN : entrada booleana del temporizador
 *   - PT : tiempo programado en milisegundos
 * Retorna     :
 *   - Q  : salida booleana del temporizador
 *   - ET : tiempo transcurrido en milisegundos
 * Descripción :
 *   Implementa un temporizador tipo TON inspirado en PLC.
 *
 *   Cuando IN pasa a true, el temporizador empieza a contar.
 *   Si IN permanece true durante al menos PT milisegundos,
 *   la salida Q pasa a true.
 *
 *   Si IN vuelve a false antes de cumplirse PT, el temporizador
 *   se reinicia, Q vuelve a false y ET vuelve a 0.
 *
 *   El temporizador no usa delay() y se actualiza de forma
 *   cíclica mediante llamadas repetidas a update().
 *******************************************************************/
class TON
{
  private:
    bool q;                                     // Representa la salida del temporizador.
    unsigned long start_time;                   // Guarda el instante exacto en el que arranca el temporizador.
    unsigned long elapsed_time;                 // Tiempo transcurrido desde el arranque del temporizador.
    bool timing;                                // Representa si el temporizador está contando o no.
  
  public:
    TON();                                      // Constructor.
    void update (bool in, unsigned long pt);    // Actualiza el estado interno del temporizador.
    bool Q() const;                             // Devuelve el estado actual de la salida Q. 
    unsigned long ET() const;                   // Devuelve el tiempo transcurrido en el temporizador.
};

/************************************************************************
 * Bloque       : TOF
 * Tipo         : Temporizador a la desconexión
 * Parámetros   : 
 *    - IN  : entrada booleana del temporizador.
 *    - PT  : tiempo programado en milisegundos.
 * Retorna      :
 *    - Q   : salida booleana del temporizador.
 *    - ET  ; tiempo transcurrido en milisegunods.
 * Descripción :
 *  Implementa un temporizador TOF inspirado en PLC.
 * 
 *  Cuando IN pasa a TRUE, la salida Q pasa inmediatamente a TRUE
 *  y el temporizador queda reseteado.
 * 
 *  Cuando IN pasa a FALSE, el temporizador comienza a contar.
 *  Mientras ET sea menor que PT, Q permanece TRUE.
 * 
 *  Cuando ET alcanza o supera PT, Q pasa a FALSE.
 * 
 *  El temporizador no usa delay() y se actualiza de forma
 *  cíclica mediante llamadas repetidas a update().
 ***********************************************************************/
class TOF
{
  private:
    bool q;
    unsigned long start_time;
    unsigned long elapsed_time;
    bool timing;

  public:
    TOF();
    void update (bool in, unsigned long pt);
    bool Q() const;
    unsigned long ET() const;
};

/************************************************************************
 * Bloque       : TP
 * Tipo         : Genera un pulso temporizado
 * Parámetros   : 
 *    - IN  : entrada booleana del temporizador.
 *    - PT  : tiempo programado en milisegundos.
 * Retorna      :
 *    - Q   : salida booleana del temporizador.
 *    - ET  ; tiempo transcurrido en milisegunods.
 * Descripción :
 *  Implementa un temporizador TP inspirado en PLC.
 * 
 *  Cuando IN pasa desde FALSE a TRUE, la salida Q pasa inmediatamente
 *  a TRUE y comienza la temporización del pulso.
 * 
 *  Mientras ET sea menor que PT, Q permanece a TRUE.
 * 
 *  Cuando ET alcanza o supera PT, Q pasa a FALSE.
 * 
 *  Mantener IN a TRUE no reinicia el pulso. Para generar un nuevo
 *  pulso, IN debe volver a FALSE y después pasar de nuevo a TRUE.
 * 
 *  El temporizador no usa delay() y se actualiza de forma cíclica
 *  mediante llamadas repetidas a update() 
 ***********************************************************************/
class TP
{
  private:
    bool q;
    unsigned long start_time;
    unsigned long elapsed_time;
    bool timing;
    bool previous_in;
  public:
    TP();
    void update (bool in, unsigned long pt);
    bool Q() const;
    unsigned long ET() const;
};

#endif