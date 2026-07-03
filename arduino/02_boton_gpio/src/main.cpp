// ============================================================================
// Ejercicio 02 - Lectura de botón GPIO
// Placa: ESP32-S3-CAM
//
// Objetivo:
//    - Leer un botón externo mediante GPIO
//    - Usar INPUT PULLUP
//    - Filtrar el rebote mecánico
//    - Detectar el flanco descendente: HIGH -> LOW
//    - Cambiar el color del led integrado con pulsaciones.
// ============================================================================

// ===== Librerías ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ====== Definiciones ========================================================
#define BUTTON_PIN 3
#define RGB_PIN 48
#define NUM_LEDS 1

// ===== Constantes y Variables ===============================================
// Objeto NeoPixel
Adafruit_NeoPixel pixel (NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
int color_index = 0;
const int MAX_COLOR_INDEX = 4;

// Tiempo minimo estabilidad en una lectura/pulsación para considerarla válida.
const unsigned long DEBOUNCE_TIME_MS = 50;
int button_state = HIGH;            // estado confirmado del boton
int previous_button_state = HIGH;  // estado anterior del boton
int last_reading = HIGH;            // última lectura directa del GPIO
// Momento en el que detectamos el úlitmo cambio de lectura
unsigned long last_debounce_time = 0;   

// ===== Protipado de Funciones ===============================================
// Aplicar color segun el numero de pulsacion
void aplicar_color(Adafruit_NeoPixel &rgb_led, int index);
// Calculo del color
void cambiar_color(Adafruit_NeoPixel &rgb_led, int &index);

// ===== INICIALIZACION =======================================================
void setup() {
  Serial.begin(115200);

  // Boton conectado entre GND y GPIO3 -> resistencia PULLUP interna  
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.show();

  aplicar_color(pixel, color_index);

  Serial.println("Ejercicio 02 - Boton GPIO");
  Serial.print("Probando BUTTON_PIN = GPIO ");
  Serial.println(BUTTON_PIN);
  
}

// ===== BUCLE PRINCIPAL ======================================================
void loop() {
  int current_reading = digitalRead(BUTTON_PIN);
  
  // Actualizar temporizador antirebote ante cambio en la lectura
  if (current_reading != last_reading){
    last_debounce_time = millis();
  }

  // Si la lectura se mantiene estable aceptamos el cambio como válido
  if ((millis() - last_debounce_time) > DEBOUNCE_TIME_MS)
  {
    if (current_reading != button_state){
      // Guardamos el estado anterior del boton y actualizamos el actual
      previous_button_state = button_state;
      button_state = current_reading;

      if (previous_button_state == HIGH && button_state == LOW){
        Serial.println("Flanco descendente: Pulsación Válida");
        cambiar_color(pixel, color_index);
      }

      if (previous_button_state == LOW && button_state == HIGH){
        Serial.println("Flanco ascendente: Pulsación Liberado");
      }
    }
  }
  // Guardamos la lectura actual como la última para posteriores comparaciones.
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

  Serial.print("Color index: ");
  Serial.println(index);
}
