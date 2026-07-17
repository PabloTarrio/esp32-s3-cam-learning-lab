# Ejercicio 13 - `DEBOUNCE` con `plc_blocks`

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio valida el nuevo bloque reutilizable `DEBOUNCE` añadido a la librería local `lib/plc_blocks`. El objetivo es filtrar una señal booleana procedente de un pulsador físico y combinarla después con bloques ya existentes de la librería: `R_TRIG` y `TP`.

---

## Objetivo

Validar la cadena completa:

```text
GPIO -> señal lógica raw -> DEBOUNCE -> R_TRIG -> TP -> RGB
```

El ejercicio comprueba:

- que `PlcDebounce.h` se puede incluir desde un proyecto PlatformIO;
- que `DEBOUNCE` filtra una señal booleana procedente de un pulsador físico;
- que `R_TRIG` detecta una pulsación válida sobre la señal estabilizada;
- que `TP` genera un pulso temporizado a partir del flanco válido;
- que el RGB integrado se controla mediante la salida `Q` del `TP`;
- que la librería `plc_blocks` puede crecer con nuevos módulos reutilizables.

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
- Librería local `plc_blocks`.

---

## Estructura del ejercicio

```text
arduino/13_plc_debounce/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

La librería utilizada está en:

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

## Configuración PlatformIO

El proyecto debe permitir que PlatformIO encuentre la librería local ubicada en la raíz del repositorio.

En `platformio.ini` debe incluirse:

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

## Includes utilizados

En `main.cpp` se usan tres módulos de la librería:

```cpp
#include <PlcDebounce.h>
#include <PlcEdges.h>
#include <PlcTimers.h>
```

Cada uno tiene una responsabilidad distinta:

```text
PlcDebounce -> filtrado de señal
PlcEdges    -> detección de flanco
PlcTimers   -> temporización
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

La lógica eléctrica esperada es:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
```

Para trabajar de forma más clara, el programa transforma la lectura física en una señal lógica positiva:

```cpp
bool boton_raw = (digitalRead(BUTTON_PIN) == LOW);
```

Por tanto:

```text
Botón sin pulsar -> boton_raw = false
Botón pulsado    -> boton_raw = true
```

---

## Pines utilizados

| Función | GPIO | Estado |
|---|---:|---|
| Pulsador externo | GPIO3 | Validado |
| RGB integrado | GPIO48 | Validado |

---

## Bloques usados

### `DEBOUNCE`

Filtra la señal booleana procedente del pulsador.

```cpp
debounce_boton.update(boton_raw, DEBOUNCE_TIME_MS);
```

La salida estable se obtiene con:

```cpp
bool boton_estable = debounce_boton.Q();
```

---

### `R_TRIG`

Detecta una pulsación válida sobre la señal estabilizada:

```cpp
r_trig_boton.update(boton_estable);
```

Se activa cuando la señal estable pasa de:

```text
false -> true
```

---

### `TP`

Genera un pulso temporizado cuando `R_TRIG` detecta la pulsación válida:

```cpp
tp_boton.update(r_trig_boton.Q(), TP_TIME_MS);
```

Mientras `TP.Q()` es `true`, el RGB integrado permanece encendido en verde.

---

## Constantes principales

```cpp
const unsigned long DEBOUNCE_TIME_MS = 50;
const unsigned long TP_TIME_MS = 2000;
const unsigned long SERIAL_INTERVAL_MS = 500;
```

Significado:

- `DEBOUNCE_TIME_MS`: tiempo mínimo que una señal debe mantenerse estable para ser aceptada.
- `TP_TIME_MS`: duración del pulso generado por `TP`.
- `SERIAL_INTERVAL_MS`: intervalo para imprimir diagnóstico sin saturar el monitor serie.

---

## Funcionamiento general

El flujo lógico del programa es:

```text
GPIO3
↓
boton_raw
↓
DEBOUNCE
↓
boton_estable
↓
R_TRIG
↓
TP de 2000 ms
↓
RGB verde mientras TP.Q() == true
```

En código:

```cpp
bool boton_raw = (digitalRead(BUTTON_PIN) == LOW);

debounce_boton.update(boton_raw, DEBOUNCE_TIME_MS);

bool boton_estable = debounce_boton.Q();

r_trig_boton.update(boton_estable);

tp_boton.update(r_trig_boton.Q(), TP_TIME_MS);

aplicar_salida_rgb();
```

---

## Resultado esperado

Al arrancar, el monitor serie muestra información similar a:

```text
Ejercicio 13 - DEBOUNCE con plc_blocks
BUTTON_PIN = GPIO3
RGB_PIN = GPIO48
```

Al pulsar el botón:

```text
boton_raw puede cambiar rápidamente por rebote
DEBOUNCE espera estabilidad durante 50 ms
boton_estable pasa a true
R_TRIG detecta el flanco ascendente
TP genera un pulso de 2000 ms
RGB verde durante el pulso
```

Al mantener el botón pulsado:

```text
R_TRIG no se repite
TP no se reinicia
```

Al terminar el pulso:

```text
TP.Q() pasa a false
RGB apagado
```

Para generar un nuevo pulso:

```text
Soltar el botón
Esperar estabilización
Pulsar de nuevo
```

---

## Diagnóstico por monitor serie

El ejercicio imprime periódicamente:

```text
Raw: 0/1 | Stable: 0/1 | R_TRIG.Q: 0/1 | TP.Q: 0/1 | TP.ET: N ms
```

Esto permite observar la diferencia entre:

```text
señal física raw
señal estabilizada
flanco detectado
pulso temporizado
```

---

## Conceptos trabajados

### Antirrebote reutilizable

Antes, el antirrebote se implementaba dentro de cada ejercicio o función concreta.

Ahora se encapsula en un bloque reutilizable:

```cpp
DEBOUNCE debounce_boton;
```

---

### Separación de responsabilidades

El ejercicio separa claramente:

```text
lectura GPIO       -> main.cpp
filtrado rebote    -> DEBOUNCE
detección evento   -> R_TRIG
temporización      -> TP
salida física      -> RGB
```

Esto mejora la claridad y facilita la reutilización.

---

### Señal raw frente a señal estable

`boton_raw` representa la lectura lógica directa del GPIO.

`boton_estable` representa el estado aceptado tras pasar por `DEBOUNCE`.

---

### Combinación de bloques

El valor más importante de este ejercicio es combinar bloques simples:

```text
DEBOUNCE + R_TRIG + TP
```

para construir una lógica más robusta y expresiva.

---

## Problemas o decisiones relevantes

### No usar `detectar_pulsacion_valida()`

Se decidió no añadir una función específica de pulsación válida a la librería.

En su lugar, se optó por un bloque más genérico:

```cpp
DEBOUNCE
```

Esto permite reutilizar la señal estable con distintos bloques:

```text
DEBOUNCE -> R_TRIG
DEBOUNCE -> F_TRIG
DEBOUNCE -> TON
DEBOUNCE -> TP
```

---

### `DEBOUNCE` no lee GPIO directamente

El bloque recibe una señal booleana ya procesada.

Esto mantiene la librería desacoplada del hardware concreto.

---

### `R_TRIG` trabaja sobre señal estable

El flanco se detecta después del antirrebote.

Esto evita que un rebote mecánico genere varios eventos falsos.

---

## Relación con ejercicios anteriores

Este ejercicio evoluciona desde:

```text
arduino/12_plc_blocks_library/
```

En el ejercicio 12 se validó la librería con la cadena:

```text
GPIO -> R_TRIG -> TP -> RGB
```

En este ejercicio se añade el bloque de antirrebote:

```text
GPIO -> DEBOUNCE -> R_TRIG -> TP -> RGB
```

---

## Relación con PLC

La cadena del ejercicio se parece a una arquitectura típica de control:

```text
entrada física
↓
filtrado / acondicionamiento
↓
detección de evento
↓
temporización
↓
salida
```

En bloques:

```text
boton_raw -> DEBOUNCE -> R_TRIG -> TP -> RGB
```

---

## Documentación relacionada

```text
lib/plc_blocks/README.md
lib/plc_blocks/library.json
lib/plc_blocks/src/PlcDebounce.h
lib/plc_blocks/src/PlcDebounce.cpp
lib/plc_blocks/src/PlcEdges.h
lib/plc_blocks/src/PlcEdges.cpp
lib/plc_blocks/src/PlcTimers.h
lib/plc_blocks/src/PlcTimers.cpp
arduino/12_plc_blocks_library/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Inclusión de `PlcDebounce.h` | Validada |
| Inclusión de `PlcEdges.h` | Validada |
| Inclusión de `PlcTimers.h` | Validada |
| Uso de `DEBOUNCE` desde librería | Validado |
| Uso de `R_TRIG` desde librería | Validado |
| Uso de `TP` desde librería | Validado |
| Cadena `GPIO -> DEBOUNCE -> R_TRIG -> TP` | Validada |
| RGB controlado desde bloques reutilizables | Validado |

---

## Próximo paso

Actualizar la documentación de `lib/plc_blocks` para incluir `DEBOUNCE` como bloque oficial de la librería.

Después, valorar si la Fase 4 puede darse por cerrada o si conviene añadir algún ejemplo combinado adicional.
