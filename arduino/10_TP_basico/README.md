# Ejercicio 10 - TP básico

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio introduce la implementación de un temporizador tipo **TP** inspirado en PLC, usando una clase C++ con estado interno. El objetivo es generar un pulso temporizado de duración fija cuando se detecta un flanco de activación en la entrada.

---

## Objetivo

Implementar y validar un temporizador tipo `TP` básico.

Este ejercicio permite practicar:

- creación de una clase C++ sencilla;
- estado interno dentro de un objeto;
- equivalencia entre bloques PLC y C++;
- conceptos `IN`, `PT`, `Q` y `ET` aplicados a un pulso temporizado;
- detección interna de flanco ascendente;
- uso de `millis()` sin bloquear `loop()`;
- generación de una salida activa durante un tiempo fijo;
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
arduino/10_tp_basico/
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

## Qué es un TP

Un `TP` es un temporizador de pulso.

Su comportamiento básico es:

```text
IN = false
    Q = false
    ET = 0

IN pasa de false a true
    Q = true inmediatamente
    empieza a contar el tiempo del pulso

Mientras ET < PT
    Q permanece true

Cuando ET >= PT
    Q pasa a false
    el pulso finaliza

Si IN permanece true
    no se genera otro pulso

Para generar un nuevo pulso
    IN debe volver antes a false
    y después pasar de nuevo a true
```

---

## Diferencia entre TON, TOF y TP

| Temporizador | Evento principal | Comportamiento de Q |
|---|---|---|
| `TON` | `IN` pasa a `true` | `Q` se activa cuando `ET >= PT` |
| `TOF` | `IN` pasa a `false` | `Q` se desactiva cuando `ET >= PT` |
| `TP` | Flanco `false -> true` en `IN` | `Q` se activa inmediatamente durante `PT` |

Resumen conceptual:

```text
TON -> retarda la activación
TOF -> retarda la desactivación
TP  -> genera un pulso temporizado
```

---

## Equivalencia PLC ↔ C++

| Concepto PLC | Implementación en este ejercicio |
|---|---|
| `IN` | Parámetro `in` del método `update()` |
| `PT` | Parámetro `pt` del método `update()` |
| `Q` | Variable interna `q` y método `Q()` |
| `ET` | Variable interna `elapsed_time` y método `ET()` |
| Flanco de disparo | Comparación entre `previous_in` e `in` |
| Bloque funcional | Clase `TP` |
| Ciclo PLC | Llamadas repetidas a `update()` desde `loop()` |

---

## Clase `TP`

La clase implementada tiene esta estructura conceptual:

```cpp
class TP
{
private:
    bool q;
    unsigned long start_time;
    unsigned long elapsed_time;
    bool timing;
    bool previous_in;

public:
    TP();
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
q = true  -> el pulso está activo
q = false -> el pulso está inactivo o ya ha finalizado
```

---

### `start_time`

Guarda el instante en el que empieza el pulso temporizado.

Se carga con `millis()` cuando se detecta el flanco de activación.

---

### `elapsed_time`

Representa el tiempo transcurrido desde que comenzó el pulso.

Equivale a `ET` en un bloque `TP` de PLC.

---

### `timing`

Indica si el pulso temporizado está actualmente en curso.

Evita reiniciar el pulso mientras el pulso anterior sigue activo.

---

### `previous_in`

Guarda el valor anterior de la entrada `IN`.

Permite detectar el flanco ascendente:

```text
previous_in = false
in          = true
```

Ese cambio dispara el pulso.

---

## Métodos principales

### Constructor `TP()`

Inicializa el temporizador en estado de reposo:

```text
Q = false
ET = 0
start_time = 0
timing = false
previous_in = false
```

---

### `update(bool in, unsigned long pt)`

Actualiza el estado interno del temporizador.

Debe llamarse de forma cíclica desde `loop()`.

Comportamiento:

```text
Si se detecta flanco ascendente:
    Q = true
    ET = 0
    start_time = millis()
    timing = true

Mientras timing == true:
    actualizar ET
    mantener Q = true mientras ET < PT

Si ET >= PT:
    Q = false
    ET = PT
    timing = false

Al final:
    previous_in = in
```

---

### `Q() const`

Devuelve el estado actual de la salida `Q`.

```cpp
if (tp_boton.Q())
{
    // pulso activo
}
```

---

### `ET() const`

Devuelve el tiempo transcurrido del pulso temporizado, en milisegundos.

---

## Funcionamiento del ejercicio

El pulsador actúa como entrada `IN` del temporizador.

```cpp
bool boton_pulsado = (digitalRead(BUTTON_PIN) == LOW);
```

El temporizador se actualiza en cada vuelta de `loop()`:

```cpp
tp_boton.update(boton_pulsado, 2000);
```

Cuando la salida `Q` está activa, el RGB integrado se mantiene verde:

```cpp
if (tp_boton.Q())
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
Ejercicio 10 - Temporizador TP - Básico
BUTTON_PIN = GPIO3
```

Comportamiento esperado:

```text
Botón sin pulsar:
    Q = false
    ET = 0
    RGB apagado

Botón pulsado:
    Q = true inmediatamente
    RGB verde
    empieza el pulso

Durante PT:
    Q = true
    RGB verde

Pasados 2 segundos:
    Q = false
    RGB apagado

Botón mantenido pulsado después de terminar el pulso:
    Q sigue false
    no se genera un nuevo pulso

Botón liberado y pulsado de nuevo:
    se genera un nuevo pulso
```

---

## Conceptos trabajados

### Pulso temporizado

El `TP` genera una salida activa durante un tiempo fijo cuando detecta un evento de entrada.

El tiempo de pulso no depende de cuánto tiempo permanezca activa la entrada.

---

### Detección de flanco

El temporizador usa `previous_in` para detectar un cambio:

```text
false -> true
```

Esto introduce la base conceptual de los futuros bloques `R_TRIG` y `F_TRIG`.

---

### Estado interno

La clase `TP` recuerda información entre llamadas:

- si el pulso está activo;
- cuándo empezó el pulso;
- cuánto tiempo ha transcurrido;
- cuál era el valor anterior de `IN`.

---

### Clase C++ como bloque funcional

El temporizador se representa como un objeto:

```cpp
TP tp_boton;
```

Esto se parece a instanciar un bloque funcional en PLC.

---

### No bloqueo

El temporizador no usa `delay()`.

La medición se realiza mediante `millis()`, lo que permite que `loop()` siga ejecutándose.

---

## Problemas encontrados

### Constructor y `update()` incompletos durante el desarrollo

Durante el desarrollo, el constructor y el método `update()` quedaron inicialmente incompletos.

La corrección consistió en inicializar todas las variables internas y completar la lógica de detección de flanco y temporización del pulso.

---

### Falta de actualización de `previous_in`

Inicialmente se planteó la detección de flanco, pero faltaba actualizar `previous_in` al final de `update()`.

Sin esta actualización, el bloque no puede detectar correctamente nuevos flancos.

---

### Texto heredado de ejercicios anteriores

Durante el desarrollo apareció texto de diagnóstico como `Salida TON`.

Se corrigió a `Salida TP` para mantener coherencia con el temporizador implementado.

---

## Relación con ejercicios anteriores

Este ejercicio evoluciona desde:

```text
arduino/08_ton_basico/
arduino/09_tof_basico/
```

En los ejercicios anteriores se implementaron temporizadores a la conexión y a la desconexión.

En este ejercicio se implementa un temporizador de pulso.

---

## Relación con PLC

La clase `TP` aproxima el comportamiento de un temporizador de pulso:

```text
flanco de IN -> Q activa durante PT
```

Este patrón será la base para implementar detectores de flanco reutilizables:

- `R_TRIG`;
- `F_TRIG`.

También prepara la extracción futura de bloques a:

```text
lib/plc_blocks
```

---

## Documentación relacionada

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
docs/buenas_practicas.md
docs/lecciones_aprendidas.md
arduino/08_ton_basico/
arduino/09_tof_basico/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Clase `TP` | Validada |
| Constructor `TP()` | Validado |
| Método `update()` | Validado |
| Método `Q()` | Validado |
| Método `ET()` | Validado |
| Detección interna de flanco | Validada |
| Entrada `IN` desde pulsador externo | Validada |
| Salida `Q` controlando RGB | Validada |
| Pulso temporizado | Validado |
