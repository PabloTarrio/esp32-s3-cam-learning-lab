// ============================================================================
// Ejercicio 05c - Tareas cooperativas en loop()
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Ejecutar varias tareas dentro de loop() sin usar delay().
//   - Mantener el RGB integrado funcionando con temporización no bloqueante.
//   - Leer un pulsador externo con antirrebote sin bloquear el programa.
//   - Imprimir información periódica por Serial sin detener otras tareas.
//   - Consolidar el patrón de ejecución cooperativa basado en millis().
//   - Preparar el camino hacia máquinas de estado y temporizadores tipo PLC.
//
// Tareas cooperativas del ejercicio:
//   - Tarea 1: detectar pulsaciones válidas del botón en GPIO3.
//   - Tarea 2: actualizar el comportamiento del RGB integrado en GPIO48.
//   - Tarea 3: imprimir periódicamente el estado actual por monitor serie.
//
// Hardware:
//   - Pulsador externo conectado entre GPIO3 y GND.
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//
// Lógica del pulsador:
//   - Se usa INPUT_PULLUP.
//   - Sin pulsar -> HIGH.
//   - Pulsado    -> LOW.
//   - Liberado   -> HIGH.
//
// Modos previstos:
//   - Modo 0: RGB parpadea en azul.
//   - Modo 1: RGB fijo en verde.
//   - Modo 2: RGB apagado.
//
// Reglas del ejercicio:
//   - No usar delay().
//   - No bloquear loop().
//   - Cada tarea debe ejecutarse solo cuando le corresponda.
//   - Cada temporización debe gestionarse con millis().
//   - Las funciones deben tener responsabilidades claras.
//   - El código debe preparar el paso hacia una máquina de estados formal.
//
// Conceptos trabajados:
//   - millis().
//   - Temporización no bloqueante.
//   - Ejecución cooperativa.
//   - Antirrebote por software.
//   - Detección de flanco.
//   - Variables de estado.
//   - Separación de responsabilidades.
//   - Base conceptual para TON, TOF, TP, R_TRIG y F_TRIG.
// ============================================================================

// ===== LIBRERIAS =============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ===== DEFINICIONES ==========================================================
#define RGB_PIN 48
#define NUM_LEDS 1
#define BUTTON_PIN 3

// === CONSTANTES ==============================================================
const unsigned long BLINK_INTERVAL_MS = 500;
const unsigned long DEBOUNCE_TIME_MS = 50;
const unsigned long SERIAL_INTERVAL_MS = 1000;

const int MAX_MODE = 2;

// === VARIABLES GLOBALES ======================================================
// Objeto Led RGB y Control Parpadeo
Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
bool blink_state = false;
unsigned long previous_blink_millis = 0;

// Control modo
int current_mode = 0;
// Control pulsador
int button_state = HIGH;
int previous_button_state = HIGH;
int last_reading = HIGH;
unsigned long last_debounce_time = 0;
unsigned long valid_press_counter = 0;
// Control impresión Puerto Serie
unsigned long previous_serial_millis = 0;

// === PROTOTIPOS ==============================================================
void aplicar_color(Adafruit_NeoPixel &rgb_led, uint32_t color);
void actualizar_rgb_modo(Adafruit_NeoPixel &rgb_led);
bool detectar_pulsacion_valida();
void cambiar_modo();
void aplicar_modo_actual(Adafruit_NeoPixel &rgb_led);
void imprimir_estado_periodico();


// === INICIALIZACION ==========================================================
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.setBrightness(20);
  pixel.show();

  aplicar_modo_actual(pixel);
  
  Serial.println("Ejercicio 05c - Tareas cooperativas");
  Serial.print("BUTTON_PIN = GPIO");
  Serial.println(BUTTON_PIN);
  Serial.print("RGB_PIN = GPIO");
  Serial.println(RGB_PIN);
  Serial.println("Modo inicial: 0 -> parpadeo azul");
}

// === BUCLE PRINCIPAL =========================================================
void loop() {
  // TAREA 1: LEER BOTON Y DETECTAR LA PULSACION VÁLIDA
  if (detectar_pulsacion_valida())
  {
    valid_press_counter++;
    cambiar_modo();
    aplicar_modo_actual(pixel);
  }
  // TAREA 2: ACTUALIZAR PARPADEO SOLO SI ESTAMOS EN MODE = 0
  if (current_mode == 0)
  {
    actualizar_rgb_modo(pixel);
  }

  // TAREA 3: IMPRIMIR ESTADO
  imprimir_estado_periodico();
}


// === FUNCIONES Y RUTINAS =====================================================
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
 * Rutina      : actualizar_rgb_modo
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
void actualizar_rgb_modo(Adafruit_NeoPixel &rgb_led)
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

  if (current_reading != last_reading)
  {
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) >= DEBOUNCE_TIME_MS)
  {
    if (current_reading != button_state)
    {
      previous_button_state = button_state;
      button_state = current_reading;

      if (button_state == HIGH && previous_button_state == LOW)
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
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
      break;
  }
}

/********************************************************************
 * Rutina      : imprimir_estado_periodico
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - Nada
 * Retorna     :
 *   - Nada
 * Descripción :
 *  Gestiona la impresión del Puerto Serie no boqueante.
 *  Usa millis() para la temporización de la impresión sin detener
 *    la ejecución del programa.
 * ******************************************************************/
void imprimir_estado_periodico()
{
  unsigned long current_millis = millis();

  if (current_millis - previous_serial_millis >= SERIAL_INTERVAL_MS)
  {
    previous_serial_millis = current_millis;

    Serial.print("· Modo Actual: ");
    Serial.print(current_mode);
    if (current_mode == 0) 
    {
      Serial.println(" - RGB Azul parpadeo");
    }
    else if (current_mode == 1)
    {
      Serial.println(" - RGB Verde fijo");
    }
    else if (current_mode == 2)
    {
      Serial.println(" - RGB Apagado");
    }
  }
}