// ============================================================================
// Ejercicio 11 - R_TRIG y F_TRIG básicos
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Implementar detectores de flanco inspirados en PLC.
//   - Crear un bloque R_TRIG para detectar flanco ascendente.
//   - Crear un bloque F_TRIG para detectar flanco descendente.
//   - Representar cada detector como una clase C++ con estado interno.
//   - Practicar el uso de memoria interna para comparar el estado actual
//     de una señal con su estado anterior.
//   - Probar los detectores de flanco con un pulsador externo en GPIO3.
//   - Preparar el camino hacia una futura librería local lib/plc_blocks.
//
// Hardware:
//   - Pulsador externo conectado entre GPIO3 y GND.
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//
// Lógica del pulsador:
//   - Se usa INPUT_PULLUP.
//   - Sin pulsar -> HIGH.
//   - Pulsado    -> LOW.
//
// Señal lógica usada en el programa:
//   - boton_pulsado = true  cuando digitalRead(BUTTON_PIN) == LOW.
//   - boton_pulsado = false cuando digitalRead(BUTTON_PIN) == HIGH.
//
// Flancos sobre la señal boton_pulsado:
//   - false -> true  : pulsación del botón.
//   - true  -> false : liberación del botón.
//
// Comportamiento esperado:
//   - R_TRIG detecta un flanco ascendente:
//       false -> true
//     En este ejercicio corresponde a pulsar el botón.
//
//   - F_TRIG detecta un flanco descendente:
//       true -> false
//     En este ejercicio corresponde a liberar el botón.
//
//   - La salida Q de cada bloque debe estar activa solo durante un ciclo
//     de loop() cuando se detecta su flanco correspondiente.
//
// Equivalencia PLC:
//   - R_TRIG : detector de flanco ascendente.
//   - F_TRIG : detector de flanco descendente.
//   - CLK    : señal booleana de entrada.
//   - Q      : salida activa durante un ciclo cuando se detecta el flanco.
//
// Reglas del ejercicio:
//   - No usar delay().
//   - No bloquear loop().
//   - No usar interrupciones.
//   - Detectar los flancos mediante comparación entre estado actual
//     y estado anterior.
//   - Encapsular la lógica de flanco en clases reutilizables.
//   - Mantener la lectura del pulsador separada de la lógica de flanco.
//   - Mantener el control del RGB separado de los bloques de flanco.
//
// Conceptos trabajados:
//   - Clase C++.
//   - Estado interno.
//   - Flanco ascendente.
//   - Flanco descendente.
//   - R_TRIG.
//   - F_TRIG.
//   - Memoria del estado anterior.
//   - Señales booleanas.
//   - Separación entre entrada, detección de evento y acción.
//   - Preparación para lib/plc_blocks.
// ============================================================================
// ===== LIBRERIAS ============================================================
#include <Arduino.h>

// === CLASES =================================================================
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

/***************************************************************
 * Rutina      : R_TRIG()      Tipo        : Constructor
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - Ninguno
 * Descripción :
 *      Inicializa el detector de flanco ascendente en reposo.
 ***************************************************************/
R_TRIG::R_TRIG()
{
  q = false;
  previous_clk = false;
}

/***************************************************************
 * Rutina      : update()   * Tipo        : Método
 * Parámetros  :
 *       - clk : señal booleana de entrada.
 * 
 * Retorno     :
 *       - Ninguno
 * 
 * Descripción :
 *       Actualiza el estado interno del detector R_TRIG.
 * 
 *       Debe llamarse de forma cíclica desde loop().
 * **************************************************************/
void R_TRIG::update (bool clk)
{
  if (!previous_clk && clk)
  {
    q = true;
  }
  else
  {
    q = false;
  }
  previous_clk = clk;
}

/***************************************************************
 * Rutina      : Q()
 * Tipo        : Método de consulta
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - true  : se ha detectado flanco ascendente en este ciclo.
 *      - false : no se ha detectado flanco ascendente.
 * Descripción :
 *      Devuelve el estado actual de la salida Q.
 *
 *      Este método no modifica el estado interno del objeto.
 ***************************************************************/
bool R_TRIG::Q () const{
  return q;
}

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

/***************************************************************
 * Rutina      : F_TRIG()           Tipo        : Constructor
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - Ninguno
 * Descripción :
 *      Inicializa el detector de flanco descendente en reposo.
 ***************************************************************/
F_TRIG::F_TRIG()
{
  q = false;
  previous_clk = false;
}

/***************************************************************
 *  Rutina      : update()  Tipo        : Método
 *  Parámetros  :
 *       - clk : señal booleana de entrada.
 * 
 * Retorno     :
 *       - Ninguno
 * 
 * Descripción :
 *       Actualiza el estado interno del detector F_TRIG.
 * 
 *       Debe llamarse de forma cíclica desde loop().
 * **************************************************************/
void F_TRIG::update(bool clk)
{
  if(previous_clk && !clk)
  {
    q = true;
  }
  else
  {
    q = false;
  }
  previous_clk = clk;
}

/***************************************************************
 * Rutina      : Q()
 * Tipo        : Método de consulta
 * Parámetros  :
 *      - Ninguno
 * Retorno     :
 *      - true  : se ha detectado flanco descendente en este ciclo.
 *      - false : no se ha detectado flanco descendente.
 * Descripción :
 *      Devuelve el estado actual de la salida Q.
 *
 *      Este método no modifica el estado interno del objeto.
 ***************************************************************/
bool F_TRIG::Q() const
{
  return q;
}

// ===== DEFINICIONES ==========================================================
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// ===== VARIABLES GLOBALES ===================================================
R_TRIG r_trig_boton;
F_TRIG f_trig_boton;

// === INICIALIZACION ==========================================================
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Ejercicio 11 - Detección flanco básico");
  Serial.print("BUTTON PIN = GPIO ");
  Serial.println(BUTTON_PIN);
}

// === BUCLE PRINCIPAL =========================================================
void loop() {
  bool boton = (digitalRead(BUTTON_PIN) == LOW);

  r_trig_boton.update(boton);
  f_trig_boton.update(boton);

  if(r_trig_boton.Q())
  {
    Serial.println("R_TRIG: Flanco ascendente detectado.");
  }

  if(f_trig_boton.Q())
  {
    Serial.println("F_TRIG: Flanco descendente detectado.");
  }
}