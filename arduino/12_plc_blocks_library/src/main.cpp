// ============================================================================
// Ejercicio 12 - Validación de librería local plc_blocks
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Validar el uso de la librería local lib/plc_blocks.
//   - Importar bloques PLC reutilizables desde PlcTimers.h y PlcEdges.h.
//   - Usar R_TRIG para detectar una pulsación del botón.
//   - Usar TP para generar un pulso temporizado.
//   - Controlar el RGB integrado mediante la salida Q del TP.
//   - Confirmar que PlatformIO encuentra correctamente la librería local.
//
// Hardware:
//   - Pulsador externo conectado entre GPIO3 y GND.
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//
// Funcionamiento esperado:
//   - Al pulsar el botón, R_TRIG detecta un flanco ascendente.
//   - Ese flanco dispara un TP de 2000 ms.
//   - Mientras TP.Q() es true, el RGB integrado permanece verde.
//   - Al terminar el pulso, el RGB se apaga.
//   - Mantener el botón pulsado no reinicia el pulso.
//   - Para generar otro pulso, hay que soltar y volver a pulsar.
//
// Librería usada:
//   - lib/plc_blocks
//
// Includes:
//   - PlcTimers.h
//   - PlcEdges.h
//
// Conceptos trabajados:
//   - Librería local PlatformIO.
//   - Reutilización de clases.
//   - Bloques PLC en C++.
//   - R_TRIG.
//   - TP.
//   - Separación entre librería y ejercicio de uso.
// ============================================================================

// ===== LIBRERIAS ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <PlcTimers.h>
#include <PlcEdges.h>

// ===== DEFINICIONES =========================================================
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// ===== CONSTANTES ===========================================================
const unsigned long TP_TIME_MS = 2000;

// ===== VARIABLES GLOBALES ===================================================
Adafruit_NeoPixel pixel (NUM_LEDS, RGB_PIN, NEO_GRB+NEO_KHZ800);
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

    Serial.println("Ejercicio 12 - PLC Blocks LIbrary");
    Serial.print("BUTTON_PIN = GPIO");
    Serial.println(BUTTON_PIN);
    Serial.print("RGB_PIN = GPIO");
    Serial.println(RGB_PIN);
}

// ===== BUCLE PRINCIPAL ======================================================
void loop() {

    bool boton = (digitalRead(BUTTON_PIN) == LOW);

    r_trig_boton.update(boton);
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