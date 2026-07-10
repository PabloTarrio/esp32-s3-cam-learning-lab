# Ejercicio 06 - Máquina de estados básica

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio formaliza la lógica de modos trabajada en ejercicios anteriores mediante una **máquina de estados básica**. El programa sustituye modos numéricos por estados con nombre usando `enum`, mantiene ejecución cooperativa en `loop()`, lee un pulsador externo con antirrebote y controla el LED RGB integrado según el estado actual del sistema.

---

## Objetivo

Formalizar una lógica de modos como una máquina de estados sencilla.

Este ejercicio permite practicar:

- definición de estados mediante `enum`;
- separación entre evento, transición y acción de estado;
- lectura de pulsador externo con antirrebote;
- cambio de estado mediante pulsaciones válidas;
- ejecución cooperativa dentro de `loop()`;
- temporización no bloqueante con `millis()`;
- control del LED RGB integrado según el estado actual;
- impresión periódica de diagnóstico por monitor serie.

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
arduino/06_maquina_estado_basica/
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

## Estados del sistema

La máquina de estados se define mediante un `enum`:

```cpp
enum EstadoSistema
{
    ESTADO_PARPADEO_AZUL,
    ESTADO_VERDE_FIJO,
    ESTADO_APAGADO
};
```

Estados implementados:

| Estado | Comportamiento |
|---|---|
| `ESTADO_PARPADEO_AZUL` | RGB parpadea en azul sin usar `delay()` |
| `ESTADO_VERDE_FIJO` | RGB permanece fijo en verde |
| `ESTADO_APAGADO` | RGB permanece apagado |

---

## Transiciones

Cada pulsación válida avanza al siguiente estado:

```text
ESTADO_PARPADEO_AZUL
        ↓
ESTADO_VERDE_FIJO
        ↓
ESTADO_APAGADO
        ↓
ESTADO_PARPADEO_AZUL
```

La transición se realiza en la rutina:

```cpp
void actualizar_estado(EstadoSistema &estado)
```

Esta rutina modifica el estado recibido por referencia.

---

## Funcionamiento general

El flujo principal del programa separa tres responsabilidades:

1. Detectar eventos de entrada.
2. Actualizar el estado de la máquina.
3. Ejecutar el comportamiento asociado al estado actual.

Estructura conceptual:

```cpp
void loop()
{
    if (detectar_pulsacion_valida())
    {
        actualizar_estado(estado_actual);
        ejecutar_estado_actual(pixel, estado_actual);
    }

    if (estado_actual == ESTADO_PARPADEO_AZUL)
    {
        actualizar_parpadeo_azul(pixel);
    }

    imprimir_estado_periodico(estado_actual);
}
```

Este patrón permite que el programa mantenga tareas cooperativas sin bloquear `loop()`.

---

## Resultado esperado

Al arrancar, el monitor serie muestra información similar a:

```text
Ejercicio 06 - Máquina de Estados Básica
BUTTON_PIN = GPIO3
RGB_PIN = GPIO48
Modo inicial: 0 -> parpadeo azul
```

Durante la ejecución, el monitor serie imprime periódicamente el estado actual:

```text
· Modo Actual: ESTADO_PARPADEO_AZUL
```

Al pulsar y soltar el botón, el sistema cambia al siguiente estado:

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

## Rutinas principales

### `detectar_pulsacion_valida()`

Lee el pulsador externo, aplica antirrebote mediante `millis()` y devuelve `true` solo cuando se detecta una liberación válida del pulsador.

---

### `actualizar_estado()`

Gestiona las transiciones de la máquina de estados.

No aplica salidas físicas. Su responsabilidad es cambiar el estado actual siguiendo la secuencia definida.

---

### `ejecutar_estado_actual()`

Ejecuta el comportamiento asociado al estado actual.

- En `ESTADO_PARPADEO_AZUL`, llama a `actualizar_parpadeo_azul()`.
- En `ESTADO_VERDE_FIJO`, aplica color verde fijo.
- En `ESTADO_APAGADO`, apaga el RGB.

---

### `actualizar_parpadeo_azul()`

Gestiona el parpadeo azul no bloqueante del LED RGB integrado.

Usa `millis()` para alternar entre encendido y apagado sin usar `delay()`.

---

### `imprimir_estado_periodico()`

Imprime periódicamente el estado actual por monitor serie sin bloquear la ejecución del programa.

---

## Conceptos trabajados

### Máquina de estados

Una máquina de estados organiza el comportamiento del programa en estados definidos y transiciones entre esos estados.

En este ejercicio:

```text
estado actual + evento -> nuevo estado
```

El evento principal es una pulsación válida del botón.

---

### Estados con nombre

En vez de usar números como:

```cpp
int current_mode = 0;
```

se usa un `enum`:

```cpp
EstadoSistema estado_actual = ESTADO_PARPADEO_AZUL;
```

Esto mejora la legibilidad del código y reduce la dependencia de números mágicos.

---

### Separación entre transición y salida

El ejercicio separa:

```text
actualizar_estado()        -> cambia el estado
activar/ejecutar estado    -> aplica el comportamiento asociado
```

Esto ayuda a evitar mezclar lectura de entradas, lógica de transición y control de salidas.

---

### Temporización no bloqueante

El estado `ESTADO_PARPADEO_AZUL` usa `millis()` para parpadear sin bloquear el programa.

Esto permite que el sistema siga leyendo el pulsador e imprimiendo el estado por serie.

---

### Ejecución cooperativa

El programa mantiene varias tareas activas dentro de `loop()`:

- lectura de pulsador;
- transición de estado;
- ejecución del estado actual;
- parpadeo no bloqueante;
- impresión periódica.

Ninguna de estas tareas usa `delay()`.

---

## Problemas encontrados

### Doble detección de pulsación

Durante el desarrollo se llamó a `detectar_pulsacion_valida()` tanto en `loop()` como dentro de `actualizar_estado()`.

Eso podía consumir el evento en la primera llamada y provocar que la transición no se ejecutase correctamente en la segunda.

La solución fue separar responsabilidades:

```text
loop() detecta el evento
actualizar_estado() cambia el estado
```

Por tanto, `actualizar_estado()` no debe volver a leer el botón.

---

### Impresión de estados como números

Inicialmente, imprimir el valor del `enum` mostraba números como `0`, `1` o `2`.

Para mejorar el diagnóstico, se imprimen nombres explícitos:

```text
ESTADO_PARPADEO_AZUL
ESTADO_VERDE_FIJO
ESTADO_APAGADO
```

---

## Relación con ejercicios anteriores

Este ejercicio evoluciona directamente desde:

```text
arduino/05c_tareas_cooperativas/
```

La diferencia principal es que los modos numéricos se sustituyen por una máquina de estados básica con estados nominales.

---

## Relación con PLC

El ejercicio se parece a una lógica PLC donde se separan:

```text
lectura de entradas
transiciones de estado
ejecución de acciones por estado
actualización de salidas
```

La diferencia es que aquí se implementa en C++ sobre Arduino Framework usando `loop()` y `millis()`.

Este ejercicio prepara el camino para:

- máquinas de estado más completas;
- acciones de entrada a estado;
- acciones cíclicas de estado;
- acciones de salida de estado;
- temporizadores tipo `TON`, `TOF` y `TP`;
- detectores de flanco `R_TRIG` y `F_TRIG`.

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
arduino/05c_tareas_cooperativas/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Pulsador externo en GPIO3 | Validado |
| RGB integrado en GPIO48 | Validado |
| Uso de `enum` para estados | Validado |
| Transiciones por pulsación válida | Validado |
| Parpadeo no bloqueante | Validado |
| Impresión periódica de estado | Validado |
| Separación entre evento, transición y acción | Validado |
