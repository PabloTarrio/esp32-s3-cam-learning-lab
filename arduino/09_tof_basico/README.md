# Ejercicio 09 - TOF básico

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio introduce la implementación de un temporizador tipo **TOF** inspirado en PLC, usando una clase C++ con estado interno. El objetivo es entender el retardo a la desconexión: una salida se activa inmediatamente cuando la entrada está activa y permanece activa durante un tiempo después de que la entrada se desactive.

---

## Objetivo

Implementar y validar un temporizador tipo `TOF` básico.

Este ejercicio permite practicar:

- creación de una clase C++ sencilla;
- estado interno dentro de un objeto;
- equivalencia entre bloques PLC y C++;
- conceptos `IN`, `PT`, `Q` y `ET` aplicados a un retardo a la desconexión;
- uso de `millis()` sin bloquear `loop()`;
- mantenimiento temporal de una salida después de desactivar la entrada;
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
arduino/09_tof_basico/
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

## Qué es un TOF

Un `TOF` es un temporizador a la desconexión.

Su comportamiento básico es:

```text
IN = true
    Q = true
    ET = 0
    el temporizador queda reseteado

IN pasa a false
    empieza a contar tiempo
    Q sigue en true mientras ET < PT

IN permanece false durante PT
    Q pasa a false

IN vuelve a true
    Q = true
    ET = 0
    el temporizador se reinicia
```

---

## Diferencia entre TON y TOF

| Temporizador | Cuándo temporiza | Comportamiento de Q |
|---|---|---|
| `TON` | Al activar `IN` | `Q` se activa cuando `ET >= PT` |
| `TOF` | Al desactivar `IN` | `Q` se desactiva cuando `ET >= PT` |

Resumen conceptual:

```text
TON -> retarda la activación
TOF -> retarda la desactivación
```

---

## Equivalencia PLC ↔ C++

| Concepto PLC | Implementación en este ejercicio |
|---|---|
| `IN` | Parámetro `in` del método `update()` |
| `PT` | Parámetro `pt` del método `update()` |
| `Q` | Variable interna `q` y método `Q()` |
| `ET` | Variable interna `elapsed_time` y método `ET()` |
| Bloque funcional | Clase `TOF` |
| Ciclo PLC | Llamadas repetidas a `update()` desde `loop()` |

---

## Clase `TOF`

La clase implementada tiene esta estructura conceptual:

```cpp
class TOF
{
private:
    bool q;
    unsigned long start_time;
    unsigned long elapsed_time;
    bool timing;

public:
    TOF();
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
q = true  -> la salida sigue activa
q = false -> el retardo a la desconexión ha finalizado
```

---

### `start_time`

Guarda el instante en el que empieza la temporización de desconexión.

Se carga con `millis()` cuando `IN` pasa a `false` y `Q` todavía debe mantenerse activa durante el tiempo `PT`.

---

### `elapsed_time`

Representa el tiempo transcurrido desde que comenzó la temporización de desconexión.

Equivale a `ET` en un bloque `TOF` de PLC.

---

### `timing`

Indica si el temporizador está contando actualmente el retardo a la desconexión.

Esta variable evita reiniciar `start_time` en cada vuelta de `loop()` mientras `IN` permanece en `false` y `Q` sigue activa.

---

## Métodos principales

### Constructor `TOF()`

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
Si in == true:
    Q = true
    ET = 0
    timing = false
    reset de temporización de desconexión

Si in == false y Q == true:
    si timing == false:
        guardar start_time
        empezar a contar

    si timing == true:
        actualizar ET
        si ET >= PT, desactivar Q

Si in == false y Q == false:
    no hay temporización pendiente
```

---

### `Q() const`

Devuelve el estado actual de la salida `Q`.

```cpp
if (tof_boton.Q())
{
    // salida activa
}
```

---

### `ET() const`

Devuelve el tiempo transcurrido de la temporización de desconexión, en milisegundos.

---

## Funcionamiento del ejercicio

El pulsador actúa como entrada `IN` del temporizador.

```cpp
bool boton_pulsado = (digitalRead(BUTTON_PIN) == LOW);
```

El temporizador se actualiza en cada vuelta de `loop()`:

```cpp
tof_boton.update(boton_pulsado, 2000);
```

Cuando la salida `Q` está activa, el RGB integrado se mantiene verde:

```cpp
if (tof_boton.Q())
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
Ejercicio 09 - Temporizador TOF - Básico
BUTTON_PIN = GPIO3
```

Comportamiento esperado:

```text
Botón sin pulsar inicialmente:
    Q = false
    ET = 0
    RGB apagado

Botón pulsado:
    Q = true inmediatamente
    RGB verde

Botón liberado:
    empieza ET
    Q sigue true
    RGB sigue verde

Botón liberado durante 2 segundos o más:
    Q = false
    RGB apagado

Botón pulsado de nuevo:
    Q = true inmediatamente
    ET = 0
    RGB verde
```

---

## Conceptos trabajados

### Retardo a la desconexión

El `TOF` mantiene activa la salida durante un tiempo después de que la entrada se desactive.

Esto es útil en situaciones donde se desea prolongar una acción después de desaparecer la condición de entrada.

---

### Estado interno

La clase `TOF` recuerda información entre llamadas:

- si la salida `Q` está activa;
- si se está temporizando la desconexión;
- cuándo empezó la temporización;
- cuánto tiempo ha transcurrido.

---

### Clase C++ como bloque funcional

El temporizador se representa como un objeto:

```cpp
TOF tof_boton;
```

Esto se parece a instanciar un bloque funcional en PLC.

---

### Llamada cíclica

El temporizador debe actualizarse continuamente:

```cpp
tof_boton.update(in, pt);
```

Esto equivale a ejecutar un bloque temporizador en cada ciclo de scan de un PLC.

---

### No bloqueo

El temporizador no usa `delay()`.

La medición se realiza mediante `millis()`, lo que permite que `loop()` siga ejecutándose.

---

## Problemas encontrados

### Confusión inicial con la lógica de TON

Durante el desarrollo apareció una confusión inicial: se planteó que el temporizador empezase a contar al activar `IN`.

Eso corresponde a un `TON`, no a un `TOF`.

La corrección fue recordar la regla:

```text
TON -> temporiza al conectar
TOF -> temporiza al desconectar
```

---

### Uso de asignación en lugar de comparación

Durante el desarrollo se escribió una condición de este estilo:

```cpp
if (timing = false)
```

Eso asigna `false` a `timing`, pero no compara.

La forma correcta es:

```cpp
if (timing == false)
```

O de forma más idiomática:

```cpp
if (!timing)
```

---

### Comentarios heredados de TON

Parte de la documentación inicial del método `update()` arrastraba lógica propia de `TON`.

Se corrigió la descripción para reflejar que el `TOF` temporiza cuando `IN` pasa a `false`.

---

## Relación con ejercicios anteriores

Este ejercicio evoluciona desde:

```text
arduino/08_ton_basico/
```

En el ejercicio 08 se implementó un temporizador a la conexión.

En este ejercicio se implementa el temporizador complementario: retardo a la desconexión.

---

## Relación con PLC

La clase `TOF` aproxima el comportamiento de un temporizador a la desconexión:

```text
IN activa Q inmediatamente
IN false inicia temporización
ET >= PT desactiva Q
```

Este patrón será la base para implementar:

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
arduino/08_ton_basico/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Clase `TOF` | Validada |
| Constructor `TOF()` | Validado |
| Método `update()` | Validado |
| Método `Q()` | Validado |
| Método `ET()` | Validado |
| Entrada `IN` desde pulsador externo | Validada |
| Salida `Q` controlando RGB | Validada |
| Retardo a la desconexión | Validado |
