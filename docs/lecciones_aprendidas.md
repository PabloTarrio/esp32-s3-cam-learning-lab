# Lecciones aprendidas

Registro de aprendizajes relevantes descubiertos durante el desarrollo del repositorio **ESP32-S3-CAM Learning Lab**.

Este documento recoge errores reales, decisiones técnicas y conceptos importantes que conviene recordar para futuros ejercicios y proyectos.

---

## 1. Propósito del documento

Este archivo no es un diario cronológico completo. Para eso se usará la carpeta `journal/`.

El objetivo de `lecciones_aprendidas.md` es recopilar aprendizajes reutilizables que puedan consultarse rápidamente más adelante.

Cada lección debe intentar incluir:

- contexto,
- problema encontrado,
- causa,
- solución,
- conclusión práctica.

---

## 2. PlatformIO y entorno de trabajo

### 2.1 PlatformIO como entorno principal

**Contexto:**  
El entorno elegido para el aprendizaje es Visual Studio Code con PlatformIO.

**Lección:**  
Aunque el Arduino IDE podría usarse para ejemplos simples, en este repositorio se usará PlatformIO desde el inicio para mantener una estructura más profesional y preparada para proyectos grandes.

**Conclusión práctica:**  
Cada ejercicio Arduino será un proyecto PlatformIO independiente con esta estructura:

```text
arduino/XX_nombre_ejercicio/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

---

### 2.2 Configuración inicial de placa

**Contexto:**  
La placa usada es una ESP32-S3-CAM basada en ESP32-S3-WROOM-1 N16R8.

**Lección:**  
Para comenzar con Arduino Framework en PlatformIO se puede usar una placa genérica compatible como:

```ini
board = esp32-s3-devkitc-1
```

**Conclusión práctica:**  
La configuración inicial usada en los primeros ejercicios será similar a:

```ini
[env:esp32-s3]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200

build_flags =
    -DBOARD_HAS_PSRAM
```

La activación de PSRAM será importante para ejercicios posteriores de cámara.

---

## 3. GPIO y pinout

### 3.1 No asumir que cualquier GPIO está libre

**Contexto:**  
En una placa ESP32-S3-CAM hay muchos periféricos integrados.

**Problema:**  
Inicialmente se propuso usar `GPIO4` para un botón externo.

**Causa:**  
`GPIO4` aparece asociado a la cámara como `CAM_SIOD`.

**Solución:**  
Se decidió no usar GPIO de cámara para ejercicios generales.

**Conclusión práctica:**  
Antes de usar cualquier pin se debe consultar:

```text
hardware/pinout.md
hardware/gpio_reservados.md
```

---

### 3.2 La cámara ocupa muchos GPIO

**Contexto:**  
La cámara OV2640 usa una interfaz paralela DVP y señales de control.

**Lección:**  
Los siguientes pines deben reservarse para la cámara:

```text
GPIO4, GPIO5, GPIO6, GPIO7,
GPIO8, GPIO9, GPIO10, GPIO11,
GPIO12, GPIO13, GPIO15,
GPIO16, GPIO17, GPIO18
```

**Conclusión práctica:**  
Aunque alguno pueda funcionar si la cámara no se inicializa, no se usarán para ejercicios generales.

---

### 3.3 GPIO48 está reservado para el RGB integrado

**Contexto:**  
La placa incorpora un LED RGB integrado.

**Lección:**  
El RGB integrado está conectado a:

```cpp
#define LED_PIN 48
```

**Conclusión práctica:**  
`GPIO48` queda reservado para el NeoPixel integrado y no debe usarse como GPIO general.

---

## 4. LED RGB integrado / NeoPixel

### 4.1 El RGB integrado no es un LED normal

**Contexto:**  
Se intentó inicialmente hacer parpadear un LED usando `digitalWrite()`.

**Problema:**  
El código compilaba y el monitor serie funcionaba, pero el LED no parpadeaba.

**Causa:**  
El LED integrado no es un LED simple conectado directamente a un GPIO, sino un LED RGB direccionable tipo NeoPixel / WS2812.

**Solución:**  
Se usó la librería:

```cpp
#include <Adafruit_NeoPixel.h>
```

**Conclusión práctica:**  
No usar:

```cpp
digitalWrite(48, HIGH);
```

Usar una librería NeoPixel:

```cpp
Adafruit_NeoPixel pixel(1, 48, NEO_GRB + NEO_KHZ800);
```

---

### 4.2 Siempre llamar a `show()`

**Contexto:**  
Con NeoPixel, cambiar un color en memoria no actualiza inmediatamente el LED físico.

**Lección:**  
Después de modificar el color se debe llamar a:

```cpp
pixel.show();
```

**Conclusión práctica:**  
Este patrón debe respetarse:

```cpp
pixel.setPixelColor(0, pixel.Color(255, 0, 0));
pixel.show();
```

---

### 4.3 `setBrightness()` puede no ser suficiente por sí solo

**Contexto:**  
Se implementó control de brillo por puerto serie usando `+` y `-`.

**Problema:**  
Modificar únicamente el brillo no siempre producía el comportamiento visual esperado.

**Causa:**  
La librería NeoPixel puede requerir reenviar el color después de modificar el brillo para que el estado final quede correctamente aplicado.

**Solución:**  
Actualizar brillo, reenviar color y después llamar a `show()`:

```cpp
rgb_led.setBrightness(brillo);
rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
rgb_led.show();
```

**Conclusión práctica:**  
En este repositorio, cuando se cambie el brillo del NeoPixel, se reenviará también el color.

---

### 4.4 Usar valores de brillo bajos en pruebas

**Contexto:**  
El LED RGB integrado puede resultar muy brillante.

**Lección:**  
Aunque el rango de brillo sea `0-255`, para pruebas de escritorio conviene usar valores bajos.

**Conclusión práctica:**  
Valores como estos son cómodos para pruebas:

```cpp
pixel.setBrightness(10);
pixel.setBrightness(20);
pixel.setBrightness(30);
```

---

## 5. C++ aplicado a sistemas embebidos

### 5.1 Paso por valor frente a paso por referencia

**Contexto:**  
Se creó una función que recibía un objeto `Adafruit_NeoPixel`.

**Problema:**  
La función estaba definida así:

```cpp
void rgb_blink(Adafruit_NeoPixel rgb_led)
```

**Causa:**  
Ese código pasa el objeto por valor, creando una copia.

**Solución:**  
Pasar el objeto por referencia:

```cpp
void rgb_blink(Adafruit_NeoPixel &rgb_led)
```

**Conclusión práctica:**  
Cuando se quiera modificar un objeto existente o evitar copias innecesarias, se usará paso por referencia.

---

### 5.2 Shadowing de variables

**Contexto:**  
Se usó una variable global para almacenar el brillo actual.

**Problema:**  
Dentro de `loop()` se escribió:

```cpp
int actual_bright = update_brillo_serie(pixel, actual_bright);
```

**Causa:**  
Ese código crea una nueva variable local llamada igual que la variable global. Esto se llama *shadowing*.

**Solución:**  
No volver a declarar la variable dentro de `loop()`:

```cpp
actual_bright = update_brillo_serie(pixel, actual_bright);
```

**Conclusión práctica:**  
Prestar atención al alcance de variables globales, locales y estáticas.

---

### 5.3 Inicializar siempre las variables

**Contexto:**  
Se detectó el riesgo de usar una variable antes de darle valor.

**Lección:**  
En C/C++, una variable local no inicializada puede contener basura.

**Conclusión práctica:**  
Inicializar explícitamente:

```cpp
int actual_bright = 20;
```

---

### 5.4 Separación de responsabilidades

**Contexto:**  
Se debatió si una función encargada de actualizar el brillo debía imprimir por consola.

**Decisión:**  
Como regla general, una función debe hacer una cosa. Si una función actualiza el brillo, no debería encargarse también de imprimir mensajes, salvo que ese sea explícitamente su propósito.

**Conclusión práctica:**  
La impresión por `Serial` o `printf()` se realizará preferentemente fuera de funciones de lógica o control de hardware.

Ejemplo:

```cpp
int new_bright = update_brillo_serie(pixel, actual_bright);

if (new_bright != actual_bright)
{
    actual_bright = new_bright;
    printf("El brillo actual es: %d\n", actual_bright);
}
```

---

## 6. Puerto serie y depuración

### 6.1 El monitor serie es una herramienta de diagnóstico fundamental

**Contexto:**  
El primer código imprimía mensajes por serie.

**Lección:**  
Aunque el LED no funcionara inicialmente, el mensaje por serie permitió comprobar que el programa arrancaba y que la carga del firmware era correcta.

**Conclusión práctica:**  
En ejercicios iniciales se incluirá un mensaje de arranque:

```cpp
Serial.begin(115200);
Serial.println("Inicio OK");
```

---

### 6.2 Evitar imprimir continuamente en `loop()`

**Contexto:**  
Si se imprime en cada ciclo de `loop()`, el monitor serie se satura.

**Lección:**  
Conviene imprimir solo cuando haya cambios relevantes.

**Conclusión práctica:**  
Usar comparación de estado:

```cpp
if (new_bright != actual_bright)
{
    actual_bright = new_bright;
    printf("El brillo actual es: %d\n", actual_bright);
}
```

---

## 7. Documentación y repositorio

### 7.1 Separar documentación estable de planificación

**Contexto:**  
Algunos documentos técnicos incluían secciones de próximos pasos.

**Problema:**  
Eso obliga a actualizar muchos archivos con frecuencia.

**Decisión:**  
Los documentos técnicos deben ser estables y describir información de referencia.

**Conclusión práctica:**  
Los próximos pasos deben vivir en:

```text
ROADMAP.md
CHANGELOG.md
journal/
```

No en cada archivo técnico.

---

### 7.2 Usar `assets/` para recursos visuales

**Contexto:**  
Se planteó guardar imágenes dentro de `docs/images/`.

**Decisión:**  
Los recursos visuales se guardarán en `assets/`.

**Conclusión práctica:**  
Estructura preferida:

```text
assets/
├── images/
├── diagrams/
└── schematics/
```

---

### 7.3 Evitar tildes y espacios en nombres técnicos

**Contexto:**  
Se creó inicialmente una carpeta con nombre en castellano con tilde.

**Lección:**  
Aunque Git puede manejar muchos nombres Unicode, en repositorios técnicos conviene mantener nombres simples y portables.

**Conclusión práctica:**  
Preferir:

```text
assets/images/
```

en lugar de:

```text
docs/imágenes/
```

---

## 8. Seguridad y credenciales

### 8.1 No subir credenciales al repositorio

**Contexto:**  
Más adelante se trabajará con WiFi, MQTT y servicios externos.

**Lección:**  
Credenciales, contraseñas, tokens y claves privadas nunca deben subirse a GitHub.

**Conclusión práctica:**  
El `.gitignore` debe excluir:

```text
secrets.h
credentials.h
wifi_credentials.h
.env
```

Cuando haga falta un ejemplo, se usará un archivo tipo:

```text
secrets.example.h
```

---

## 9. Alimentación y niveles lógicos

### 9.1 Los GPIO trabajan a 3.3 V

**Contexto:**  
El ESP32-S3 no debe tratarse como un Arduino UNO de 5 V.

**Lección:**  
No se deben aplicar señales de 5 V directamente a los GPIO.

**Conclusión práctica:**  
Antes de conectar sensores o módulos externos, comprobar:

- tensión de alimentación,
- nivel lógico de salida,
- necesidad de divisor resistivo o conversor de nivel,
- GND común.

---

### 9.2 Cámara y WiFi pueden aumentar el consumo

**Contexto:**  
Los primeros ejercicios son ligeros, pero cámara y WiFi exigirán más alimentación.

**Lección:**  
Si aparecen reinicios, fallos de cámara o desconexiones WiFi, la alimentación será una de las primeras hipótesis a comprobar.

**Conclusión práctica:**  
Usar cable USB corto y de calidad, y revisar alimentación al trabajar con cámara, SD o WiFi.

---

## 10. Criterios para añadir nuevas lecciones

Añadir una nueva lección cuando ocurra alguno de estos casos:

- se detecte un error conceptual,
- se corrija una mala práctica,
- se confirme un comportamiento concreto de la placa,
- se resuelva un problema de compilación o carga,
- se descubra una limitación de hardware,
- se tome una decisión de arquitectura,
- se confirme una buena práctica reutilizable.

Formato recomendado:

```markdown
### Título de la lección

**Contexto:**  
Descripción breve.

**Problema:**  
Qué ocurrió.

**Causa:**  
Por qué ocurrió.

**Solución:**  
Cómo se resolvió.

**Conclusión práctica:**  
Regla o aprendizaje que queda para el futuro.
```

No todas las lecciones necesitan todos los apartados, pero conviene mantener esta estructura cuando sea posible.
