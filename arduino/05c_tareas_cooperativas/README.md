# Ejercicio 05c - Tareas cooperativas en `loop()`

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio consolida el uso de `millis()` para ejecutar varias tareas de forma cooperativa dentro de `loop()`, sin usar `delay()` y sin bloquear el programa.

---

## Objetivo

Ejecutar varias tareas independientes dentro de `loop()` sin que una tarea bloquee a las demás.

El ejercicio combina:

- lectura de pulsador externo en `GPIO3`;
- antirrebote por software con `millis()`;
- detección de pulsación válida;
- control del LED RGB integrado en `GPIO48`;
- parpadeo no bloqueante;
- impresión periódica por puerto serie;
- cambio de modo de funcionamiento;
- separación de responsabilidades mediante funciones.

Este ejercicio sirve como paso intermedio antes de formalizar una **máquina de estados básica**.

---

## Hardware utilizado

- ESP32-S3-CAM.
- Pulsador externo.
- Protoboard.
- Cables Dupont.
- LED RGB integrado NeoPixel / WS2812 en `GPIO48`.

---

## Software utilizado

- Visual Studio Code.
- PlatformIO.
- Arduino Framework.
- Librería `Adafruit_NeoPixel`.

---

## Estructura del ejercicio

```text
arduino/05c_tareas_cooperativas/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

---

## Configuración PlatformIO

Archivo:

```text
platformio.ini
```

Configuración usada:

```ini
[env:esp32-s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200

build_flags =
    -DBOARD_HAS_PSRAM

lib_deps =
    adafruit/Adafruit NeoPixel
```

---

## Cableado

El pulsador externo se conecta entre `GPIO3` y `GND`.

```text
ESP32-S3-CAM                         PROTOBOARD

GPIO3  ------------------------------┐
                                     │
                                  [ PULSADOR ]
                                     │
GND    ------------------------------┘
```

Se usa la resistencia `pull-up` interna del ESP32-S3:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

La lógica esperada es:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
Liberado   -> HIGH
```

En este ejercicio se considera pulsación válida la liberación del pulsador:

```text
LOW -> HIGH = flanco ascendente = liberación
```

---

## Pines utilizados

| Función | GPIO | Estado |
|---|---:|---|
| Pulsador externo | GPIO3 | Validado |
| RGB integrado | GPIO48 | Validado |

---

## Tareas cooperativas

El programa ejecuta tres tareas principales dentro de `loop()`:

| Tarea | Función | Descripción |
|---|---|---|
| Tarea 1 | Botón | Detecta pulsaciones válidas con antirrebote. |
| Tarea 2 | RGB | Actualiza el comportamiento del RGB según el modo actual. |
| Tarea 3 | Serial | Imprime periódicamente el estado del sistema. |

La idea principal es que ninguna tarea use `delay()` ni bloquee el ciclo principal.

---

## Modos de funcionamiento

El ejercicio usa tres modos:

| Modo | Comportamiento |
|---:|---|
| 0 | RGB parpadea en azul sin usar `delay()` |
| 1 | RGB permanece fijo en verde |
| 2 | RGB permanece apagado |

Cada pulsación válida cambia al siguiente modo:

```text
Modo 0 -> Modo 1 -> Modo 2 -> Modo 0 -> ...
```

---

## Funcionamiento general

El flujo principal del programa es:

```cpp
void loop()
{
    if (detectar_pulsacion_valida())
    {
        cambiar_modo();
        aplicar_modo_actual(pixel);
    }

    if (current_mode == 0)
    {
        actualizar_rgb_modo(pixel);
    }

    imprimir_estado_periodico();
}
```

Este patrón permite que el programa:

- atienda el pulsador;
- actualice el RGB;
- imprima información periódica;
- siga ejecutándose sin bloqueos.

---

## Resultado esperado

Al arrancar, el monitor serie muestra:

```text
Ejercicio 05c - Tareas cooperativas
BUTTON_PIN = GPIO3
RGB_PIN = GPIO48
Modo inicial: 0 -> parpadeo azul
```

En modo `0`, el RGB integrado parpadea en azul.

Cada cierto tiempo se imprime el estado actual:

```text
· Modo Actual: 0 - RGB Azul parpadeo
```

Al pulsar y soltar el botón, el modo cambia.

Ejemplo:

```text
· Modo Actual: 1 - RGB Verde fijo
```

Después:

```text
· Modo Actual: 2 - RGB Apagado
```

Y posteriormente vuelve a:

```text
· Modo Actual: 0 - RGB Azul parpadeo
```

---

## Conceptos trabajados

### Ejecución cooperativa

El programa divide el trabajo en varias tareas pequeñas que se ejecutan repetidamente dentro de `loop()`.

Cada tarea comprueba si debe actuar y, si no debe actuar, devuelve el control rápidamente.

---

### Temporización no bloqueante

El ejercicio usa varios temporizadores basados en `millis()`:

```cpp
previous_blink_millis
previous_serial_millis
last_debounce_time
```

Cada temporizador controla una tarea diferente:

- parpadeo RGB;
- impresión periódica;
- antirrebote del pulsador.

---

### Antirrebote por software

El pulsador se filtra con:

```cpp
const unsigned long DEBOUNCE_TIME_MS = 50;
```

Solo se acepta un cambio de estado cuando la lectura permanece estable durante el tiempo definido.

---

### Variables de estado

El programa mantiene estado entre ciclos de `loop()` mediante variables globales:

```cpp
bool blink_state = false;
int current_mode = 0;
int button_state = HIGH;
unsigned long previous_blink_millis = 0;
unsigned long previous_serial_millis = 0;
```

Estas variables permiten que el programa recuerde qué estaba haciendo sin bloquearse.

---

### Separación de responsabilidades

El código se organiza en rutinas específicas:

- `aplicar_color()` aplica un color al RGB.
- `actualizar_rgb_modo()` gestiona el parpadeo no bloqueante.
- `detectar_pulsacion_valida()` lee el botón y aplica antirrebote.
- `cambiar_modo()` cambia el modo actual.
- `aplicar_modo_actual()` aplica la salida inicial de cada modo.
- `imprimir_estado_periodico()` imprime el estado sin bloquear el programa.

---

## Relación con máquinas de estado

Este ejercicio todavía no implementa una máquina de estados formal, pero ya trabaja con modos de funcionamiento:

```text
Modo 0 -> RGB parpadeo azul
Modo 1 -> RGB verde fijo
Modo 2 -> RGB apagado
```

El siguiente paso natural será sustituir los modos numéricos por estados con nombre, por ejemplo mediante un `enum`.

---

## Relación con PLC

Este ejercicio se parece a un ciclo principal de PLC en el que se ejecutan varias tareas por ciclo:

```text
ciclo principal:
    leer entradas
    actualizar temporizadores
    ejecutar lógica de modo
    actualizar salidas
    enviar diagnóstico
```

La diferencia es que aquí se implementa manualmente con `loop()` y `millis()`.

Este ejercicio prepara el camino para:

- `TON`;
- `TOF`;
- `TP`;
- `R_TRIG`;
- `F_TRIG`;
- máquinas de estado.

---

## Documentación relacionada

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
docs/buenas_practicas.md
docs/lecciones_aprendidas.md
arduino/05_millis_no_bloqueante/
arduino/05b_millis_rgb_boton/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Pulsador externo en GPIO3 | Validado |
| RGB integrado en GPIO48 | Validado |
| Parpadeo sin `delay()` | Validado |
| Antirrebote con `millis()` | Validado |
| Impresión periódica sin bloqueo | Validado |
| Varias tareas cooperativas en `loop()` | Validado |
