# Ejercicio 01 - RGB NeoPixel integrado

Primer ejercicio práctico del repositorio **ESP32-S3-CAM Learning Lab**.

Este ejercicio valida el entorno de desarrollo y controla el LED RGB integrado de la placa ESP32-S3-CAM.

---

## Objetivo

Controlar el LED RGB integrado de la placa ESP32-S3-CAM usando:

- PlatformIO,
- Arduino Framework,
- librería `Adafruit_NeoPixel`,
- monitor serie.

Este ejercicio permite comprobar:

- compilación con PlatformIO,
- subida de firmware a la placa,
- funcionamiento del monitor serie,
- control del RGB integrado en `GPIO48`,
- control básico de brillo mediante comandos serie.

---

## Hardware utilizado

| Elemento | Descripción |
|---|---|
| Placa | ESP32-S3-CAM |
| Módulo | ESP32-S3-WROOM-1 N16R8 |
| LED integrado | RGB NeoPixel / WS2812 |
| GPIO RGB | GPIO48 |
| Conexión al PC | USB |
| Alimentación | USB |

No requiere cableado externo.

---

## Software utilizado

- Visual Studio Code
- PlatformIO
- Arduino Framework
- Librería `Adafruit_NeoPixel`

---

## Estructura del ejercicio

```text
arduino/01_rgb_neopixel/
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

Contenido recomendado:

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

Notas:

- `board = esp32-s3-devkitc-1` se usa como placa compatible para comenzar.
- `monitor_speed = 115200` configura la velocidad del monitor serie.
- `-DBOARD_HAS_PSRAM` activa PSRAM para mantener una configuración compatible con futuros ejercicios de cámara.
- `adafruit/Adafruit NeoPixel` permite controlar el LED RGB integrado.

---

## Funcionamiento

El programa:

1. Inicializa el monitor serie.
2. Inicializa el LED RGB integrado.
3. Enciende el LED con un brillo inicial.
4. Permite modificar el brillo desde el monitor serie.

Comandos disponibles:

```text
+ -> aumenta el brillo
- -> reduce el brillo
```

El brillo se mantiene dentro del rango:

```text
0 - 255
```

---

## Resultado esperado

Al arrancar el programa debe aparecer en el monitor serie:

```text
RGB_listo
```

Al enviar `+` o `-`, el brillo del LED cambia y se imprime el nuevo valor:

```text
El brillo actual es: 21
```

El LED RGB integrado debe cambiar visualmente su intensidad.

---

## Problemas encontrados

### El LED no funcionaba con `digitalWrite()`

El RGB integrado no es un LED simple, sino un NeoPixel / WS2812.

Por tanto, no se controla correctamente con:

```cpp
digitalWrite(LED_PIN, HIGH);
```

Se debe usar una librería compatible con NeoPixel.

---

### Paso por valor de `Adafruit_NeoPixel`

El objeto `Adafruit_NeoPixel` no debe pasarse por valor si se quiere modificar el objeto real.

Forma incorrecta:

```cpp
void rgb_blink(Adafruit_NeoPixel rgb_led)
```

Forma correcta:

```cpp
void rgb_blink(Adafruit_NeoPixel &rgb_led)
```

---

### Shadowing de variable

Se debe evitar declarar una variable local con el mismo nombre que una variable global.

Forma problemática:

```cpp
int actual_bright = update_brillo_serie(pixel, actual_bright);
```

Forma correcta:

```cpp
actual_bright = update_brillo_serie(pixel, actual_bright);
```

---

### Cambio de brillo

Para asegurar que el cambio de brillo se refleja correctamente, se reenvía también el color antes de llamar a `show()`.

Patrón usado:

```cpp
rgb_led.setBrightness(brillo);
rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
rgb_led.show();
```

---

## Documentación relacionada

```text
hardware/rgb_neopixel.md
hardware/pinout.md
hardware/gpio_reservados.md
docs/lecciones_aprendidas.md
docs/buenas_practicas.md
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Compilación | Validado |
| Subida a placa | Validado |
| Monitor serie | Validado |
| RGB en GPIO48 | Validado |
| Cambio de color | Validado |
| Control de brillo por serie | Validado |
| Documentación inicial | Validada |

---

## Mejoras futuras

- Sustituir `delay()` por temporización no bloqueante con `millis()`.
- Separar lectura serie y aplicación del color en funciones más específicas.
- Crear una pequeña máquina de estados para colores.
- Usar el RGB como indicador de estado en ejercicios posteriores.
- Repetir el ejercicio en ESP-IDF cuando se llegue a esa fase.
