# Ejercicio 05b - RGB y botón sin bloqueos

Ejercicio de consolidación dentro de la **Fase 4 - Temporización no bloqueante** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio combina el parpadeo no bloqueante del LED RGB integrado con la lectura de un pulsador externo, demostrando que el programa puede atender varias tareas dentro de `loop()` sin usar `delay()`.

---

## Objetivo

Demostrar que, usando `millis()`, el programa puede realizar varias tareas de forma cooperativa:

- hacer parpadear el LED RGB integrado sin bloquear el programa;
- leer un pulsador externo conectado a `GPIO3`;
- aplicar antirrebote por software;
- detectar una pulsación válida;
- cambiar el modo de funcionamiento con cada pulsación válida;
- mantener el código organizado mediante funciones con responsabilidades claras.

Este ejercicio sirve como puente entre el uso básico de `millis()` y la futura implementación de máquinas de estado y temporizadores tipo PLC.

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
arduino/05b_millis_rgb_boton/
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

En este ejercicio se considera pulsación válida la liberación del pulsador, es decir, el flanco ascendente:

```text
LOW -> HIGH = liberación del pulsador
```

---

## Pines utilizados

| Función | GPIO | Estado |
|---|---:|---|
| Pulsador externo | GPIO3 | Validado |
| RGB integrado | GPIO48 | Validado |

---

## Modos de funcionamiento

El programa tiene tres modos:

| Modo | Comportamiento |
|---:|---|
| 0 | RGB parpadea en azul sin usar `delay()` |
| 1 | RGB permanece fijo en verde |
| 2 | RGB permanece apagado |

Cada pulsación válida cambia al siguiente modo.

Cuando se supera el modo máximo, el programa vuelve al modo `0`.

```text
Modo 0 -> Modo 1 -> Modo 2 -> Modo 0 -> ...
```

---

## Funcionamiento

El programa ejecuta dos tareas principales dentro de `loop()`:

1. Detectar pulsaciones válidas del botón.
2. Actualizar el parpadeo del RGB si el sistema está en modo `0`.

La estructura conceptual es:

```cpp
void loop()
{
    if (detectar_pulsacion_valida())
    {
        cambiar_modo();
        aplicar_modo_actual(pixel);
    }

    if (current_mode == 0)
    {
        actualizar_parpadeo_rgb(pixel);
    }
}
```

Esto permite que el RGB parpadee sin bloquear la lectura del botón.

---

## Resultado esperado

Al arrancar, el monitor serie muestra:

```text
Ejercicio 05b - RGB y boton sin bloqueos
BUTTON_PIN = GPIO3
RGB_PIN = GPIO48
Modo inicial: 0 -> parpadeo azul
```

En modo `0`, el RGB integrado parpadea en azul.

Al pulsar y soltar el pulsador, el programa cambia de modo:

```text
Pulsacion valida numero: 1
Modo actual: 1
```

El RGB queda fijo en verde.

Con otra pulsación válida:

```text
Pulsacion valida numero: 2
Modo actual: 2
```

El RGB queda apagado.

Con otra pulsación válida:

```text
Pulsacion valida numero: 3
Modo actual: 0
```

El RGB vuelve a parpadear en azul.

---

## Conceptos trabajados

### Temporización no bloqueante

El parpadeo del RGB se gestiona con `millis()`:

```cpp
if (current_millis - previous_blink_millis >= BLINK_INTERVAL_MS)
{
    previous_blink_millis = current_millis;
    blink_state = !blink_state;
}
```

Esto evita usar `delay()` y permite que el programa siga atendiendo otras tareas.

---

### Antirrebote por software

El pulsador se filtra con una ventana temporal:

```cpp
const unsigned long DEBOUNCE_TIME_MS = 50;
```

Solo se acepta un cambio de estado cuando la lectura permanece estable durante el tiempo definido.

---

### Detección de pulsación válida

Con `INPUT_PULLUP`:

```text
HIGH -> LOW = pulsar
LOW -> HIGH = liberar
```

En este ejercicio se considera pulsación válida el evento de liberación:

```cpp
if (previous_button_state == LOW && button_state == HIGH)
{
    return true;
}
```

---

### Variables de estado

El programa usa variables para recordar el estado del sistema entre ciclos de `loop()`:

```cpp
bool blink_state = false;
int current_mode = 0;
unsigned long previous_blink_millis = 0;
```

Esto permite construir lógica no bloqueante y preparar el camino hacia máquinas de estado.

---

### Funciones con responsabilidades claras

El programa se divide en rutinas específicas:

- `aplicar_color()` aplica un color al RGB.
- `actualizar_parpadeo_rgb()` gestiona el parpadeo no bloqueante.
- `detectar_pulsacion_valida()` lee el botón y aplica antirrebote.
- `cambiar_modo()` incrementa el modo actual.
- `aplicar_modo_actual()` aplica la salida asociada al modo actual.

---

## Problemas encontrados

### Llamada a función sin paréntesis

Se escribió:

```cpp
if (detectar_pulsacion_valida)
```

en lugar de:

```cpp
if (detectar_pulsacion_valida())
```

El primer caso no ejecuta la función. Evalúa la referencia a la función, lo que provocaba que la condición fuese verdadera continuamente y se generasen pulsaciones falsas.

---

### Operador incorrecto para invertir estado

Se escribió:

```cpp
blink_state != blink_state;
```

en lugar de:

```cpp
blink_state = !blink_state;
```

El primer caso compara valores, pero no modifica la variable. El segundo caso invierte correctamente el estado booleano.

---

## Relación con máquinas de estado

Aunque este ejercicio todavía no implementa formalmente una máquina de estados, ya introduce una estructura basada en modos:

```text
Modo 0 -> parpadeo azul
Modo 1 -> verde fijo
Modo 2 -> apagado
```

El siguiente paso natural será formalizar esta lógica usando nombres de estado más claros, por ejemplo mediante un `enum`.

---

## Relación con PLC

Este ejercicio se parece a una ejecución cooperativa por ciclos:

- `loop()` actúa como ciclo principal.
- `millis()` permite comprobar temporizaciones.
- `current_mode` funciona como estado o modo de operación.
- `detectar_pulsacion_valida()` actúa como bloque de detección de evento.

Esto prepara el camino para temporizadores tipo `TON`, `TOF`, `TP` y detectores de flanco `R_TRIG` / `F_TRIG`.

---

## Documentación relacionada

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
docs/buenas_practicas.md
docs/lecciones_aprendidas.md
arduino/05_millis_no_bloqueante/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| RGB integrado en GPIO48 | Validado |
| Pulsador externo en GPIO3 | Validado |
| Parpadeo sin `delay()` | Validado |
| Antirrebote con `millis()` | Validado |
| Cambio de modo con pulsación válida | Validado |
| Ejecución cooperativa en `loop()` | Validado |
