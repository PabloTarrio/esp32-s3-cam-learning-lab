# Ejercicio 12 - Validación de librería local `plc_blocks`

Ejercicio de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio valida por primera vez el uso de la librería local `lib/plc_blocks` desde un proyecto PlatformIO independiente. El objetivo ya no es implementar los bloques dentro de `main.cpp`, sino importarlos desde la librería y comprobar que funcionan correctamente en un ejercicio Arduino real.

---

## Objetivo

Validar el uso de la librería local:

```text
lib/plc_blocks/
```

Desde un proyecto Arduino ubicado en:

```text
arduino/12_plc_blocks_library/
```

El ejercicio comprueba:

- que PlatformIO encuentra correctamente la librería local;
- que se pueden incluir `PlcTimers.h` y `PlcEdges.h`;
- que se puede usar `R_TRIG` desde la librería;
- que se puede usar `TP` desde la librería;
- que los bloques reutilizables pueden combinarse en una aplicación real;
- que el RGB integrado puede controlarse mediante la salida `Q` de un bloque PLC reutilizable.

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
arduino/12_plc_blocks_library/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

La librería usada por este ejercicio está en:

```text
lib/
└── plc_blocks/
    ├── README.md
    ├── library.json
    └── src/
        ├── PlcTimers.h
        ├── PlcTimers.cpp
        ├── PlcEdges.h
        └── PlcEdges.cpp
```

---

## Configuración PlatformIO

El aspecto clave de este ejercicio está en `platformio.ini`.

Además de la configuración habitual, se añade:

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

La ruta `../../lib` permite que el proyecto encuentre la librería local ubicada en la raíz del repositorio.

---

## Includes utilizados

En `main.cpp` se importan los bloques reutilizables:

```cpp
#include <PlcTimers.h>
#include <PlcEdges.h>
```

Estos archivos proceden de:

```text
lib/plc_blocks/src/
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

Para trabajar con una señal booleana más cómoda:

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

## Bloques usados en este ejercicio

### `R_TRIG`

Se usa para detectar la pulsación del botón:

```text
boton false -> true
```

Es decir, cuando el botón pasa de no pulsado a pulsado.

---

### `TP`

Se usa para generar un pulso temporizado de 2000 ms.

El pulso se dispara con la salida de `R_TRIG`:

```cpp
tp_boton.update(r_trig_boton.Q(), TP_TIME_MS);
```

Mientras `TP.Q()` está en `true`, el RGB integrado permanece encendido en verde.

---

## Funcionamiento general

El flujo lógico del programa es:

```text
GPIO3
↓
señal lógica boton
↓
R_TRIG
↓
TP de 2000 ms
↓
RGB verde mientras TP.Q() == true
```

En código:

```cpp
bool boton = (digitalRead(BUTTON_PIN) == LOW);

r_trig_boton.update(boton);

tp_boton.update(r_trig_boton.Q(), TP_TIME_MS);

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
Ejercicio 12 - PLC Blocks Library
BUTTON_PIN = GPIO3
RGB_PIN = GPIO48
```

Al pulsar el botón:

```text
R_TRIG detecta el flanco ascendente
TP genera un pulso de 2000 ms
RGB verde durante el pulso
```

Al mantener el botón pulsado:

```text
El pulso no se reinicia continuamente
```

Al terminar el pulso:

```text
TP.Q() pasa a false
RGB apagado
```

Para generar otro pulso:

```text
Soltar el botón
Pulsar de nuevo
```

---

## Conceptos trabajados

### Librería local PlatformIO

El ejercicio valida que un proyecto dentro de `arduino/` puede usar una librería local ubicada en la raíz del repositorio.

Esto permite reutilizar código sin duplicarlo en cada ejercicio.

---

### Separación entre implementación y uso

Los bloques `R_TRIG` y `TP` ya no están implementados dentro de `main.cpp`.

Ahora se usan desde:

```text
lib/plc_blocks/src/
```

Esto separa:

```text
librería reutilizable -> implementación de bloques
proyecto Arduino      -> aplicación concreta de los bloques
```

---

### Reutilización de bloques PLC

Este ejercicio combina dos bloques reutilizables:

```text
R_TRIG -> detección de evento
TP     -> generación de pulso temporizado
```

La combinación permite crear una lógica más expresiva y más cercana a programación PLC.

---

### Entrada booleana procesada

La librería no lee directamente GPIO.

El programa principal transforma la entrada física en una señal lógica:

```cpp
bool boton = (digitalRead(BUTTON_PIN) == LOW);
```

Después entrega esa señal a los bloques:

```cpp
r_trig_boton.update(boton);
```

---

## Consideración sobre rebotes

Este ejercicio no implementa antirrebote dentro de la librería.

Los bloques `R_TRIG` y `TP` reciben la señal que el programa les entrega. Si la señal del pulsador rebota, se podrían generar varios flancos.

La mejora natural posterior será añadir un bloque reutilizable de antirrebote:

```text
DEBOUNCE
```

La cadena recomendada será:

```text
GPIO -> señal lógica -> DEBOUNCE -> R_TRIG -> TP
```

---

## Problemas encontrados

### Uso incorrecto inicial de `R_TRIG`

Inicialmente se planteó llamar a:

```cpp
r_trig_boton.update(BUTTON_PIN);
```

Eso no es correcto porque `R_TRIG::update()` espera una señal booleana, no el número del GPIO.

La forma correcta es:

```cpp
bool boton = (digitalRead(BUTTON_PIN) == LOW);
r_trig_boton.update(boton);
```

---

### Mezcla inicial con `detectar_pulsacion_valida()`

Se consideró usar una función local de antirrebote y pulsación válida.

Sin embargo, para validar la librería era más limpio alimentar `R_TRIG` con una señal booleana directa y dejar el antirrebote reutilizable para un bloque posterior.

---

## Relación con ejercicios anteriores

Este ejercicio se apoya en los bloques desarrollados en:

```text
arduino/08_ton_basico/
arduino/09_tof_basico/
arduino/10_tp_basico/
arduino/11_rtrig_ftrig_basico/
```

La diferencia es que ahora esos bloques viven en una librería local:

```text
lib/plc_blocks/
```

---

## Relación con PLC

El ejercicio valida una cadena lógica similar a una programación PLC:

```text
entrada booleana -> flanco -> pulso temporizado -> salida
```

En bloques:

```text
boton -> R_TRIG -> TP -> RGB
```

---

## Documentación relacionada

```text
lib/plc_blocks/README.md
lib/plc_blocks/library.json
lib/plc_blocks/src/PlcTimers.h
lib/plc_blocks/src/PlcTimers.cpp
lib/plc_blocks/src/PlcEdges.h
lib/plc_blocks/src/PlcEdges.cpp
arduino/08_ton_basico/
arduino/09_tof_basico/
arduino/10_tp_basico/
arduino/11_rtrig_ftrig_basico/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Inclusión de `PlcTimers.h` | Validada |
| Inclusión de `PlcEdges.h` | Validada |
| Uso de `R_TRIG` desde librería | Validado |
| Uso de `TP` desde librería | Validado |
| Configuración `lib_extra_dirs` | Validada |
| RGB controlado desde bloques reutilizables | Validado |
| Librería local `plc_blocks` | Validada inicialmente |

---

## Próximo paso

Añadir un bloque reutilizable de antirrebote a la librería:

```text
PlcDebounce.h
PlcDebounce.cpp
```

Y validar la cadena:

```text
GPIO -> DEBOUNCE -> R_TRIG -> TP
```
