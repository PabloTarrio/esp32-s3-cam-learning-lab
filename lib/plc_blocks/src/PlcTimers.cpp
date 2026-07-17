#include <PlcTimers.h>


// =============================================================
// PlcTimers.cpp
// Librería local: plc_blocks
//
// Descripción:
//   Implementación de temporizadores inspirados en bloques PLC.
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
// =============================================================

// =============================================================
// TON - Temporizador a la conexión
// =============================================================

/***************************************************************
 * Rutina   : TON()         Tipo: Constructor
 * Parámetros:
 *        · Ninguno.
 * Retorno:
 *        · Ninguno
 * Descripción: Inicializa el temporizador en estado de reposo.
 ***************************************************************/
  TON::TON(){
    q = false;
    start_time = 0;
    elapsed_time = 0;
    timing = false;
  }

/***************************************************************
 * Rutina   : update()         Tipo: Método
 * Parámetros:
 *        · in: Entrada booleana de activación del temporizador.
 *        · pt: tiempo programado en milisegundos
 * Retorno:
 *        · Ninguno
 * Descripción: Actualiza el estado interno del temporizador TON.
 *  Debe lanzarse de forma cíclica desde el bucle principal.
 *        - Si in == false:
 *              - Reinicio del temporizador.
 *              - Q = False.
 *              - ET = 0
 *              - timing = false.
 *        - Si in == true:
 *              - Si el temporizador no estaba contando,se guarda
 *                    el instante actual en start_time.
 *              - timing = true.
 *              - Si ya estaba contando, se actualiza ET
 *              - Si ET>=PT entonces Q = true.
 ***************************************************************/
void TON::update (bool in, unsigned long pt){
  unsigned long current_time = millis();

  if (in == false)
  {
    q = false;
    start_time = 0;
    elapsed_time = 0;
    timing = false;
    return;
  }
  else
  {
    if (timing == false)
    {
      start_time = current_time;
      elapsed_time = 0;
      timing = true;
      q = false;
    }
    else
    {
      elapsed_time = current_time - start_time;
      if (elapsed_time >= pt)
      {
        elapsed_time = pt;
        q = true;
      }
    }
  }
}

/***************************************************************
 * Rutina   : Q()         Tipo: Método de consulta
 * Parámetros:
 *        · Ninguno
 * Retorno:
 *        · true: El temporizador ha alcanzado el tiempo PT.
 *        · false: El temporizador no ha alcanzado el tiempo PT.
 * Descripción: Devuelve el estado actual de la salida Q.
 *              No modifica el estado interno del objeto
 ***************************************************************/
bool TON::Q () const {
  return q;
}

/***************************************************************
 * Rutina   : ET()         Tipo: Método de consulta
 * Parámetros:
 *        · Ninguno
 * Retorno:
 *        · Tiempo transcurrido en milisegundos.
 * Descripción: Devuelve el tiempo transcurrido desde que el temporizador
 *  comenzó a contar.
 *  Equivale a ET en un temporizador TON de PLC.
 *  Si in == false || temporizador reset -> return 0
 * 
 * No modifica el estado interno del objeto.
 ***************************************************************/
unsigned long TON::ET() const {
  return elapsed_time;
}

// ==============================================================
// TOF - Temporizador a la desconexión
// ==============================================================

/***************************************************************
 * Rutina   : TOF()         Tipo: Constructor
 * Parámetros:
 *        · Ninguno.
 * Retorno:
 *        · Ninguno
 * Descripción: Inicializa el temporizador en estado de reposo.
 ***************************************************************/
TOF::TOF() {
  q = false;
  start_time = 0;
  elapsed_time = 0;
  timing = false;
}

/***************************************************************
 * Rutina       : update()        Tipo  : Método
 * Parámetros   :
 *      - in  : entrada booleana de activación del temporizador.
 *      - pt  : tiempo programado en milisegundos
 * Retorno      :
 *      - Ninguno
 * Descripción  :
 *    Actualiza el estado interno del temporizador TOF.
 * 
 *    Debe llamarse de forma cíclica desde loop().
 *      - Si in == true :
 *          - Q = true
 *          - ET = 0
 *          - timing = false.
 *          - El temporizador queda reseteado.
 * 
 *      - Si in == false y Q todavía está true :
 *          - Si timing == false  :
 *               - Se guarda el instante actual en start_time.
 *               - ET = 0
 *               - timing = true.
 * 
 *          - Si timing == true :
 *               - Se actualiza ET.
 *               - Mientras ET < PT, Q sigue true.
 *               - Si ET >= PT, Q pasa a false.
 ***************************************************************/
 void TOF::update (bool in, unsigned long pt){
  unsigned long current_time = millis();

  if (in)
  {
    q = true;
    start_time = 0;
    elapsed_time = 0;
    timing = false;
    return;
  }
  
  if (!q)
  {
    elapsed_time = 0;
    timing = false;
    return;
  }

  if (!timing)
  {
    start_time = current_time;
    elapsed_time = 0;
    timing = true;
  }
  else
  {
    elapsed_time = current_time - start_time;
  }

  if (elapsed_time >= pt)
  {
    elapsed_time = pt;
    q = false;
    timing = false;
  }  
 }

/***************************************************************
 * Rutina   : Q()         Tipo: Método de consulta
 * Parámetros:
 *        · Ninguno
 * Retorno:
 *        · true: El temporizador está activado y no ha alcanzado
 *              el tiempo PT.
 *        · false: El temporizador ha alcanzado el tiempo PT.
 * Descripción: Devuelve el estado actual de la salida Q.
 *              No modifica el estado interno del objeto
 ***************************************************************/
bool TOF::Q () const {
  return q;
}

/***************************************************************
 *  Rutina   : ET()         Tipo: Método de consulta
 *  Parámetros:
 *         · Ninguno
 *  Retorno:
 *         · Tiempo transcurrido en milisegundos.
 *  Descripción:
 *         Devuelve el tiempo transcurrido desde que comenzó
 *         la temporización de desconexión.
 * 
 *         Equivale a ET en un temporizador TOF de PLC.
 * 
 *         Si el temporizador está reseteado, devuelve 0.
 *         Mientras se temporiza la desconexión, devuelve el
 *         tiempo transcurrido.
 *         No modifica el estado interno del objeto.
 ***************************************************************/
unsigned long TOF::ET() const {
  return elapsed_time;
}

// ==============================================================
// TP - Temporizador de Pulso
// ==============================================================

/***************************************************************
 * Rutina   : TP()         Tipo: Constructor
 * Parámetros:
 *        · Ninguno.
 * Retorno:
 *        · Ninguno
 * Descripción: Inicializa el temporizador en estado de reposo.
 ***************************************************************/
TP::TP() {
  q = false;
  start_time = 0;
  elapsed_time = 0;
  timing = false;
  previous_in = false;
}

/***************************************************************
 * Rutina       : update()                Tipo         : Método
 * Parámetros:
 *      - in : entrada booleana del temporizador.
 *             Representa el estado actual de la señal de disparo.
 *
 *      - pt : tiempo programado del pulso, en milisegundos.
 *
 * Retorno      :
 *      - Ninguno.
 *
 * Descripción  : Actualiza el estado interno del temporizador TP.
 *
 *      Debe llamarse de forma cíclica desde loop().
 *
 *      Funcionamiento:
 *      - Si se detecta un flanco ascendente en la entrada:
 *
 *            previous_in == false
 *            in          == true
 *
 *        entonces:
 *
 *            - Q pasa a true.
 *            - ET se reinicia a 0.
 *            - start_time toma el valor actual de millis().
 *            - timing pasa a true.
 *
 *      - Mientras timing == true:
 *
 *            - Se actualiza ET.
 *            - Q permanece true mientras ET < PT.
 *
 *      - Cuando ET >= PT:
 *
 *            - Q pasa a false.
 *            - ET queda limitado a PT.
 *            - timing pasa a false.
 *
 *      - Mantener in en true no reinicia el pulso.
 *
 *      - Para generar un nuevo pulso, in debe volver primero a false
 *        y después pasar de nuevo a true.
 *
 *      Al final del método se actualiza previous_in con el valor
 *      actual de in para poder detectar el siguiente flanco.
 ***************************************************************/
void TP::update (bool in, unsigned long pt)
{
  unsigned long current_time = millis();

  if (in && !previous_in && !timing)
  {
    q = true;
    elapsed_time = 0;
    start_time = current_time;
    timing = true;
  }

  if (timing)
  {
    elapsed_time = current_time - start_time;
    if (elapsed_time < pt)
    {
      q = true;
    }
    else
    {
      q = false;
      elapsed_time = pt;
      timing = false;
    }
  }
  previous_in = in;
}

/***************************************************************
 * Rutina   : Q()         Tipo: Método de consulta
 * Parámetros:
 *        · Ninguno
 * Retorno:
 *        · true: El temporizador está activado y no ha alcanzado
 *              el tiempo PT.
 *        · false: El temporizador ha alcanzado el tiempo PT.
 * Descripción: Devuelve el estado actual de la salida Q.
 *              No modifica el estado interno del objeto
 ***************************************************************/
bool TP::Q () const {
  return q;
}

/***************************************************************
 *  Rutina   : ET()         Tipo: Método de consulta
 *  Parámetros:
 *         · Ninguno
 *  Retorno:
 *         · Tiempo transcurrido en milisegundos.
 *  Descripción:
 *         Devuelve el tiempo transcurrido desde que comenzó
 *         el pulso temporizado.
 * 
 *         Equivale a ET en un temporizador TP de PLC.
 * 
 *         Si el temporizador está reseteado, devuelve 0.
 *         Mientras se temporiza la desconexión, devuelve el
 *         tiempo transcurrido.
 *         No modifica el estado interno del objeto.
 ***************************************************************/
unsigned long TP::ET() const {
  return elapsed_time;
}
