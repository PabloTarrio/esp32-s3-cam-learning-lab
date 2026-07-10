# Ejercicio 07 - Temporizador software simple

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio introduce una primera abstracción reutilizable para temporización no bloqueante: la función `temporizador_cumplido()`. El objetivo es encapsular el patrón repetido basado en `millis()` antes de avanzar hacia temporizadores tipo PLC como `TON`, `TOF` y `TP`.

---

## Objetivo

Crear una función reutilizable de temporización no bloqueante.

Este ejercicio permite practicar:

- uso de `millis()`;
- encapsulación del patrón de temporización no bloqueante;
- uso de paso por referencia;
- reutilización de una misma función en varias tareas;
- reducción de código repetido;
- parpadeo RGB sin `delay()`;
- impresión periódica por monitor serie sin bloqueo;
- mantenimiento de una máquina de estados básica como contexto de prueba.

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
arduino/07_temporizador_software_simple/
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

## Idea principal

Hasta ahora se había usado directamente el patrón:

```cpp
if (millis() - previous_millis >= interval)
{
    previous_millis = millis();
    // acción temporizada
}
```

En este ejercicio ese patrón se encapsula en una función reutilizable:

```cpp
bool temporizador_cumplido(unsigned long &previous_time, unsigned long interval)
```

La función devuelve:

```text
true  -> si el intervalo se ha cumplido
false -> si el intervalo todavía no se ha cumplido
```

Cuando devuelve `true`, también actualiza automáticamente la marca temporal recibida por referencia.

---

## Función principal del ejercicio

```cpp
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
```

### Parámetros

- `previous_time`: referencia al instante anterior registrado.
- `interval`: tiempo que debe transcurrir, en milisegundos.

### Retorno

- `true`: el intervalo se ha cumplido.
- `false`: el intervalo todavía no se ha cumplido.

---

## Uso de paso por referencia

La variable `previous_time` se pasa por referencia:

```cpp
unsigned long &previous_time
```

Esto permite que la función modifique la variable original usada por cada tarea.

Ejemplo:

```cpp
temporizador_cumplido(previous_blink_millis, BLINK_INTERVAL_MS)
```

Si se cumple el intervalo, la función actualiza `previous_blink_millis`.

---

## Tareas que usan el temporizador simple

### Parpadeo RGB

El parpadeo azul usa:

```cpp
temporizador_cumplido(previous_blink_millis, BLINK_INTERVAL_MS)
```

Cuando el temporizador se cumple, se alterna el estado del RGB:

```cpp
blink_state = !blink_state;
```

---

### Impresión periódica

La salida periódica por monitor serie usa:

```cpp
temporizador_cumplido(previous_serial_millis, SERIAL_INTERVAL_MS)
```

Cuando el temporizador se cumple, se imprime el estado actual del sistema.

---

## Antirrebote del pulsador

Aunque este ejercicio introduce `temporizador_cumplido()`, el antirrebote del pulsador se mantiene con la comparación explícita:

```cpp
if ((millis() - last_debounce_time) >= DEBOUNCE_TIME_MS)
```

Motivo:

- `last_debounce_time` representa el momento del último cambio detectado en la lectura del botón.
- No debe comportarse como un temporizador periódico.
- Su significado es diferente al de `previous_blink_millis` o `previous_serial_millis`.

Esto ayuda a distinguir entre:

```text
temporización periódica -> temporizador_cumplido()
antirrebote             -> comprobación explícita de estabilidad
```

---

## Funcionamiento general

El ejercicio parte de la máquina de estados del ejercicio anterior y sustituye las temporizaciones manuales por la función `temporizador_cumplido()`.

El sistema mantiene tres estados:

| Estado | Comportamiento |
|---|---|
| `ESTADO_PARPADEO_AZUL` | RGB parpadea en azul |
| `ESTADO_VERDE_FIJO` | RGB permanece fijo en verde |
| `ESTADO_APAGADO` | RGB permanece apagado |

Cada pulsación válida avanza al siguiente estado.

---

## Resultado esperado

Al arrancar, el monitor serie muestra información similar a:

```text
Ejercicio 07 - Temporizador software simple
BUTTON_PIN = GPIO3
RGB_PIN = GPIO48
Modo inicial: 0 -> parpadeo azul
```

Cada segundo se imprime el estado actual:

```text
· Modo Actual: ESTADO_PARPADEO_AZUL
```

Al pulsar y soltar el botón, cambia el estado:

```text
· Modo Actual: ESTADO_VERDE_FIJO
```

Después:

```text
· Modo Actual: ESTADO_APAGADO
```

Y posteriormente vuelve a:

```text
· Modo Actual: ESTADO_PARPADEO_AZUL
```

---

## Conceptos trabajados

### Abstracción progresiva

El ejercicio transforma un patrón repetido en una función reutilizable.

Antes:

```cpp
if (millis() - previous_time >= interval)
{
    previous_time = millis();
}
```

Después:

```cpp
if (temporizador_cumplido(previous_time, interval))
{
    // acción temporizada
}
```

---

### Reutilización

La misma función se puede usar para varias tareas independientes, siempre que cada una tenga su propia variable de tiempo:

```cpp
previous_blink_millis
previous_serial_millis
```

---

### Preparación para temporizadores PLC

`temporizador_cumplido()` no es todavía un `TON`, `TOF` o `TP`, pero es un paso intermedio importante.

Permite pasar de:

```text
millis() manual
```

a:

```text
función reutilizable de temporización
```

y prepara el camino hacia:

```text
clases con estado interno tipo TON, TOF y TP
```

---

## Problemas encontrados

### Uso del temporizador simple en antirrebote

Se planteó usar `temporizador_cumplido()` también para el antirrebote.

Sin embargo, se decidió mantener el antirrebote con comparación explícita porque `last_debounce_time` representa el último cambio de lectura, no una tarea periódica.

Conclusión:

> No todas las temporizaciones con `millis()` tienen el mismo significado. Conviene elegir la abstracción adecuada según el contexto.

---

### Herencia del ejercicio anterior

El ejercicio conserva la máquina de estados básica del ejercicio 06, pero el foco principal ya no es la máquina de estados.

El foco de este ejercicio es la función:

```cpp
temporizador_cumplido()
```

---

## Relación con ejercicios anteriores

Este ejercicio evoluciona desde:

```text
arduino/06_maquina_estado_basica/
```

La diferencia principal es que ahora se encapsula la lógica de temporización no bloqueante en una función reutilizable.

---

## Relación con PLC

Este ejercicio se aproxima a la idea de reutilizar bloques temporizados.

Todavía no introduce bloques funcionales tipo PLC, pero prepara el terreno para construir:

- `TON`;
- `TOF`;
- `TP`.

La función `temporizador_cumplido()` puede verse como un primer bloque auxiliar para tareas periódicas.

---

## Documentación relacionada

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
docs/buenas_practicas.md
docs/lecciones_aprendidas.md
arduino/05_millis_no_bloqueante/
arduino/05c_tareas_cooperativas/
arduino/06_maquina_estado_basica/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Función `temporizador_cumplido()` | Validada |
| Parpadeo RGB con temporizador simple | Validado |
| Impresión periódica con temporizador simple | Validada |
| Antirrebote explícito del pulsador | Validado |
| Máquina de estados reutilizada como contexto | Validada |
| Preparación para `TON`, `TOF` y `TP` | Validada |
