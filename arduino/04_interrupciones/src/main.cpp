// ============================================================================
// Ejercicio 04 - Interrupciones
// Placa: ESP32-S3-CAM
//
// Objetivo:
//    - Leer un pulsador externo conectado al GPIO3-
//    - Detectar un flanco mediante interrupción.
//    - Usar una variable volatile como FLAG.
//    - Procesar el evento en loop(), no dentro de la ISR. 
//    - Filtrar rebotes en loop() mediante millis()
//    - Cambiar el color del led RGB integrado cuando la interrupción es válida
// ============================================================================

// ===== Librerías ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ===== Definiciones =========================================================
#define BUTTON_PIN 3
#define RGB_PIN 48
#define NUM_LEDS 1

// ===== Constantes ===========================================================
const unsigned long DEBOUNCE_TIME_MS = 50;
const unsigned int MAX_COLOR_INDEX = 4;

// ===== Variables globales ===================================================
// Debe declararse volatile dado que se modifica dentro de una interrupción.
//    asi indicamos al compilador que esta variable puede cambiar fuera del flujo
//    normal del programa.
volatile bool button_interrupt_pending = false;
volatile unsigned long interrupt_time = 0;

// Contador de eventos gestionado en loop(), no dentro de la ISR
unsigned long valid_interrupt_counter = 0;
unsigned long ignored_bounce_counter = 0;

int color_index = 0;
Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

// ===== Prototipos ===========================================================
void IRAM_ATTR on_button_interrupt();

void aplicar_color(Adafruit_NeoPixel &rgb_led, int index);
void cambiar_color(Adafruit_NeoPixel &rgb_led, int &index);

// ===== Inicialialización ====================================================
void setup()
{
  Serial.begin(115200);
  
  // Pulsador externo conectado entre GPIO3 y GND con INPUT_PULLUP.
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Interrupción asociada al flanco descendente
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    on_button_interrupt,
    FALLING
  );
  
  //Inicializar RGB integrado
  pixel.begin();
  pixel.clear();
  pixel.show();
  aplicar_color(pixel, color_index);

  // Imprimir información inicial
  Serial.println("Ejercicio 04 - Interrupciones");
  Serial.print("BUTTON PIN : GPIO ");
  Serial.println(BUTTON_PIN);
  Serial.print("LED RGB INTEGRADO : GPIO ");
  Serial.println(RGB_PIN);
  Serial.println("Interrupción configurada en flanco descendente");
}

// ===== Bucle Principal ======================================================
void loop() 
{
  bool pending = false;
  unsigned long captured_interrupt_time = 0;
  
  // Copiamos y limpiamos la bandera de forma protegida.
  //
  // La ISR puede modificar button_interrupt_pending en cualquier momento.
  // Por eso deshabilitamos interrupciones durante unas pocas instrucciones.
  noInterrupts();
  pending = button_interrupt_pending;
  captured_interrupt_time = interrupt_time;
  interrupts();

  // Si no hay una interrupción pendiente no hacemos nada.
  if (!pending)
  {
    return;
  }
  

  unsigned long current_time = millis();

  // Esperamos a que pase el tiempo de anti-rebote
  // IMPORTANTE: No borramos aun la FLAG
  if ((current_time - captured_interrupt_time) > DEBOUNCE_TIME_MS)
  {
    //Una vez asegurado que ha pasado el tiempo de rebote
    // limpiamos la FLAG de forma protegida
    noInterrupts();
    button_interrupt_pending = false;
    interrupts();

    // Dado que hemos elegido FALLING con INPUT_PULLUP,
    // una pulsación válida deja el pin en LOW.
    if (digitalRead(BUTTON_PIN) == LOW)
    {
      valid_interrupt_counter++;

      Serial.print("Interrupción válida. Contador: ");
      Serial.println(valid_interrupt_counter);

      cambiar_color(pixel, color_index);
    }
    else
    {
      ignored_bounce_counter++;

      Serial.print("Rebote ignorado. Total ignorados: ");
      Serial.println(ignored_bounce_counter);
    }
  }
}

// ===== Declaración de Rutinas ===============================================
void IRAM_ATTR on_button_interrupt() {
  // La ISR debe ser lo más corta posible.
  //    - No usamos Serial.print().
  //    - No usamos delay().
  //    - No cambiamos el RGB.
  //    - Unicamente marcamos la bandera para que loop() procese el evento
  button_interrupt_pending = true;
  interrupt_time = millis();
}

void aplicar_color(Adafruit_NeoPixel &rgb_led, int index)
{
  rgb_led.setBrightness(20);
  switch (index)
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
      rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
      break;
  }
  rgb_led.show();
}

void cambiar_color(Adafruit_NeoPixel &rgb_led, int &index)
{
  index++;

  if (index > MAX_COLOR_INDEX)
  {
    index = 0;
  }
  aplicar_color(rgb_led, index);
}

