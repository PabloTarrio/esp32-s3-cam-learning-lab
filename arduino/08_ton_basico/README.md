# Ejercicio 08 - TON básico

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio introduce la implementación de un temporizador tipo **TON** inspirado en PLC, usando una clase C++ con estado interno. El objetivo es pasar de funciones auxiliares basadas en `millis()` a un bloque reutilizable que encapsula su propio estado, de forma similar a un bloque funcional PLC.

---

## Objetivo

Implementar y validar un temporizador tipo `TON` básico.

Este ejercicio permite practicar:

- creación de una clase C++ sencilla;
- estado interno dentro de un objeto;
- equivalencia entre bloques PLC y C++;
- conceptos `IN`, `PT`, `Q` y `ET`;
- uso de `millis()` sin bloquear `loop()`;
- activación de una salida cuando una entrada permanece activa durante un tiempo;
- control del RGB integrado usando la salida del temporizador.

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
arduino/08_ton_basico/
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
```

Por tanto, la entrada lógica del temporizador se calcula así:

```cpp
bool boton_pulsado = (digitalRead(BUTTON_PIN) == LOW);
```

---

## Pines utilizados

| Función | GPIO | Estado |
|---|---:|---|
| Pulsador externo | GPIO3 | Validado |
| RGB integrado | GPIO48 | Validado |

---

## Qué es un TON

Un `TON` es un temporizador a la conexión.

Su comportamiento básico es:

```text
IN = false
    Q = false
    ET = 0

IN pasa a true
    empieza a contar tiempo
    Q sigue en false mientras ET < PT

IN permanece true durante PT
    Q pasa a true

IN vuelve a false
    Q = false
    ET = 0
    el temporizador se reinicia
```

---

## Equivalencia PLC ↔ C++

| Concepto PLC | Implementación en este ejercicio |
|---|---|
| `IN` | Parámetro `in` del método `update()` |
| `PT` | Parámetro `pt` del método `update()` |
| `Q` | Variable interna `q` y método `Q()` |
| `ET` | Variable interna `elapsed_time` y método `ET()` |
| Bloque funcional | Clase `TON` |
| Ciclo PLC | Llamadas repetidas a `update()` desde `loop()` |

---

## Clase `TON`

La clase implementada tiene esta estructura conceptual:

```cpp
class TON
{
private:
    bool q;
    unsigned long start_time;
    unsigned long elapsed_time;
    bool timing;

public:
    TON();
    void update(bool in, unsigned long pt);
    bool Q() const;
    unsigned long ET() const;
};
```

---

## Variables internas

### `q`

Representa la salida del temporizador.

```text
q = false -> el temporizador no ha alcanzado PT
q = true  -> el temporizador ha alcanzado PT
```

---

### `start_time`

Guarda el instante en el que empieza la temporización.

Se carga con `millis()` cuando la entrada `IN` pasa a `true` y el temporizador comienza a contar.

---

### `elapsed_time`

Representa el tiempo transcurrido desde que comenzó la temporización.

Equivale a `ET` en un bloque `TON` de PLC.

---

### `timing`

Indica si el temporizador está contando actualmente.

Esta variable evita reiniciar `start_time` en cada vuelta de `loop()` mientras `IN` sigue en `true`.

---

## Métodos principales

### Constructor `TON()`

Inicializa el temporizador en estado de reposo:

```text
Q = false
ET = 0
start_time = 0
timing = false
```

---

### `update(bool in, unsigned long pt)`

Actualiza el estado interno del temporizador.

Debe llamarse de forma cíclica desde `loop()`.

Comportamiento:

```text
Si in == false:
    reset del temporizador

Si in == true y timing == false:
    guardar start_time
    empezar a contar

Si in == true y timing == true:
    actualizar ET
    si ET >= PT, activar Q
```

---

### `Q() const`

Devuelve el estado actual de la salida `Q`.

```cpp
if (ton_boton.Q())
{
    // temporizador cumplido
}
```

---

### `ET() const`

Devuelve el tiempo transcurrido del temporizador, en milisegundos.

---

## Funcionamiento del ejercicio

El pulsador actúa como entrada `IN` del temporizador.

```cpp
bool boton_pulsado = (digitalRead(BUTTON_PIN) == LOW);
```

El temporizador se actualiza en cada vuelta de `loop()`:

```cpp
ton_boton.update(boton_pulsado, 2000);
```

Cuando la salida `Q` está activa, el RGB integrado se enciende en verde:

```cpp
if (ton_boton.Q())
{
    pixel.setPixelColor(0, pixel.Color(0, 255, 0));
}
else
{
    pixel.setPixelColor(0, pixel.Color(0, 0, 0));
}

pixel.show();
```

---

## Resultado esperado

Al arrancar, el monitor serie muestra información similar a:

```text
Ejercicio 08 - Temporizador TON - Básico
BUTTON_PIN = GPIO3
```

Comportamiento esperado:

```text
Botón sin pulsar:
    Q = false
    ET = 0
    RGB apagado

Botón pulsado menos de 2 segundos:
    Q = false
    RGB apagado

Botón mantenido durante 2 segundos o más:
    Q = true
    RGB verde

Botón liberado:
    Q = false
    ET = 0
    RGB apagado
```

---

## Conceptos trabajados

### Estado interno

A diferencia de una función simple, la clase `TON` recuerda información entre llamadas:

- si está contando;
- cuándo empezó a contar;
- cuánto tiempo ha transcurrido;
- si la salida `Q` está activa.

---

### Clase C++ como bloque funcional

El temporizador se representa como un objeto:

```cpp
TON ton_boton;
```

Esto se parece a instanciar un bloque funcional en PLC.

---

### Llamada cíclica

El temporizador no se ejecuta solo.

Debe actualizarse continuamente:

```cpp
ton_boton.update(in, pt);
```

Esto es equivalente a ejecutar un bloque temporizador en cada ciclo de scan de un PLC.

---

### No bloqueo

El temporizador no usa `delay()`.

La medición se realiza mediante `millis()`, lo que permite que `loop()` siga ejecutándose.

---

## Problemas encontrados

### Reinicio incorrecto de `start_time`

Un error típico al implementar un `TON` es reiniciar `start_time` en cada vuelta de `loop()` mientras `IN` está activo.

Si se hiciera eso, el temporizador nunca alcanzaría `PT`.

La variable `timing` evita ese problema.

---

### Constructor incompleto durante el desarrollo

Durante el desarrollo, el constructor quedó inicialmente sin cuerpo.

La corrección consistió en inicializar explícitamente todas las variables internas del bloque.

---

### Método `update()` incompleto durante el desarrollo

Durante el desarrollo, la lógica de `update()` quedó parcialmente escrita.

La corrección consistió en separar claramente:

```text
reset cuando IN = false
arranque de temporización
actualización de ET
activación de Q
```

---

## Relación con ejercicios anteriores

Este ejercicio evoluciona desde:

```text
arduino/07_temporizador_software_simple/
```

En el ejercicio 07 se creó una función reutilizable para temporización periódica.

En este ejercicio se da un paso más: el temporizador pasa a ser un objeto con estado interno.

---

## Relación con PLC

Este ejercicio es el primer bloque claramente inspirado en PLC.

La clase `TON` aproxima el comportamiento de un temporizador a la conexión:

```text
IN -> PT -> Q / ET
```

Este patrón será la base para implementar:

- `TOF`;
- `TP`;
- futuros bloques reutilizables en `lib/plc_blocks`.

---

## Documentación relacionada

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
docs/buenas_practicas.md
docs/lecciones_aprendidas.md
arduino/07_temporizador_software_simple/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Clase `TON` | Validada |
| Constructor `TON()` | Validado |
| Método `update()` | Validado |
| Método `Q()` | Validado |
| Método `ET()` | Validado |
| Entrada `IN` desde pulsador externo | Validada |
| Salida `Q` controlando RGB | Validada |
| Reset al liberar el pulsador | Validado |
