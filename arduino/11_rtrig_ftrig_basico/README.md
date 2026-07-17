# Ejercicio 11 - R_TRIG y F_TRIG básicos

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio introduce la implementación de detectores de flanco inspirados en PLC mediante clases C++ con estado interno. El objetivo es detectar eventos discretos en una señal booleana comparando el estado actual con el estado anterior.

---

## Objetivo

Implementar y validar dos detectores de flanco básicos:

- `R_TRIG`: detector de flanco ascendente.
- `F_TRIG`: detector de flanco descendente.

Este ejercicio permite practicar:

- creación de clases C++ sencillas;
- estado interno dentro de un objeto;
- detección de flanco ascendente;
- detección de flanco descendente;
- comparación entre estado actual y estado anterior;
- uso de señales booleanas;
- separación entre lectura de entrada, detección de evento y acción;
- preparación para una futura librería local `lib/plc_blocks`.

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
arduino/11_rtrig_ftrig_basico/
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

La lógica eléctrica esperada es:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
```

Para trabajar con una señal más cómoda a nivel lógico, se define:

```cpp
bool boton = (digitalRead(BUTTON_PIN) == LOW);
```

Por tanto:

```text
Botón sin pulsar -> boton = false
Botón pulsado    -> boton = true
```

---

## Pines utilizados

| Función | GPIO | Estado |
|---|---:|---|
| Pulsador externo | GPIO3 | Validado |
| RGB integrado | GPIO48 | Validado |

---

## Qué es un detector de flanco

Un detector de flanco permite detectar un cambio puntual en una señal booleana.

En lugar de preguntar si una señal está activa de forma continua, el detector responde solo cuando se produce una transición.

```text
Estado anterior + estado actual -> evento de flanco
```

---

## R_TRIG

`R_TRIG` detecta un flanco ascendente.

```text
false -> true
```

En este ejercicio, sobre la señal lógica `boton`:

```text
false -> true = pulsación del botón
```

La salida `Q` de `R_TRIG` se activa solo durante un ciclo de `loop()` cuando se detecta esa transición.

---

## F_TRIG

`F_TRIG` detecta un flanco descendente.

```text
true -> false
```

En este ejercicio, sobre la señal lógica `boton`:

```text
true -> false = liberación del botón
```

La salida `Q` de `F_TRIG` se activa solo durante un ciclo de `loop()` cuando se detecta esa transición.

---

## Equivalencia PLC ↔ C++

| Concepto PLC | Implementación en este ejercicio |
|---|---|
| `R_TRIG` | Clase `R_TRIG` |
| `F_TRIG` | Clase `F_TRIG` |
| `CLK` | Parámetro `clk` del método `update()` |
| `Q` | Variable interna `q` y método `Q()` |
| Memoria interna | Variable `previous_clk` |
| Ciclo PLC | Llamadas repetidas a `update()` desde `loop()` |

---

## Clase `R_TRIG`

La clase implementada tiene esta estructura conceptual:

```cpp
class R_TRIG
{
private:
    bool q;
    bool previous_clk;

public:
    R_TRIG();
    void update(bool clk);
    bool Q() const;
};
```

### Variables internas

#### `q`

Representa la salida del detector.

```text
q = true  -> flanco ascendente detectado en este ciclo
q = false -> no se ha detectado flanco ascendente
```

#### `previous_clk`

Guarda el valor anterior de la señal de entrada `CLK`.

Permite detectar la transición:

```text
previous_clk = false
clk          = true
```

---

## Clase `F_TRIG`

La clase implementada tiene esta estructura conceptual:

```cpp
class F_TRIG
{
private:
    bool q;
    bool previous_clk;

public:
    F_TRIG();
    void update(bool clk);
    bool Q() const;
};
```

### Variables internas

#### `q`

Representa la salida del detector.

```text
q = true  -> flanco descendente detectado en este ciclo
q = false -> no se ha detectado flanco descendente
```

#### `previous_clk`

Guarda el valor anterior de la señal de entrada `CLK`.

Permite detectar la transición:

```text
previous_clk = true
clk          = false
```

---

## Funcionamiento del ejercicio

En cada vuelta de `loop()` se lee el estado lógico del pulsador:

```cpp
bool boton = (digitalRead(BUTTON_PIN) == LOW);
```

Después se actualizan ambos detectores:

```cpp
r_trig_boton.update(boton);
f_trig_boton.update(boton);
```

Cuando `R_TRIG` detecta un flanco ascendente:

```cpp
if (r_trig_boton.Q())
{
    Serial.println("R_TRIG: Flanco ascendente detectado.");
}
```

Cuando `F_TRIG` detecta un flanco descendente:

```cpp
if (f_trig_boton.Q())
{
    Serial.println("F_TRIG: Flanco descendente detectado.");
}
```

---

## Resultado esperado

Al arrancar, el monitor serie muestra información similar a:

```text
Ejercicio 11 - Detección flanco básico
BUTTON PIN = GPIO 3
```

Al pulsar el botón:

```text
R_TRIG: Flanco ascendente detectado.
```

Al mantener el botón pulsado:

```text
No se repite el mensaje de R_TRIG.
```

Al soltar el botón:

```text
F_TRIG: Flanco descendente detectado.
```

Al mantener el botón suelto:

```text
No se repite el mensaje de F_TRIG.
```

---

## Conceptos trabajados

### Flanco ascendente

Un flanco ascendente ocurre cuando una señal pasa de `false` a `true`.

```text
false -> true
```

En este ejercicio corresponde a la pulsación del botón sobre la señal lógica `boton`.

---

### Flanco descendente

Un flanco descendente ocurre cuando una señal pasa de `true` a `false`.

```text
true -> false
```

En este ejercicio corresponde a la liberación del botón sobre la señal lógica `boton`.

---

### Estado interno

Los bloques `R_TRIG` y `F_TRIG` necesitan recordar el valor anterior de la señal para comparar:

```text
estado anterior vs estado actual
```

Esa memoria se guarda en `previous_clk`.

---

### Salida activa durante un ciclo

La salida `Q` de cada detector solo permanece activa durante una ejecución de `update()`.

Esto permite convertir un cambio de nivel en un evento puntual.

---

### Separación de responsabilidades

El ejercicio separa:

```text
lectura del GPIO -> señal booleana -> detector de flanco -> acción
```

Los bloques de flanco no leen directamente el GPIO. Solo reciben una señal booleana.

---

## Consideración importante: rebote del pulsador

Los bloques `R_TRIG` y `F_TRIG` detectan flancos, pero no filtran rebotes mecánicos.

Si el pulsador genera rebotes, los detectores pueden detectar varios flancos rápidos.

Por tanto, en aplicaciones reales conviene usar esta secuencia:

```text
lectura de entrada -> antirrebote -> señal estable -> R_TRIG / F_TRIG
```

Este ejercicio se centra únicamente en la detección de flancos.

---

## Problemas encontrados

### Diferencia entre flanco eléctrico y señal lógica

Con `INPUT_PULLUP`, el GPIO físico baja a `LOW` al pulsar.

Sin embargo, el programa transforma esa lectura en una señal lógica más clara:

```cpp
bool boton = (digitalRead(BUTTON_PIN) == LOW);
```

Así, la pulsación se interpreta como:

```text
false -> true
```

Esto facilita razonar de forma similar a PLC.

---

### Riesgo de confundir detección de flanco con antirrebote

Se recuerda que detectar flancos no equivale a eliminar rebote.

Los detectores responden a los cambios que reciben. Si la señal rebota, pueden detectar varios eventos.

---

## Relación con ejercicios anteriores

Este ejercicio evoluciona desde:

```text
arduino/10_tp_basico/
```

En el ejercicio `TP`, el temporizador necesitaba detectar internamente un flanco ascendente.

En este ejercicio se extrae ese concepto a dos bloques independientes:

- `R_TRIG`;
- `F_TRIG`.

---

## Relación con PLC

Los bloques `R_TRIG` y `F_TRIG` son equivalentes a detectores de flanco usados habitualmente en programación PLC.

Este ejercicio completa el conjunto básico que posteriormente podrá extraerse a una librería local:

```text
TON
TOF
TP
R_TRIG
F_TRIG
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
arduino/10_tp_basico/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Clase `R_TRIG` | Validada |
| Clase `F_TRIG` | Validada |
| Constructor `R_TRIG()` | Validado |
| Constructor `F_TRIG()` | Validado |
| Método `update()` de `R_TRIG` | Validado |
| Método `update()` de `F_TRIG` | Validado |
| Método `Q()` de `R_TRIG` | Validado |
| Método `Q()` de `F_TRIG` | Validado |
| Detección de pulsación | Validada con `R_TRIG` |
| Detección de liberación | Validada con `F_TRIG` |
