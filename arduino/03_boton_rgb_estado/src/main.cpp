// ============================================================================
// Ejercicio 03 - Lectura de botón GPIO
// Ejercicio de consolidación de Fase 2
// Placa: ESP32-S3-CAM
//
// Objetivo:
//    El objetivo no es aprender algo completamente nuevo, sino consolidar:
//          - lectura de botón;
//          - antirrebote;
//          - detección de flanco;
//          - cambio de estado;
//          - control de RGB;
//          - funciones limpias;
//          - variables globales necesarias;
//          - variables locales temporales;
//          - paso por referencia;
//          - documentación mínima.
// Reglas del ejercicio:
//          - No usar delay
//          - Solo cambiar el color con el flanco ascendente
//          - No imprimir dentro de aplicar_color()
// ============================================================================

// ===== Librerías ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ====== Definiciones ========================================================
#define BUTTON_PIN  3
#define RGB_PIN     48
#define NUM_LEDS    1

// ===== Constantes y Variables ===============================================
Adafruit_NeoPixel pixel (NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
int color_index = 0;
const int MAX_COLOR_INDEX = 4;

const unsigned long DEBOUNCE_TIME_MS = 50;
unsigned long last_debounce_time = 0;

int button_state = HIGH;
int previous_button_state = HIGH;
int last_reading = HIGH;
int pulsacion = 0;
// ===== Protipado de Funciones ===============================================
// Definir color segun numero de pulsación
void aplicar_color( Adafruit_NeoPixel &rgbled, int index);
// Cálculo del color.
void cambiar_color( Adafruit_NeoPixel &rgb_led, int &index);

// ===== INICIALIZACION =======================================================
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.show();

  aplicar_color(pixel, color_index);

  Serial.println("Ejercicio 03 - Boton RGB Estado");
  Serial.print("GPIO Boton: ");  
  Serial.println(BUTTON_PIN);
  Serial.print("GPIO RGB: ");  
  Serial.println(RGB_PIN);
}

// ===== BUCLE PRINCIPAL ======================================================
void loop() {
  
  int current_reading = digitalRead(BUTTON_PIN);

  if (current_reading != last_reading){
    last_debounce_time = millis();
  }

  if (millis() - last_debounce_time >= DEBOUNCE_TIME_MS){
    
    if (current_reading != button_state){
      previous_button_state = button_state;
      button_state = current_reading;

      if (previous_button_state == LOW && button_state == HIGH){
        pulsacion++;
        cambiar_color(pixel, color_index);
        Serial.print("\n· Pulsación número: ");
        Serial.println(pulsacion);
        Serial.print("· Color index: ");
        Serial.println(color_index);
      }
    }
  }
  last_reading = current_reading;
}


// ===== Declaración de Funciones ==============================================
void aplicar_color(Adafruit_NeoPixel &rgb_led, int index){
  rgb_led.setBrightness(20);

  switch(index)
  {
    case 0:
      rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
      break;
    case 1:
      rgb_led.setPixelColor(0, rgb_led.Color(0, 255, 0));
      break;
    case 2:
      rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 255));
      break;
    case 3:
      rgb_led.setPixelColor(0, rgb_led.Color(255, 255, 0));
      break;
    case 4:
      rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 0));
      break;
    default:
      rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 0));
      break;
  }
  rgb_led.show();
}

void cambiar_color(Adafruit_NeoPixel &rgb_led, int &index){
  index++;

  if (index > MAX_COLOR_INDEX ){
    index = 0;
  }

  aplicar_color(rgb_led, index);
}
