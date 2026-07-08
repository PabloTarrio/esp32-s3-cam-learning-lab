# Ejercicio 04 - Interrupciones

Ejercicio de la **Fase 3 - Interrupciones** del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio introduce el uso de interrupciones externas en el ESP32-S3 usando un pulsador externo conectado a `GPIO3`. El objetivo es entender cómo detectar eventos hardware mediante `attachInterrupt()`, cómo comunicar una interrupción con el flujo principal del programa y cómo aplicar antirrebote sin ejecutar lógica pesada dentro de la ISR.

---

## Objetivo

Entender eventos hardware, ISR y limitaciones de las interrupciones.

En concreto, este ejercicio trabaja:

- interrupción externa sobre un GPIO;
- detección de flanco descendente con `FALLING`;
- uso de `volatile` para variables modificadas desde una ISR;
- comunicación entre ISR y `loop()` mediante una bandera;
- antirrebote por temporización usando `millis()`;
- validación posterior del estado estable del pin;
- cambio de color del LED RGB integrado cuando la interrupción es válida;
- diferencia entre código seguro y no seguro dentro de una ISR.

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
arduino/04_interrupciones/
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

Por tanto:

```text
HIGH -> LOW = flanco descendente = pulsar
LOW  -> HIGH = flanco ascendente  = liberar
```

En este ejercicio se usa interrupción por **flanco descendente**:

```cpp
attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    on_button_interrupt,
    FALLING
);
```

Con `INPUT_PULLUP`, `FALLING` detecta el momento de la pulsación.

---

## Pines utilizados

| Función | GPIO | Estado |
|---|---:|---|
| Pulsador externo | GPIO3 | Validado |
| RGB integrado | GPIO48 | Validado |

---

## Versiones trabajadas

### Versión 1 - Interrupción básica

Primera versión del ejercicio.

Objetivo:

- configurar `attachInterrupt()`;
- detectar una interrupción en `GPIO3`;
- marcar una bandera desde la ISR;
- procesar el evento en `loop()`.

Resultado observado:

- la interrupción se dispara correctamente al pulsar;
- cada pulsación puede generar varias interrupciones debido al rebote mecánico.

Conclusión:

> Las interrupciones no eliminan el rebote del pulsador.

---

### Versión 2 - Interrupción con antirrebote

Segunda versión del ejercicio.

Objetivo:

- mantener la ISR mínima;
- filtrar rebotes en `loop()`;
- usar `millis()` como temporizador de antirrebote;
- contar interrupciones válidas e interrupciones ignoradas.

Resultado observado:

- se reducen los eventos falsos provocados por rebote;
- el programa distingue entre interrupciones válidas y rebotes ignorados.

---

### Versión 3 - Interrupción válida con cambio de RGB

Versión final del ejercicio.

Objetivo:

- mantener la interrupción como señal de evento;
- validar el evento en `loop()`;
- cambiar el color del RGB integrado solo si la interrupción es válida.

Flujo de trabajo:

```text
Pulsador GPIO3
↓
Interrupción FALLING
↓
ISR marca bandera
↓
loop() espera tiempo de debounce
↓
loop() valida estado estable del pin
↓
si el evento es válido, cambia el color del RGB
```

---

## Funcionamiento

El programa realiza los siguientes pasos:

1. Inicializa el monitor serie.
2. Configura `GPIO3` como entrada con `INPUT_PULLUP`.
3. Configura una interrupción externa en `GPIO3` usando `FALLING`.
4. Inicializa el LED RGB integrado en `GPIO48`.
5. Cuando se pulsa el botón, la ISR marca una bandera.
6. `loop()` detecta que hay una interrupción pendiente.
7. `loop()` espera `DEBOUNCE_TIME_MS`.
8. Pasado ese tiempo, `loop()` lee de nuevo el pin.
9. Si el pin sigue en `LOW`, la pulsación se considera válida.
10. El programa incrementa el contador de interrupciones válidas.
11. El programa cambia el color del RGB integrado.
12. Si el pin no está en el estado esperado, el evento se considera rebote.

---

## Resultado esperado

Al arrancar, el monitor serie muestra información similar a:

```text
Ejercicio 04 - Interrupciones
BUTTON PIN : GPIO 3
LED RGB INTEGRADO : GPIO 48
Interrupción configurada en flanco descendente
```

Cuando se detecta una interrupción válida:

```text
Interrupción válida. Contador: 1
```

Si se detecta un rebote ignorado:

```text
Rebote ignorado. Total ignorados: 1
```

El RGB integrado cambia de color solo cuando la interrupción se considera válida.

---

## Flanco descendente y flanco ascendente

En este ejercicio se utiliza `FALLING`, porque el pulsador está configurado con `INPUT_PULLUP`.

Con esta configuración:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
Liberado   -> HIGH
```

Por tanto:

```text
FALLING = HIGH -> LOW = pulsar
RISING  = LOW -> HIGH = liberar
```

No se implementa una versión práctica separada con `RISING`, porque el concepto queda cubierto: bastaría con cambiar el modo de interrupción a `RISING` y validar que el estado estable esperado después del antirrebote sea `HIGH`.

---

## ISR

La ISR usada en el ejercicio es mínima:

```cpp
void IRAM_ATTR on_button_interrupt()
{
    button_interrupt_pending = true;
    interrupt_time = millis();
}
```

La ISR solo marca que ha ocurrido una interrupción y guarda el instante de detección.

Dentro de la ISR no se debe hacer:

- `Serial.print()`;
- `delay()`;
- control del RGB;
- lógica pesada;
- operaciones largas o bloqueantes.

El trabajo importante se hace en `loop()`.

---

## Uso de `volatile`

La bandera de interrupción se declara como `volatile`:

```cpp
volatile bool button_interrupt_pending = false;
volatile unsigned long interrupt_time = 0;
```

Se usa `volatile` porque estas variables pueden cambiar fuera del flujo normal del programa, concretamente dentro de la ISR.

---

## Antirrebote con interrupciones

El antirrebote no se resuelve dentro de la ISR.

El patrón usado es:

```text
ISR -> marcar evento pendiente
loop() -> esperar debounce
loop() -> validar estado del pin
loop() -> aceptar o descartar evento
```

Para `INPUT_PULLUP` y `FALLING`, una pulsación válida debe dejar el pin en `LOW` después del tiempo de antirrebote.

```cpp
if (digitalRead(BUTTON_PIN) == LOW)
{
    // interrupción válida
}
```

---

## Error importante encontrado

Durante el desarrollo se detectó un error en la lógica de antirrebote.

### Problema

Inicialmente se limpiaba la bandera `button_interrupt_pending` demasiado pronto, antes de que pasara el tiempo de antirrebote.

Eso hacía que el evento se perdiera antes de poder validarse.

### Causa

El programa hacía algo equivalente a:

```text
1. ISR marca evento pendiente.
2. loop() detecta la bandera.
3. loop() borra la bandera inmediatamente.
4. todavía no ha pasado DEBOUNCE_TIME_MS.
5. no se valida nada.
6. el evento queda perdido.
```

### Solución

La bandera no debe limpiarse hasta que el evento haya terminado el proceso de validación.

Flujo corregido:

```text
1. ISR marca evento pendiente.
2. loop() observa que hay evento pendiente.
3. si no ha pasado DEBOUNCE_TIME_MS, loop() espera.
4. la bandera sigue activa.
5. cuando pasa el tiempo, loop() valida el pin.
6. loop() acepta o descarta el evento.
7. solo entonces se limpia la bandera.
```

### Conclusión práctica

> No se debe borrar una bandera de interrupción pendiente antes de completar la validación del evento.

---

## Comparación: polling frente a interrupción

### Polling

En polling, el programa lee el estado del pin continuamente desde `loop()`.

Ventajas:

- lógica sencilla;
- depuración más fácil;
- antirrebote más intuitivo;
- suficiente para muchos botones.

Inconvenientes:

- si `loop()` se bloquea, se pueden perder eventos;
- el programa debe consultar la entrada constantemente;
- puede complicarse cuando hay muchas entradas o tareas.

---

### Interrupciones

Con interrupciones, el hardware avisa al programa cuando ocurre un evento.

Ventajas:

- reacción rápida;
- útil para eventos breves;
- no depende de leer constantemente el pin;
- permite que `loop()` esté dedicado a otras tareas.

Inconvenientes:

- no eliminan el rebote mecánico;
- requieren ISR mínima;
- exigen cuidado con variables compartidas;
- suelen necesitar `volatile`;
- pueden complicar la lógica si se usan sin criterio.

---

## Conceptos trabajados

- `attachInterrupt()`.
- `digitalPinToInterrupt()`.
- ISR.
- `IRAM_ATTR`.
- `volatile`.
- Flanco descendente `FALLING`.
- Flanco ascendente `RISING` a nivel conceptual.
- `INPUT_PULLUP`.
- Antirrebote por temporización.
- Comunicación ISR -> `loop()`.
- Validación posterior del estado del pin.
- Código seguro dentro de una ISR.
- Qué no hacer dentro de una ISR.
- Control de RGB como acción posterior a un evento válido.

---

## Documentación relacionada

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
docs/lecciones_aprendidas.md
docs/buenas_practicas.md
arduino/03_boton_rgb_estado/
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Interrupción por flanco descendente | Validado |
| Interrupción por flanco ascendente | Cubierto conceptualmente |
| Uso de `volatile` | Validado |
| Comunicación ISR -> `loop()` | Validado |
| Antirrebote básico con interrupciones | Validado |
| Antirrebote mediante temporización | Validado |
| Cambio de RGB con interrupción válida | Validado |
| Comparación polling vs interrupción | Documentada |
