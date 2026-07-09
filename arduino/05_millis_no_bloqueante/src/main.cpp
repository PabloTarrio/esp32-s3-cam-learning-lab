// ============================================================================
// Ejercicio 05 - millis() no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Hacer parpadear el LED RGB integrado sin usar delay().
//   - Introducir temporización no bloqueante con millis().
// ============================================================================

// ===== LIBRERIAS ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ===== DEFINICIONES =========================================================
#define RGB_PIN 48
#define NUM_LEDS 1

// ===== CONSTANTES ===========================================================
const unsigned long BLINK_INTERVAL_MS = 500;

// ===== VARIABLES GLOBALES ===================================================
Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

unsigned long previous_millis = 0;
bool led_state = false;

// ===== PROTOTIPOS ===========================================================
void aplicar_estado_rgb(Adafruit_NeoPixel &rgb_led, bool estado);

// ===== INICIALIACIÓN ========================================================
void setup()
{
  Serial.begin(115200);

  pixel.begin();
  pixel.clear();
  pixel.show();

  Serial.println("Ejercicio 05 - millis() no bloqueante");
  Serial.println("RGB integrado parpadeando sin delay");  
}

// ===== BUCLE PRINCIPAL ======================================================
void loop()
{
  unsigned long current_millis = millis();

  if (current_millis - previous_millis >= BLINK_INTERVAL_MS)
  {
    previous_millis = current_millis;
    
    led_state = !led_state;

    aplicar_estado_rgb(pixel, led_state);

    Serial.print("LED state:");
    Serial.println(led_state ? "ON" : "OFF");
  }  
}

// ===== FUNCIONES Y RUTINAS ==================================================
void aplicar_estado_rgb(Adafruit_NeoPixel &rgb_led, bool estado)
{
  rgb_led.setBrightness(20);

  if (estado)
  {
    rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 255));
  }
  else
  {
    rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 0));
  }
  rgb_led.show();
}

