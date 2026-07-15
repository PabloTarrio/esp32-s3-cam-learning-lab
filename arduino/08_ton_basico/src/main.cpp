// ============================================================================
// Ejercicio 08 - TON básico
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Implementar un temporizador tipo TON inspirado en PLC.
//   - Representar un bloque temporizador con estado interno usando una clase C++.
//   - Practicar los conceptos IN, PT, Q y ET.
//   - Usar millis() para medir tiempo sin bloquear el programa.
//   - Activar una salida cuando una entrada permanece activa durante un tiempo.
//   - Preparar el camino hacia temporizadores TOF, TP y una futura librería
//     local lib/plc_blocks.
//
// Hardware:
//   - Pulsador externo conectado entre GPIO3 y GND.
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//
// Comportamiento esperado:
//   - Mientras el pulsador no está activo, Q = false y ET = 0.
//   - Cuando el pulsador se mantiene activo, ET empieza a contar.
//   - Si el pulsador permanece activo durante PT, Q pasa a true.
//   - Si el pulsador se suelta antes de PT, el TON se reinicia.
//   - Cuando Q = true, el RGB integrado cambia a verde.
//   - Cuando Q = false, el RGB integrado permanece apagado o en otro estado.
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
//   - Encapsular la lógica del temporizador en una clase TON.
//   - Mantener la lectura del pulsador separada de la lógica del temporizador.
//   - Mantener el control del RGB separado de la lógica del temporizador.
//
// Conceptos trabajados:
//   - Clase C++.
//   - Estado interno.
//   - Temporizador TON.
//   - millis().
//   - IN, PT, Q y ET.
//   - Paso de lógica PLC a C++.
//   - Ejecución cooperativa.
// ============================================================================

// ===== LIBRERIAS ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// === CLASES =================================================================
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

// ===== DEFINICIONES ==========================================================
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// === CONSTANTES ==============================================================
const unsigned long DEBOUNCE_TIME_MS = 50;

// ==== VARIABLES GLOBALES =====================================================
TON ton_boton;
Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB+NEO_KHZ800);

// === INICIALIZACION ==========================================================
// =============================================================================
void setup() {
  Serial.begin(115200);
  pinMode (BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.setBrightness(20);
  pixel.show();

  Serial.println("Ejercicio 08 - Temporizador TON - Básico");
  Serial.print("BUTTON_PIN = GPIO");
  Serial.println(BUTTON_PIN);
}

// === BUCLE PRINCIPAL =========================================================
// =============================================================================
void loop() {
  bool boton_pulsado = (digitalRead(BUTTON_PIN) == LOW);

  ton_boton.update(boton_pulsado, 2000);
  if (ton_boton.Q())
  {
    pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  }
  else
  {
    pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  }
  pixel.show();

  Serial.print("Elapsed Time: ");
  Serial.print(ton_boton.ET());
  Serial.print(" ms. | Salida TON: ");
  Serial.println(ton_boton.Q());
}

// === FUNCIONES Y RUTINAS =====================================================
