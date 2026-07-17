# plc_blocks

Librería local para proyectos Arduino/ESP32 con bloques reutilizables inspirados en programación PLC.

Esta librería forma parte del repositorio **ESP32-S3-CAM Learning Lab** y recoge los bloques desarrollados durante la Fase 4 de temporización no bloqueante:

- temporizadores tipo PLC;
- detectores de flanco;
- bloques con estado interno;
- ejecución cíclica desde `loop()`;
- uso de `millis()` sin `delay()`.

---

## Objetivo

Centralizar en una librería local los bloques reutilizables que se han ido desarrollando en ejercicios independientes.

La idea es pasar de este enfoque:

```text
Cada ejercicio contiene su propia copia de TON, TOF, TP, R_TRIG o F_TRIG.
```

a este enfoque:

```text
Los ejercicios importan bloques reutilizables desde lib/plc_blocks.
```

Esto permite:

- evitar duplicar código;
- reutilizar bloques validados;
- mantener una única implementación de cada bloque;
- acercar el proyecto a una estructura más profesional;
- preparar una posible futura librería independiente.

---

## Ubicación en el repositorio

La librería se encuentra en:

```text
lib/plc_blocks/
```

Estructura recomendada:

```text
lib/
└── plc_blocks/
    ├── README.md
    ├── library.json
    └── src/
        ├── PlcTimers.h
        ├── PlcTimers.cpp
        ├── PlcEdges.h
        ├── PlcEdges.cpp
        ├── PlcDebounce.h
        └── PlcDebounce.cpp
```

---

## Archivos principales

### `PlcTimers.h`

Declara las clases de temporizadores:

```cpp
class TON;
class TOF;
class TP;
```

---

### `PlcTimers.cpp`

Implementa los métodos de los temporizadores:

```cpp
TON::TON()
TON::update()
TON::Q()
TON::ET()

TOF::TOF()
TOF::update()
TOF::Q()
TOF::ET()

TP::TP()
TP::update()
TP::Q()
TP::ET()
```

---

### `PlcEdges.h`

Declara las clases de detección de flanco:

```cpp
class R_TRIG;
class F_TRIG;
```

---

### `PlcEdges.cpp`

Implementa los métodos de los detectores de flanco:

```cpp
R_TRIG::R_TRIG()
R_TRIG::update()
R_TRIG::Q()

F_TRIG::F_TRIG()
F_TRIG::update()
F_TRIG::Q()
```

### `PlcDebounce.h`

Declara la clase para el antirrebote_
```cpp
class DEBOUNCE;
```

### `PlcDebounce.cpp`

Implementa los métodos de antirrebote:

```cpp
DEBOUNCE::DEBOUNCE();
DEBOUNCE::update();
DEBOUNCE::Q();
```

---

## Bloques incluidos

## Temporizadores

### `TON`

Temporizador a la conexión.

```text
IN pasa a true
↓
empieza a contar
↓
si IN permanece true durante PT
↓
Q pasa a true
```

Uso conceptual:

```cpp
TON ton_boton;

ton_boton.update(in, 2000);

if (ton_boton.Q())
{
    // temporizador cumplido
}
```

---

### `TOF`

Temporizador a la desconexión.

```text
IN es true
↓
Q es true inmediatamente
↓
IN pasa a false
↓
empieza a contar
↓
Q permanece true mientras ET < PT
↓
cuando ET >= PT, Q pasa a false
```

Uso conceptual:

```cpp
TOF tof_boton;

tof_boton.update(in, 2000);

if (tof_boton.Q())
{
    // salida mantenida activa
}
```

---

### `TP`

Temporizador de pulso.

```text
IN pasa de false a true
↓
Q pasa a true inmediatamente
↓
Q permanece true durante PT
↓
Q pasa a false
```

Mantener `IN` en `true` no reinicia el pulso. Para generar un nuevo pulso, `IN` debe volver primero a `false` y después pasar otra vez a `true`.

Uso conceptual:

```cpp
TP tp_boton;

tp_boton.update(in, 2000);

if (tp_boton.Q())
{
    // pulso activo
}
```

---

## Detectores de flanco

### `R_TRIG`

Detector de flanco ascendente.

Detecta:

```text
false -> true
```

La salida `Q` se activa solo durante un ciclo de `update()`.

Uso conceptual:

```cpp
R_TRIG r_trig_boton;

r_trig_boton.update(signal);

if (r_trig_boton.Q())
{
    // flanco ascendente detectado
}
```

---

### `F_TRIG`

Detector de flanco descendente.

Detecta:

```text
true -> false
```

La salida `Q` se activa solo durante un ciclo de `update()`.

Uso conceptual:

```cpp
F_TRIG f_trig_boton;

f_trig_boton.update(signal);

if (f_trig_boton.Q())
{
    // flanco descendente detectado
}
```

---

## Uso desde un proyecto PlatformIO

Desde un ejercicio dentro de `arduino/`, por ejemplo:

```text
arduino/12_plc_blocks_library/
```

el archivo `platformio.ini` debe incluir la ruta a la carpeta `lib` del repositorio:

```ini
lib_extra_dirs =
    ../../lib
```

Ejemplo completo:

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

lib_extra_dirs =
    ../../lib
```

---

## Inclusión en `main.cpp`

Para usar temporizadores:

```cpp
#include <PlcTimers.h>
```

Para usar detectores de flanco:

```cpp
#include <PlcEdges.h>
```

Para usar ambos:

```cpp
#include <PlcTimers.h>
#include <PlcEdges.h>
```

---

## Ejemplo básico de uso

```cpp
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include <PlcTimers.h>
#include <PlcEdges.h>

#define BUTTON_PIN 3
#define RGB_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel pixel(NUM_LEDS, RGB_PIN, NEO_GRB + NEO_KHZ800);

R_TRIG r_trig_boton;
TP tp_boton;

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pixel.begin();
    pixel.clear();
    pixel.setBrightness(20);
    pixel.show();
}

void loop()
{
    bool boton = (digitalRead(BUTTON_PIN) == LOW);

    r_trig_boton.update(boton);

    tp_boton.update(r_trig_boton.Q(), 2000);

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
```

Este ejemplo genera un pulso de 2 segundos cuando se detecta una pulsación del botón.

---

## Relación con PLC

La librería intenta mantener una nomenclatura cercana a PLC:

| PLC | Librería `plc_blocks` |
|---|---|
| `TON` | `TON` |
| `TOF` | `TOF` |
| `TP` | `TP` |
| `R_TRIG` | `R_TRIG` |
| `F_TRIG` | `F_TRIG` |
| `IN` / `CLK` | parámetro de `update()` |
| `Q` | método `Q()` |
| `ET` | método `ET()` |
| ciclo PLC | llamadas repetidas desde `loop()` |

---

## Reglas de uso

### Llamada cíclica

Todos los bloques deben actualizarse de forma cíclica desde `loop()`:

```cpp
bloque.update(...);
```

Si un bloque no se actualiza, su estado interno no evoluciona.

---

### Sin `delay()`

Los bloques no usan `delay()`.

La temporización se realiza mediante `millis()`.

---

### Entrada booleana ya procesada

Los bloques no leen directamente GPIO.

Antes de llamar a `update()`, el programa debe convertir la entrada física en una señal lógica:

```cpp
bool boton = (digitalRead(BUTTON_PIN) == LOW);
```

---

### Rebote de pulsadores

Los detectores de flanco no implementan antirrebote.

En aplicaciones con pulsadores físicos se recomienda esta secuencia:

```text
lectura GPIO -> antirrebote -> señal estable -> R_TRIG / F_TRIG / temporizadores
```

---

## Bloques validados previamente

Esta librería se basa en ejercicios previos del repositorio:

```text
arduino/08_ton_basico/
arduino/09_tof_basico/
arduino/10_tp_basico/
arduino/11_rtrig_ftrig_basico/
```

Cada bloque fue implementado primero de forma local en un ejercicio independiente antes de ser extraído a esta librería.

---

## Estado actual

| Bloque | Estado |
|---|---|
| `TON` | Implementado |
| `TOF` | Implementado |
| `TP` | Implementado |
| `R_TRIG` | Implementado |
| `F_TRIG` | Implementado |
| Uso desde proyecto PlatformIO | Pendiente de validar en ejercicio 12 |

---

## Próximos pasos

- Validar la librería desde `arduino/12_plc_blocks_library/`.
- Crear ejemplos de uso combinando temporizadores y flancos.
- Revisar si conviene añadir antirrebote como bloque independiente.
- Evaluar una futura separación en módulos:
  - `PlcTimers`
  - `PlcEdges`
  - `PlcDebounce`
- Considerar publicación futura como librería independiente si madura lo suficiente.
