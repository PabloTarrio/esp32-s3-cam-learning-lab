// ============================================================================
// Ejercicio 13 - DEBOUNCE con plc_blocks
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Validar el bloque DEBOUNCE de la librería local lib/plc_blocks.
//   - Filtrar una señal booleana procedente de un pulsador físico.
//   - Combinar DEBOUNCE con R_TRIG para detectar una pulsación válida.
//   - Combinar R_TRIG con TP para generar un pulso temporizado.
//   - Controlar el RGB integrado mediante la salida Q del TP.
//   - Consolidar la cadena GPIO -> DEBOUNCE -> R_TRIG -> TP -> salida.
//
// Hardware:
//   - Pulsador externo conectado entre GPIO3 y GND.
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//
// Funcionamiento esperado:
//   - El GPIO se lee como señal física raw.
//   - DEBOUNCE filtra los rebotes del pulsador.
//   - R_TRIG detecta el flanco ascendente de la señal estable.
//   - TP genera un pulso de 2000 ms.
//   - Mientras TP.Q() es true, el RGB integrado permanece verde.
//   - Al terminar el pulso, el RGB se apaga.
//
// Librería usada:
//   - lib/plc_blocks
//
// Includes:
//   - PlcDebounce.h
//   - PlcEdges.h
//   - PlcTimers.h
//
// Reglas del ejercicio:
//   - No usar delay().
//   - No bloquear loop().
//   - No implementar antirrebote local en main.cpp.
//   - Usar DEBOUNCE como bloque reutilizable.
//   - Mantener la lectura GPIO separada del filtrado.
//   - Mantener la detección de flanco separada del antirrebote.
//   - Mantener la temporización separada de la detección de flanco.
//
// Conceptos trabajados:
//   - DEBOUNCE.
//   - Antirrebote reutilizable.
//   - R_TRIG.
//   - TP.
//   - Señal raw.
//   - Señal estable.
//   - Bloques PLC en C++.
//   - Librería local PlatformIO.
// ============================================================================

// ===== LIBRERIAS ============================================================
#include <Arduino.h>
#include <PlcDebounce.h>
#include <PlcEdges.h>
#include <PlcTimers.h>
#include <Adafruit_NeoPixel.h>

// ===== DEFINICIONES =========================================================
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// ===== CONSTANTES ===========================================================
const unsigned long DEBOUNCE_TIME_MS = 50;
const unsigned long TP_TIME_MS = 2000;

// ===== VARIABLES GLOBALES ===================================================
Adafruit_NeoPixel pixel (NUM_LEDS, RGB_PIN, NEO_GRB+NEO_KHZ800);
DEBOUNCE debounce_boton;
R_TRIG r_trig_boton;
TP tp_boton;

// ===== INICIALIACIÓN ========================================================
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.setBrightness(20);
  pixel.show();

  Serial.println("Ejercicio 13 - PLC Debounce");
  Serial.print("BUTTON_PIN = GPIO");
  Serial.println(BUTTON_PIN);
  Serial.print("RGB_PIN = GPIO");
  Serial.println(RGB_PIN);
}

// ===== BUCLE PRINCIPAL ======================================================
void loop() {
  bool boton = (digitalRead(BUTTON_PIN) == LOW);

  r_trig_boton.update(debounce_boton.Q());
  tp_boton.update(r_trig_boton.Q(), TP_TIME_MS);

  if (tp_boton.Q())
  {
      pixel.setPixelColor(0, pixel.Color(0, 255, 0));
  }
  else
  {
      pixel.setPixelColor(0, pixel.Color(0, 0, 0));
  }
  pixel.show();
}