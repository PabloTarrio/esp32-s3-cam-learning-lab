# 001 - PlatformIO y RGB NeoPixel integrado

**Fecha:** 2026-07-03  
**Proyecto:** ESP32-S3-CAM Learning Lab  
**Fase del roadmap:** Fase 0 / Fase 1  
**Entorno:** Visual Studio Code + PlatformIO + Arduino Framework  
**Placa:** ESP32-S3-CAM basada en ESP32-S3-WROOM-1 N16R8

---

## Objetivo de la sesión

Iniciar el repositorio de aprendizaje y validar el primer contacto real con la placa ESP32-S3-CAM.

Los objetivos principales fueron:

- confirmar que PlatformIO funciona correctamente;
- compilar y subir firmware a la placa;
- abrir el monitor serie;
- identificar el LED RGB integrado;
- controlar el LED RGB mediante código Arduino;
- ajustar el brillo desde el monitor serie;
- comenzar la documentación semiprofesional del repositorio.

---

## Trabajo realizado

### Entorno y placa

- Se confirmó el uso de Visual Studio Code con PlatformIO como entorno principal.
- Se creó el primer proyecto para ESP32-S3 usando Arduino Framework.
- Se configuró la placa inicialmente como `esp32-s3-devkitc-1`.
- Se configuró el monitor serie a `115200` baudios.
- Se activó la PSRAM mediante `-DBOARD_HAS_PSRAM`.

### LED RGB integrado

- Se comprobó que el LED integrado no funciona como un LED digital simple.
- Se identificó que el LED RGB integrado es de tipo NeoPixel / WS2812.
- Se confirmó que el pin de control del RGB integrado es `GPIO48`.
- Se añadió la librería `Adafruit_NeoPixel` al proyecto.
- Se consiguió encender el LED en rojo, verde y azul.
- Se implementó control de brillo mediante comandos por puerto serie.

### Documentación del repositorio

Se inició la estructura semiprofesional del repositorio con documentos base:

- `README.md`
- `ROADMAP.md`
- `CHANGELOG.md`
- `CONTRIBUTING.md`
- `LICENSE`
- `.gitignore`

También se creó documentación inicial en:

- `hardware/`
- `docs/`
- `arduino/01_rgb_neopixel/`

---

## Código trabajado

El primer ejercicio se ubicó en:

```text
arduino/01_rgb_neopixel/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

El archivo `platformio.ini` incluye:

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

## Problemas encontrados

### 1. El LED no parpadeaba con `digitalWrite()`

Inicialmente se intentó controlar el LED como si fuera un LED digital normal.

El programa compilaba y el monitor serie imprimía correctamente, pero el LED no parpadeaba.

**Causa:**  
El LED integrado de la placa es un NeoPixel / WS2812, no un LED conectado directamente al GPIO.

**Solución:**  
Usar la librería `Adafruit_NeoPixel`.

---

### 2. Paso por valor del objeto `Adafruit_NeoPixel`

Se plantearon funciones que recibían el objeto NeoPixel por valor.

Ejemplo problemático:

```cpp
void rgb_blink(Adafruit_NeoPixel rgb_led)
```

**Causa:**  
Ese código crea una copia del objeto.

**Solución:**  
Pasar el objeto por referencia:

```cpp
void rgb_blink(Adafruit_NeoPixel &rgb_led)
```

---

### 3. Shadowing de variable

Se detectó un problema al declarar una variable local con el mismo nombre que una variable global.

Código problemático:

```cpp
int actual_bright = update_brillo_serie(pixel, actual_bright);
```

**Causa:**  
Se estaba creando una nueva variable local llamada igual que la global.

**Solución:**  
Actualizar la variable global sin volver a declararla:

```cpp
actual_bright = update_brillo_serie(pixel, actual_bright);
```

---

### 4. Cambio de brillo del NeoPixel

Se observó que modificar únicamente el brillo no siempre producía el comportamiento esperado.

Código menos fiable:

```cpp
rgb_led.setBrightness(brillo);
rgb_led.show();
```

**Solución aplicada:**

```cpp
rgb_led.setBrightness(brillo);
rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
rgb_led.show();
```

---

### 5. Codificación UTF-8

Se detectaron caracteres mal codificados en comentarios del archivo `main.cpp`.

**Solución:**  
Guardar el archivo en codificación UTF-8.

---

## Decisiones tomadas

- El entorno principal será PlatformIO, no Arduino IDE.
- Cada ejercicio Arduino será un proyecto PlatformIO independiente.
- La explicación detallada del código irá preferentemente en comentarios dentro de `src/main.cpp`.
- El `README.md` de cada ejercicio será una guía breve de uso y contexto.
- Los documentos técnicos no incluirán secciones de planificación diaria.
- Los avances y próximos pasos se registrarán en `ROADMAP.md`, `CHANGELOG.md` y `journal/`.
- Las imágenes y esquemas se guardarán en `assets/`.
- No se usará ningún GPIO sin consultar antes la documentación de hardware.

---

## Conceptos aprendidos

- Diferencia entre LED digital simple y NeoPixel / WS2812.
- Uso básico de `Adafruit_NeoPixel`.
- Necesidad de llamar a `show()` para aplicar cambios en NeoPixel.
- Control de brillo mediante `setBrightness()`.
- Lectura de caracteres desde el monitor serie.
- Variables globales y locales.
- Shadowing de variables.
- Paso por referencia en C++.
- Importancia de estructurar el repositorio desde el inicio.
- Importancia de documentar errores reales.

---

## Archivos relacionados

```text
README.md
ROADMAP.md
CHANGELOG.md
CONTRIBUTING.md
LICENSE
.gitignore

docs/lecciones_aprendidas.md
docs/buenas_practicas.md
docs/glosario.md

hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md

arduino/01_rgb_neopixel/README.md
arduino/01_rgb_neopixel/platformio.ini
arduino/01_rgb_neopixel/src/main.cpp
```

---

## Estado de la sesión

| Elemento | Estado |
|---|---|
| PlatformIO funcionando | Validado |
| Firmware subido a placa | Validado |
| Monitor serie | Validado |
| RGB integrado en GPIO48 | Validado |
| Control con `Adafruit_NeoPixel` | Validado |
| Control de brillo por serie | Validado |
| Documentación base del repositorio | En progreso avanzado |
| Primer ejercicio documentado | Validado |

---

## Próximas acciones registradas

- Crear el repositorio remoto en GitHub.
- Inicializar Git en la carpeta local si todavía no se ha hecho.
- Revisar `git status`.
- Hacer el primer commit del repositorio.
- Continuar con `arduino/02_boton_gpio/`.
