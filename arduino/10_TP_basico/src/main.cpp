// ============================================================================
// Ejercicio 10 - TP básico
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Implementar un temporizador tipo TP inspirado en PLC.
//   - Representar un bloque temporizador de pulso usando una clase C++.
//   - Practicar los conceptos IN, PT, Q y ET aplicados a un pulso temporizado.
//   - Detectar el flanco de activación de la entrada.
//   - Usar millis() para medir tiempo sin bloquear el programa.
//   - Generar una salida activa durante un tiempo fijo PT.
//   - Preparar el camino hacia detectores de flanco R_TRIG y F_TRIG,
//     y hacia una futura librería local lib/plc_blocks.
//
// Hardware:
//   - Pulsador externo conectado entre GPIO3 y GND.
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//
// Comportamiento esperado:
//   - Cuando IN pasa de false a true, Q pasa a true inmediatamente.
//   - Q permanece true durante PT milisegundos.
//   - Cuando ET alcanza PT, Q pasa a false.
//   - Mantener IN en true no reinicia el pulso.
//   - Para generar un nuevo pulso, IN debe volver a false y después a true.
//   - Cuando Q = true, el RGB integrado cambia a verde.
//   - Cuando Q = false, el RGB integrado permanece apagado.
//
// Equivalencia PLC:
//   - IN : entrada del temporizador.
//   - PT : preset time / tiempo programado.
//   - Q  : salida del temporizador.
//   - ET : elapsed time / tiempo transcurrido del pulso.
//
// Reglas del ejercicio:
//   - No usar delay().
//   - No bloquear loop().
//   - Usar millis() para medir tiempo.
//   - Encapsular la lógica del temporizador en una clase TP.
//   - Detectar internamente el flanco de activación de IN.
//   - Mantener la lectura del pulsador separada de la lógica del temporizador.
//   - Mantener el control del RGB separado de la lógica del temporizador.
//
// Conceptos trabajados:
//   - Clase C++.
//   - Estado interno.
//   - Temporizador TP.
//   - Pulso temporizado.
//   - Detección de flanco.
//   - millis().
//   - IN, PT, Q y ET.
//   - Paso de lógica PLC a C++.
//   - Ejecución cooperativa.
// ============================================================================

// ===== LIBRERIAS ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// === CLASES =================================================================
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

// ===== DEFINICIONES ==========================================================
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// ===== VARIABLES GLOBALES ===================================================
TP tp_boton;
Adafruit_NeoPixel pixel (NUM_LEDS, RGB_PIN, NEO_GRB+NEO_KHZ800);


// === INICIALIZACION ==========================================================
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.setBrightness(20);
  pixel.show();

  Serial.println("Ejercicio 10 - Temporizador TP - Básico");
  Serial.print("BUTTON PIN = GPIO ");
  Serial.print(BUTTON_PIN);
}

// === BUCLE PRINCIPAL =========================================================
void loop() {
  bool boton_pulsado = (digitalRead(BUTTON_PIN) == LOW);

  tp_boton.update(boton_pulsado, 2000);

  if(tp_boton.Q())
  {
    pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  }
  else
  {
    pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  }
  pixel.show();

  Serial.print("Elapsed Time: ");
  Serial.print(tp_boton.ET());
  Serial.print(" ms. | Salida TP: ");
  Serial.println(tp_boton.Q()); 
}