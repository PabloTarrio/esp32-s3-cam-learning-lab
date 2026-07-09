# Ejercicio 05 - `millis()` no bloqueante

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio introduce el patrón básico de temporización no bloqueante usando `millis()`, sustituyendo el uso de `delay()` para hacer parpadear el LED RGB integrado de la placa ESP32-S3-CAM.

---

## Objetivo

Hacer parpadear el LED RGB integrado sin usar `delay()`.

Este ejercicio permite practicar:

- uso básico de `millis()`;
- temporización no bloqueante;
- cambio periódico de estado;
- control del LED RGB integrado en `GPIO48`;
- uso de variables de estado;
- estructura básica para ejecutar otras tareas dentro de `loop()` sin bloquear el programa.

---

## Hardware utilizado

- ESP32-S3-CAM.
- LED RGB integrado NeoPixel / WS2812.
- Cable USB.

No requiere cableado externo.

---

## Software utilizado

- Visual Studio Code.
- PlatformIO.
- Arduino Framework.
- Librería `Adafruit_NeoPixel`.

---

## Estructura del ejercicio

```text
arduino/05_millis_no_bloqueante/
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

## Pines utilizados

| Función | GPIO | Estado |
|---|---:|---|
| RGB integrado | GPIO48 | Validado |

---

## Funcionamiento

El programa hace parpadear el LED RGB integrado sin detener la ejecución de `loop()`.

En lugar de escribir:

```cpp
delay(500);
```

se usa el patrón:

```cpp
unsigned long current_millis = millis();

if (current_millis - previous_millis >= BLINK_INTERVAL_MS)
{
    previous_millis = current_millis;
    led_state = !led_state;
    aplicar_estado_rgb(pixel, led_state);
}
```

La idea principal es comprobar periódicamente si ya ha transcurrido el intervalo deseado, sin bloquear el programa mientras tanto.

---

## Resultado esperado

Al arrancar, el monitor serie muestra:

```text
Ejercicio 05 - millis no bloqueante
RGB integrado parpadeando sin delay()
```

Después, el LED RGB integrado parpadea en azul:

```text
500 ms encendido
500 ms apagado
```

El monitor serie muestra cambios de estado:

```text
LED state: ON
LED state: OFF
LED state: ON
LED state: OFF
```

---

## Conceptos trabajados

### `delay()` bloqueante

`delay()` detiene la ejecución del programa durante el tiempo indicado.

Durante un `delay()`, el programa no puede atender otras tareas de forma normal, por ejemplo:

- leer botones;
- procesar eventos;
- actualizar otros temporizadores;
- gestionar comunicaciones;
- cambiar estados de una máquina de estados.

---

### `millis()`

`millis()` devuelve el tiempo transcurrido desde que arrancó el programa, expresado en milisegundos.

Permite implementar temporizaciones comparando el tiempo actual con un tiempo anterior guardado.

---

### Temporización no bloqueante

La temporización no bloqueante consiste en preguntar:

```text
¿Ha pasado ya el intervalo configurado?
```

Si no ha pasado, el programa continúa ejecutando `loop()`.

Si ha pasado, se ejecuta la acción temporizada y se actualiza el instante de referencia.

---

### Variable de estado

El estado del LED se guarda en una variable booleana:

```cpp
bool led_state = false;
```

Cada vez que se cumple el intervalo, el estado cambia:

```cpp
led_state = !led_state;
```

Esto permite alternar entre:

```text
apagado -> encendido -> apagado -> encendido
```

---

## Relación con PLC

Este ejercicio todavía no implementa un temporizador tipo `TON`, `TOF` o `TP`, pero introduce el patrón base necesario para construirlos.

En términos conceptuales:

| Concepto PLC | Equivalente en este ejercicio |
|---|---|
| Tiempo actual del ciclo | `millis()` |
| Tiempo anterior guardado | `previous_millis` |
| Tiempo programado | `BLINK_INTERVAL_MS` |
| Salida temporizada | `led_state` |
| Ejecución cíclica | `loop()` |

Este patrón será la base para crear temporizadores reutilizables inspirados en bloques PLC.

---

## Problemas encontrados

No se han detectado problemas relevantes en esta primera versión.

El ejercicio ha validado correctamente:

- compilación;
- subida a placa;
- parpadeo del RGB integrado;
- funcionamiento sin `delay()`;
- salida por monitor serie.

---

## Documentación relacionada

```text
hardware/rgb_neopixel.md
hardware/pinout.md
hardware/gpio_reservados.md
docs/buenas_practicas.md
docs/lecciones_aprendidas.md
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| RGB integrado en GPIO48 | Validado |
| Parpadeo sin `delay()` | Validado |
| Uso de `millis()` | Validado |
| Monitor serie | Validado |

---

## Evolución prevista dentro de la fase

Este ejercicio es el primer paso de la Fase 4.

A partir de este patrón se trabajarán conceptos más avanzados:

- temporizador software simple;
- varias tareas simuladas en `loop()`;
- RGB + botón sin bloqueos;
- máquina de estados básica;
- temporizadores tipo `TON`, `TOF` y `TP`;
- detección de flancos `R_TRIG` y `F_TRIG`;
- extracción de bloques reutilizables a `lib/plc_blocks`.
