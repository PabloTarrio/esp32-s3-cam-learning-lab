// ============================================================================
// Ejercicio 05b - RGB y botón sin bloqueos
// Fase 4 - Temporización no bloqueante.
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Hacer parpadear el LED RGB integrado sin usar delay().
//   - Leer  un pulsador externo sin bloquear el programa.
//   - Cambiar el modo de funcionamiento con cada pulsación válida. 
//   - Consolidar millis(), antirrebote, flancos y variables de estado.
// ============================================================================

// ===== Librerías =============================================================14
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ===== Definiciones ==========================================================19
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// ===== CONSTANTES ===========================================================
const unsigned long BLINK_INTERVAL_MS = 500;
const unsigned long DEBOUNCE_TIME_MS = 50;

const int MAX_MODE = 2;

// ===== VARIABLES GLOBALES ===================================================
Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

bool blink_state = false;
unsigned long previous_blink_millis = 0;

int button_state = HIGH;
int previous_button_state = HIGH;
int last_reading = HIGH;
unsigned long last_debounce_time = 0;

int current_mode = 0;

unsigned long valid_press_counter = 0;

// ===== PROTOTIPOS ===========================================================
void aplicar_color(Adafruit_NeoPixel &rgb_led, uint32_t color);
void actualizar_parpadeo_rgb(Adafruit_NeoPixel &rgb_led);
bool detectar_pulsacion_valida();
void cambiar_modo();
void aplicar_modo_actual(Adafruit_NeoPixel &rgb_led);

// ===== INICIALIACIÓN ========================================================
void setup() {
  Serial.begin(115200);

  pinMode (BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.setBrightness(20);
  pixel.show();

  aplicar_modo_actual(pixel);

  Serial.println("Ejercicio 05b - RGB y boton sin bloqueos");
  Serial.print("BUTTON_PIN = GPIO");
  Serial.println(BUTTON_PIN);
  Serial.print("RGB_PIN = GPIO");
  Serial.println(RGB_PIN);
  Serial.println("Modo inicial: 0 -> parpadeo azul");
}

// ===== BUCLE PRINCIPAL ======================================================
void loop() {
  // Tarea 1: Leer botón y detectar pulsación válida.
  if (detectar_pulsacion_valida())
  {
    valid_press_counter++;

    cambiar_modo();
    aplicar_modo_actual(pixel);

    Serial.print("Pulsación válida numero: ");
    Serial.println(valid_press_counter);

    Serial.print("Modo actual: ");
    Serial.print(current_mode);

    if (current_mode == 0)
    {
      Serial.println(" -> Azul parpadeo");
    }
    else if (current_mode == 1)    
    {
      Serial.println(" -> Verde fijo");
    }
    else if (current_mode == 2)
    {
      Serial.println(" -> Apagado");
    }
    
  }

  // Tarea 2: actualizar parpadeo solo si estamos en modo 0
  if (current_mode == 0)
  {
    actualizar_parpadeo_rgb(pixel);
  } 
}

// ===== FUNCIONES Y RUTINAS ==================================================
/********************************************************************
 * Rutina      : aplicar_color
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - rgb_led : referencia al objeto NeoPixel que controla el RGB
 *   - color   : color codificado como valor uint32_t
 * Retorna     :
 *   - Nada
 * Descripción :
 *    Aplica un color al LED RGB integrado y actualiza físicamente
 *    el NeoPixel mediante show().
 * ******************************************************************/
void aplicar_color(Adafruit_NeoPixel &rgb_led, uint32_t color)
{
  rgb_led.setPixelColor(0, color);
  rgb_led.show();
}

/********************************************************************
 * Rutina      : aplicar_parpadeo_rgb
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - rgb_led : referencia al objeto NeoPixel que controla el RGB
 * Retorna     :
 *   - Nada
 * Descripción :
 *  Gestiona el parpadeo no bloqueante del LED RGB integrado.
 *  Usa millis() para alternan entre encendido y apagado sin
 *    detener la ejecución del programa. 
 * ******************************************************************/
void actualizar_parpadeo_rgb(Adafruit_NeoPixel &rgb_led)
{
  unsigned long current_millis = millis();

  if (current_millis - previous_blink_millis >= BLINK_INTERVAL_MS)
  {
    previous_blink_millis = current_millis;
    blink_state = !blink_state;

    if (blink_state)
    {
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 255));
    }
    else
    {
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
    }
  }
}

/********************************************************************
 * Rutina      : detectar_pulsación_valida
 * Tipo        : Función/Rutina
 * Parámetros  :
 *   - Nada
 * Retorna     :
 *   - true -> Se ha detectado pulsación válida.
 *   - false -> No se ha detectado pulsación válida.
 * Descripción :
 *    Lee el pulsador conectado a BUTTON_PIN, comprueba el antirrebote
 * mediante millis() y devuelve true solo cuando se detecta una 
 * liberación válida del pulsador.
 * ******************************************************************/
bool detectar_pulsacion_valida()
{
  int current_reading = digitalRead(BUTTON_PIN);

  // Reiniciar el temporizador antirrebote  
  if (current_reading != last_reading)
  {
    last_debounce_time = millis();
  }

  // Si la lectura se mantiene estable un tiempo suficiente
  if ((millis() - last_debounce_time) >= DEBOUNCE_TIME_MS)
  {
    // aceptamos la lectura como correcta
    if (current_reading != button_state)
    {
      previous_button_state = button_state;
      button_state = current_reading;

      // INPUT_PULLUP  
      if (previous_button_state == LOW && button_state == HIGH)
      {
        last_reading = current_reading;
        return true;
      }
    }
  }
  last_reading = current_reading;
  return false;
}

/********************************************************************
 * Rutina      : cambiar modo
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - Nada
 * Retorna     :
 *   - Nada
 * Descripción :
 *  Incrementa el modo actual del sistema. Si se supera el modo
 * máximo permitido, vuelve al modo inicial. 
 * ******************************************************************/
void cambiar_modo()
{
  current_mode++;

  if (current_mode > MAX_MODE)
  {
    current_mode = 0;
  }
}

/********************************************************************
 * Rutina      : aplicar_modo_actual
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - rgb_led : referencia al objeto NeoPixel que controla el RGB
 * Retorna     :
 *   - Nada
 * Descripción :
 *  Aplica la salida correspondiente al modo actual del sistema.
 * 
 *    Modo 0: parpadeo azul no bloqueante.
 *    Modo 1: verde fijo
 *    Modo 2: apagado 
 * ******************************************************************/
void aplicar_modo_actual(Adafruit_NeoPixel &rgb_led)
{
  switch (current_mode)
  {
    case 0:    
      previous_blink_millis = millis();
      blink_state = false;
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
      break;
    case 1:
      aplicar_color(rgb_led, rgb_led.Color(0, 255, 0));
      break;
    case 2:
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
      break;
    default:
      current_mode = 0;
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
      break;
  }
}