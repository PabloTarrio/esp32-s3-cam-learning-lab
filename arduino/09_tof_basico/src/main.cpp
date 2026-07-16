// ============================================================================
// Ejercicio 09 - TOF básico
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Implementar un temporizador tipo TOF inspirado en PLC.
//   - Representar un bloque temporizador con estado interno usando una clase C++.
//   - Practicar los conceptos IN, PT, Q y ET aplicados a retardo a la desconexión.
//   - Usar millis() para medir tiempo sin bloquear el programa.
//   - Mantener una salida activa durante un tiempo después de desactivar la entrada.
//   - Preparar el camino hacia TP y una futura librería local lib/plc_blocks.
//
// Hardware:
//   - Pulsador externo conectado entre GPIO3 y GND.
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//
// Comportamiento esperado:
//   - Mientras el pulsador está activo, Q = true.
//   - Cuando el pulsador se libera, ET empieza a contar.
//   - Mientras ET < PT, Q sigue en true.
//   - Cuando ET >= PT, Q pasa a false.
//   - Cuando Q = true, el RGB integrado permanece verde.
//   - Cuando Q = false, el RGB integrado se apaga.
//
// Equivalencia PLC:
//   - IN : entrada del temporizador.
//   - PT : preset time / tiempo programado.
//   - Q  : salida del temporizador.
//   - ET : elapsed time / tiempo transcurrido.
//
// Reglas del ejercicio:
//   - No usar delay().
//   - No bloquear loop().
//   - Usar millis() para medir tiempo.
//   - Encapsular la lógica del temporizador en una clase TOF.
//   - Mantener la lectura del pulsador separada de la lógica del temporizador.
//   - Mantener el control del RGB separado de la lógica del temporizador.
//
// Conceptos trabajados:
//   - Clase C++.
//   - Estado interno.
//   - Temporizador TOF.
//   - millis().
//   - IN, PT, Q y ET.
//   - Retardo a la desconexión.
//   - Paso de lógica PLC a C++.
//   - Ejecución cooperativa.
// ============================================================================

// ===== LIBRERIAS ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// === CLASES =================================================================
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

// ===== DEFINICIONES ==========================================================
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// ===== CONSTANTES ===========================================================
const unsigned long DEBOUNCE_TIME_MS = 50;

// ===== VARIABLES GLOBALES ===================================================
TOF tof_boton;
Adafruit_NeoPixel pixel (NUM_LEDS, RGB_PIN, NEO_GRB+NEO_KHZ800);

// === INICIALIZACION ==========================================================
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.setBrightness(20);
  pixel.show();

  Serial.println("Ejercicio 09 - Temporizador TOF - Básico");
  Serial.print("BUTTON PIN = GPIO ");
  Serial.print(BUTTON_PIN);
}

// === BUCLE PRINCIPAL =========================================================
void loop() {
  bool boton_pulsado = (digitalRead(BUTTON_PIN) == LOW);

  tof_boton.update(boton_pulsado, 2000);

  if(tof_boton.Q())
  {
    pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  }
  else
  {
    pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  }
  pixel.show();

  Serial.print("Elapsed Time: ");
  Serial.print(tof_boton.ET());
  Serial.print(" ms. | Salida TON: ");
  Serial.println(tof_boton.Q());  
}