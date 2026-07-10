// ============================================================================
// Ejercicio 07 - Temporizador software simple
// Fase 4 - Temporización no bloqueante
// Placa: ESP32-S3-CAM
//
// Objetivo:
//   - Crear una función reutilizable de temporización no bloqueante.
//   - Encapsular el patrón basado en millis():
//
//       current_time - previous_time >= interval
//
//   - Evitar repetir la misma lógica de temporización en varias rutinas.
//   - Usar un temporizador software simple para gestionar varias tareas.
//   - Preparar el camino hacia temporizadores tipo PLC: TON, TOF y TP.
//
// Hardware:
//   - LED RGB integrado NeoPixel / WS2812 conectado a GPIO48.
//   - Opcionalmente, pulsador externo conectado entre GPIO3 y GND.
//
// Idea principal:
//   Hasta ahora se ha usado manualmente el patrón:
//
//       if (millis() - previous_millis >= interval)
//       {
//           previous_millis = millis();
//           // acción temporizada
//       }
//
//   En este ejercicio se encapsula ese patrón en una función reutilizable:
//
//       bool temporizador_cumplido(unsigned long &previous_time,
//                                  unsigned long interval)
//
//   La función devuelve true cuando se ha cumplido el intervalo y actualiza
//   automáticamente la referencia temporal.
//
// Tareas previstas:
//   - Tarea 1: parpadeo del RGB integrado.
//   - Tarea 2: impresión periódica por monitor serie.
//   - Tarea 3: posible lectura de pulsador o cambio de modo.
//
// Reglas del ejercicio:
//   - No usar delay().
//   - No bloquear loop().
//   - Usar millis() para todas las temporizaciones.
//   - Evitar duplicar lógica de temporización.
//   - Usar paso por referencia para actualizar la marca temporal.
//   - Mantener funciones con responsabilidades claras.
//
// Conceptos trabajados:
//   - millis().
//   - Temporización no bloqueante.
//   - Función reutilizable.
//   - Paso por referencia.
//   - Variables de tiempo.
//   - Tareas cooperativas.
//   - Abstracción progresiva.
//   - Base conceptual para TON, TOF y TP.
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

// === VARIABLES GLOBALES ======================================================
// Objeto Led RGB y Control Parpadeo
Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);
bool blink_state = false;
unsigned long previous_blink_millis = 0;

// Control Estado
enum EstadoSistema 
{
  ESTADO_PARPADEO_AZUL,
  ESTADO_VERDE_FIJO,
  ESTADO_APAGADO
};
EstadoSistema estado_actual = ESTADO_PARPADEO_AZUL;

// Control pulsador
int button_state = HIGH;
int previous_button_state = HIGH;
int last_reading = HIGH;
unsigned long last_debounce_time = 0;
unsigned long valid_press_counter = 0;
// Control impresión Puerto Serie
unsigned long previous_serial_millis = 0;

// === PROTOTIPOS ==============================================================
bool temporizador_cumplido(unsigned long &previous_time, unsigned long interval);
void aplicar_color(Adafruit_NeoPixel &rgb_led, uint32_t color);
bool detectar_pulsacion_valida();
void actualizar_estado(EstadoSistema &estado);
void ejecutar_estado_actual(Adafruit_NeoPixel &rgb_led, EstadoSistema estado);
void actualizar_parpadeo_azul(Adafruit_NeoPixel &rgb_led);
void imprimir_estado_periodico(EstadoSistema estado);

// === INICIALIZACION ==========================================================
// =============================================================================
void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pixel.begin();
  pixel.clear();
  pixel.setBrightness(20);
  pixel.show();

  ejecutar_estado_actual(pixel, estado_actual);
  
  Serial.println("Ejercicio 07 - Temporizador software simple");
  Serial.print("BUTTON_PIN = GPIO");
  Serial.println(BUTTON_PIN);
  Serial.print("RGB_PIN = GPIO");
  Serial.println(RGB_PIN);
  Serial.println("Modo inicial: 0 -> parpadeo azul");
}

// === BUCLE PRINCIPAL =========================================================
// =============================================================================
void loop() {
  // TAREA 1: LEER EL BOTON y DETECTAR LA PULSACIÓN VÁLIDA
  if (detectar_pulsacion_valida())
  {
    valid_press_counter++;
    actualizar_estado(estado_actual);
    ejecutar_estado_actual(pixel, estado_actual);
  }

  // TAREA 2: CONTROL DEL PARPADEO SOLO SI ESTADO_PARPADEO_AZUL
  if (estado_actual == ESTADO_PARPADEO_AZUL)
  {
    actualizar_parpadeo_azul(pixel);
  }

  // TAREA3: IMPRIMIR INFORMACIÓN
  imprimir_estado_periodico(estado_actual);
}

// === FUNCIONES Y RUTINAS =====================================================
/*******************************************************************
 * Rutina      : temporizador_cumplido
 * Tipo        : Función
 * Parámetros  :
 *   - previous_time : referencia al instante anterior registrado
 *   - interval      : tiempo que debe transcurrir, en milisegundos
 * Retorna     :
 *   - true  : el intervalo se ha cumplido
 *   - false : el intervalo todavía no se ha cumplido
 * Descripción :
 *   Comprueba si ha transcurrido el intervalo indicado desde el
 *   último instante registrado.
 *
 *   Si el intervalo se ha cumplido, actualiza previous_time con
 *   el tiempo actual y devuelve true.
 *
 *   Si el intervalo no se ha cumplido, no modifica previous_time
 *   y devuelve false.
 *
 *   Esta función encapsula el patrón básico de temporización no
 *   bloqueante con millis() y permite reutilizarlo en distintas
 *   tareas cooperativas.
 *******************************************************************/
bool temporizador_cumplido(unsigned long &previous_time, unsigned long interval)
{
  unsigned long current_time = millis();

  if (current_time - previous_time >= interval)
  {
    previous_time = current_time;
    return true;
  }
  return false;
}

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
void imprimir_estado_periodico(EstadoSistema estado)
{
  if (temporizador_cumplido(previous_serial_millis, SERIAL_INTERVAL_MS))
  {
    Serial.print("· Modo Actual: ");
    
    switch (estado)
    {
      case ESTADO_PARPADEO_AZUL:
        Serial.println("ESTADO_PARPADEO_AZUL");
        break;
      case ESTADO_VERDE_FIJO:
        Serial.println("ESTADO_VERDE_FIJO");
        break;
      case ESTADO_APAGADO:
        Serial.println("ESTADO_APAGADO");
        break;
      default:
        Serial.println("ESTADO_DESCONOCIDO");
        break;
    }
  }
}

/*******************************************************************
 * Rutina      : actualizar_estado
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - estado  : referencia al estado actual de la máquina de estados
 * Retorna     :
 *   - Nada
 * Descripción :
 *   Actualiza el estado actual de la máquina de estados siguiendo la
 * secuencia definida.
 * 
 *  Esta rutina no detecta directamente eventos de entrada. La
 * detección de pusación válida se realiza fuera de esta rutina.
 *******************************************************************/
void actualizar_estado(EstadoSistema &estado)
{
  switch (estado)
  {
    case ESTADO_PARPADEO_AZUL:
      estado = ESTADO_VERDE_FIJO;
      break;
    case ESTADO_VERDE_FIJO:
      estado = ESTADO_APAGADO;
      break;
    case ESTADO_APAGADO:
      estado = ESTADO_PARPADEO_AZUL;
      break;
    default:
      estado = ESTADO_PARPADEO_AZUL;
      break;
  }
}

/*******************************************************************
 * Rutina      : ejecutar_estado_actual
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - rgb_led : referencia al objeto NeoPixel que controla el RGB
 *   - estado  : estado actual de la máquina de estados
 * Retorna     :
 *   - Nada
 * Descripción :
 *   Ejecuta el comportamiento asociado al estado actual del sistema.
 *
 *   Esta rutina no decide transiciones de estado. Su responsabilidad
 *   es aplicar la salida correspondiente al estado recibido.
 *
 *   Estados gestionados:
 *
 *     ESTADO_PARPADEO_AZUL:
 *       Actualiza el parpadeo azul no bloqueante del RGB integrado.
 *
 *     ESTADO_VERDE_FIJO:
 *       Mantiene el RGB integrado en verde fijo.
 *
 *     ESTADO_APAGADO:
 *       Mantiene el RGB integrado apagado.
 *
 *   En caso de recibir un estado no previsto, se apaga el RGB como
 *   comportamiento seguro por defecto.
 *******************************************************************/
void ejecutar_estado_actual(Adafruit_NeoPixel &rgb_led, EstadoSistema estado)
{
  switch (estado)
  {
    case ESTADO_PARPADEO_AZUL:
      actualizar_parpadeo_azul(rgb_led);
      break;
    case ESTADO_VERDE_FIJO:
      aplicar_color(rgb_led, rgb_led.Color(0, 255, 0));
      break;
    case ESTADO_APAGADO:
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
      break;
    default:
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
      break;
  }
}

/*******************************************************************
 * Rutina      : actualizar_parpadeo_azul
 * Tipo        : Procedimiento
 * Parámetros  :
 *   - rgb_led : referencia al objeto NeoPixel que controla el RGB
 * Retorna     :
 *   - Nada
 * Descripción :
 *   Gestiona el parpadeo azul no bloqueante del LED RGB integrado.
 *
 *   Esta rutina debe ejecutarse de forma cíclica desde loop() cuando
 *   la máquina de estados se encuentra en ESTADO_PARPADEO_AZUL.
 *
 *   Usa millis() para comprobar si ha transcurrido el intervalo de
 *   parpadeo configurado. Cuando se cumple el intervalo, alterna el
 *   estado interno del parpadeo entre encendido y apagado.
 *
 *   La rutina no usa delay() y devuelve el control rápidamente si
 *   todavía no ha transcurrido el tiempo necesario.
 *******************************************************************/
void actualizar_parpadeo_azul(Adafruit_NeoPixel &rgb_led)
{
  if (temporizador_cumplido(previous_blink_millis, BLINK_INTERVAL_MS))
  {
    blink_state = !blink_state;

    if (blink_state){
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 255));
    }
    else
    {
      aplicar_color(rgb_led, rgb_led.Color(0, 0, 0));
    }
  }
}