# LED RGB integrado - NeoPixel / WS2812

Documento de referencia sobre el LED RGB integrado de la placa **ESP32-S3-CAM** utilizada en este repositorio.

Este LED se utilizará en los primeros ejercicios como salida visual de estado, depuración básica y práctica de control de periféricos.

---

## 1. Resumen

| Elemento | Valor |
|---|---|
| Tipo de LED | RGB direccionable |
| Familia habitual | NeoPixel / WS2812 |
| GPIO usado | GPIO48 |
| Número de LEDs | 1 |
| Tensión lógica | 3.3 V desde el ESP32-S3 |
| Librería usada inicialmente | `Adafruit_NeoPixel` |
| Framework | Arduino Framework sobre PlatformIO |

---

## 2. Diferencia frente a un LED normal

El LED RGB integrado **no es un LED digital simple**.

Un LED normal podría controlarse con:

```cpp
digitalWrite(LED_PIN, HIGH);
digitalWrite(LED_PIN, LOW);
```

Sin embargo, el RGB integrado de esta placa es un LED direccionable. Esto significa que no se controla con un simple nivel lógico `HIGH` o `LOW`, sino enviando una secuencia de datos con información de color.

Por tanto, para controlarlo se utiliza una librería específica, por ejemplo:

```cpp
#include <Adafruit_NeoPixel.h>
```

---

## 3. GPIO utilizado

El LED RGB integrado está conectado a:

```cpp
#define LED_PIN 48
```

Este GPIO queda reservado para el RGB integrado y no debe usarse como GPIO general en otros ejercicios.

---

## 4. Configuración mínima en PlatformIO

En el archivo `platformio.ini` del ejercicio se debe incluir la librería:

```ini
lib_deps =
    adafruit/Adafruit NeoPixel
```

Ejemplo de configuración básica:

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

## 5. Código mínimo de inicialización

```cpp
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 48
#define NUM_LEDS 1

Adafruit_NeoPixel pixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
    Serial.begin(115200);

    pixel.begin();
    pixel.clear();
    pixel.show();

    Serial.println("RGB listo");
}

void loop()
{
    pixel.setPixelColor(0, pixel.Color(255, 0, 0));
    pixel.show();
    delay(500);

    pixel.setPixelColor(0, pixel.Color(0, 255, 0));
    pixel.show();
    delay(500);

    pixel.setPixelColor(0, pixel.Color(0, 0, 255));
    pixel.show();
    delay(500);
}
```

---

## 6. Control de color

La función:

```cpp
pixel.Color(red, green, blue)
```

recibe tres valores entre `0` y `255`:

| Canal | Rango | Significado |
|---|---:|---|
| Red | 0-255 | Intensidad del rojo |
| Green | 0-255 | Intensidad del verde |
| Blue | 0-255 | Intensidad del azul |

Ejemplos:

```cpp
pixel.setPixelColor(0, pixel.Color(255, 0, 0));   // Rojo
pixel.setPixelColor(0, pixel.Color(0, 255, 0));   // Verde
pixel.setPixelColor(0, pixel.Color(0, 0, 255));   // Azul
pixel.setPixelColor(0, pixel.Color(255, 255, 255)); // Blanco
pixel.setPixelColor(0, pixel.Color(0, 0, 0));     // Apagado
pixel.show();
```

---

## 7. Control de brillo

La librería permite controlar el brillo global con:

```cpp
pixel.setBrightness(brillo);
```

El rango habitual es:

```text
0 - 255
```

Ejemplo:

```cpp
pixel.setBrightness(20);
pixel.setPixelColor(0, pixel.Color(255, 0, 0));
pixel.show();
```

Para los ejercicios iniciales se recomienda usar valores bajos, por ejemplo `10`, `20` o `30`, porque el LED RGB integrado puede resultar muy brillante.

---

## 8. Comportamiento observado con `setBrightness()`

Durante las primeras pruebas se observó que modificar únicamente el brillo no siempre produce el efecto esperado si no se vuelve a escribir el color.

Código menos fiable:

```cpp
pixel.setBrightness(brillo);
pixel.show();
```

Código recomendado:

```cpp
pixel.setBrightness(brillo);
pixel.setPixelColor(0, pixel.Color(255, 0, 0));
pixel.show();
```

Regla práctica usada en este repositorio:

> Cuando se cambie el brillo del NeoPixel, se reenviará también el color antes de llamar a `show()`.

---

## 9. Control por puerto serie

En el primer ejercicio se implementó un control simple de brillo mediante el monitor serie:

- `+` aumenta el brillo.
- `-` reduce el brillo.

Fragmento de referencia:

```cpp
int update_brillo_serie(Adafruit_NeoPixel &rgb_led, int brillo)
{
    bool pulsacion_detectada = false;

    if (Serial.available() > 0)
    {
        char tecla_pulsada = Serial.read();

        if (tecla_pulsada == '+' && brillo < 255)
        {
            brillo++;
            pulsacion_detectada = true;
        }
        else if (tecla_pulsada == '-' && brillo > 0)
        {
            brillo--;
            pulsacion_detectada = true;
        }
    }

    if (pulsacion_detectada)
    {
        rgb_led.setBrightness(brillo);
        rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
        rgb_led.show();
    }

    return brillo;
}
```

---

## 10. Conceptos de C++ trabajados con este periférico

El ejercicio del RGB integrado permitió trabajar conceptos importantes de C++:

### Paso por referencia

El objeto `Adafruit_NeoPixel` debe pasarse por referencia si se quiere modificar el objeto original:

```cpp
void funcion(Adafruit_NeoPixel &rgb_led)
```

No se recomienda pasarlo por valor:

```cpp
void funcion(Adafruit_NeoPixel rgb_led)
```

Pasarlo por valor crea una copia del objeto, lo que no es deseable en este contexto.

---

### Shadowing de variables

Se detectó un error típico:

```cpp
int actual_bright = update_brillo_serie(pixel, actual_bright);
```

Ese código crea una variable local llamada igual que la variable global, ocultando la variable original.

La forma corregida es:

```cpp
actual_bright = update_brillo_serie(pixel, actual_bright);
```

---

### Separación de responsabilidades

Se decidió que las funciones de control del brillo no impriman directamente por serie.

La lógica adoptada fue:

- la función actualiza el brillo y el LED,
- el `loop()` decide si imprime el nuevo valor.

Ejemplo:

```cpp
void loop()
{
    int new_bright = update_brillo_serie(pixel, actual_bright);

    if (new_bright != actual_bright)
    {
        actual_bright = new_bright;
        printf("El brillo actual es: %d\n", actual_bright);
    }
}
```

---

## 11. Errores frecuentes

### Intentar usar `digitalWrite()`

Incorrecto:

```cpp
digitalWrite(48, HIGH);
```

Motivo:

- El RGB integrado no es un LED simple.
- Es un LED direccionable que requiere protocolo de datos.

---

### Olvidar `show()`

Incorrecto:

```cpp
pixel.setPixelColor(0, pixel.Color(255, 0, 0));
```

Correcto:

```cpp
pixel.setPixelColor(0, pixel.Color(255, 0, 0));
pixel.show();
```

La llamada a `show()` envía los datos al LED.

---

### Usar brillo excesivo

Valores como `255` pueden ser demasiado brillantes para pruebas de escritorio.

Recomendación:

```cpp
pixel.setBrightness(20);
```

---

### No limitar el rango de brillo

El brillo debe mantenerse entre `0` y `255`.

Ejemplo de control:

```cpp
if (brillo < 255)
{
    brillo++;
}

if (brillo > 0)
{
    brillo--;
}
```

---

## 12. Uso recomendado en ejercicios futuros

El LED RGB integrado se utilizará como indicador visual en futuros ejercicios:

| Color | Uso posible |
|---|---|
| Rojo | Error, desconectado, fallo |
| Verde | Correcto, conectado, operación válida |
| Azul | Espera, inicialización, modo configuración |
| Amarillo | Advertencia |
| Blanco | Prueba general |
| Apagado | Inactivo |

Estos significados podrán variar según el ejercicio, pero conviene mantener una semántica coherente cuando sea posible.

---

## 13. Estado de validación

| Elemento | Estado |
|---|---|
| GPIO48 como salida para NeoPixel | Validado |
| Encendido rojo, verde y azul | Validado |
| Control de brillo con `setBrightness()` | Validado |
| Control por puerto serie con `+` y `-` | Validado |
| Uso de `Adafruit_NeoPixel` | Validado |

---

## 14. Relación con ejercicios

Este documento sirve como referencia para:

```text
arduino/01_rgb_neopixel/
```

También servirá como apoyo para ejercicios posteriores donde el LED RGB se use como indicador de estado.
