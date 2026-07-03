# CONTRIBUTING

Gracias por visitar este repositorio. Aunque este proyecto nace como un repositorio personal de aprendizaje, se estructura con normas semiprofesionales para mantener el código, la documentación y los ejercicios organizados y reutilizables.

Este documento define las reglas básicas para contribuir, documentar ejercicios y mantener una estructura coherente durante el aprendizaje del ESP32-S3-CAM.

---

## 1. Propósito del repositorio

Este repositorio tiene como objetivo aprender de forma progresiva el uso de una placa ESP32-S3-CAM con:

- PlatformIO
- Arduino Framework
- ESP-IDF
- Comunicaciones locales
- WiFi
- HTTP
- MQTT
- Cámara OV2640
- FreeRTOS
- Proyectos IoT

El enfoque principal es didáctico: se prioriza la claridad, la progresión paso a paso y la documentación de errores y decisiones.

---

## 2. Principios generales

### 2.1 Aprendizaje paso a paso

Cada ejercicio debe introducir pocos conceptos nuevos. No se deben mezclar demasiados temas en un único ejemplo.

Correcto:

```text
01_rgb_neopixel        -> LED RGB integrado
02_boton_gpio          -> entrada digital
03_interrupciones      -> interrupciones
04_millis_no_bloqueante -> temporización no bloqueante
```

Incorrecto:

```text
01_rgb_wifi_mqtt_camera_freertos
```

---

### 2.2 Claridad antes que optimización

En los primeros ejercicios se priorizará que el código sea fácil de leer y entender.

La optimización, abstracción y refactorización se introducirán progresivamente.

---

### 2.3 Documentar errores reales

Los errores encontrados durante el aprendizaje son parte importante del repositorio.

Deben documentarse especialmente:

- errores de compilación,
- errores de carga,
- GPIO incorrectos,
- conflictos con cámara o periféricos,
- problemas de alimentación,
- errores de C/C++,
- problemas de red,
- comportamientos inesperados de librerías.

---

## 3. Estructura de carpetas

La estructura general del repositorio será:

```text
esp32-s3-cam-learning-lab/
│
├── README.md
├── ROADMAP.md
├── CHANGELOG.md
├── CONTRIBUTING.md
├── LICENSE
├── .gitignore
│
├── docs/
├── hardware/
├── arduino/
├── esp-idf/
├── proyectos/
├── journal/
└── assets/
```

---

## 4. Reglas para ejercicios Arduino

Cada ejercicio Arduino debe ser un proyecto PlatformIO independiente:

```text
arduino/XX_nombre_ejercicio/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

Ejemplo:

```text
arduino/01_rgb_neopixel/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

---

## 5. Reglas para ejercicios ESP-IDF

Los ejercicios ESP-IDF deben ir en la carpeta:

```text
esp-idf/
```

No deben mezclarse proyectos Arduino y ESP-IDF dentro de la misma carpeta.

Ejemplo:

```text
esp-idf/01_hello_world/
esp-idf/02_gpio/
esp-idf/03_freertos_tasks/
```

---

## 6. Reglas para documentación de ejercicios

Cada ejercicio debe incluir un `README.md` con, al menos, las siguientes secciones:

```markdown
# Ejercicio XX - Nombre del ejercicio

## Objetivo

## Hardware utilizado

## Conceptos trabajados

## Funcionamiento

## Cableado

## Código principal

## Resultado esperado

## Problemas encontrados

## Mejoras futuras
```

Si alguna sección no aplica, puede indicarse explícitamente:

```markdown
## Cableado

No requiere cableado externo. Se utiliza el LED RGB integrado de la placa.
```

---

## 7. Reglas de estilo de código

### 7.1 Nombres

Usar nombres claros y descriptivos.

Correcto:

```cpp
int brillo_actual = 20;
int nuevo_brillo = 0;
```

Menos recomendable:

```cpp
int b = 20;
int x = 0;
```

---

### 7.2 Funciones

Las funciones deben tender a tener una única responsabilidad.

Correcto:

```cpp
int actualizar_brillo_desde_serie(int brillo_actual);
void aplicar_brillo_rgb(Adafruit_NeoPixel &rgb_led, int brillo);
```

Menos recomendable:

```cpp
void hacer_todo();
```

---

### 7.3 Comentarios

Los comentarios deben explicar la intención del código, no repetir obviedades.

Correcto:

```cpp
// El pulsador se conecta a GND, por eso usamos INPUT_PULLUP.
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

Menos útil:

```cpp
// Configura el pin como entrada pull-up.
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

---

### 7.4 Código pedagógico

En los primeros ejercicios se permite usar código más explícito aunque no sea el más compacto.

El objetivo inicial es aprender, no escribir el firmware más corto posible.

---

## 8. Reglas sobre hardware y GPIO

Antes de usar un GPIO se debe comprobar la documentación de la placa.

Especialmente deben revisarse:

- GPIO usados por la cámara OV2640,
- GPIO usados por PSRAM o Flash,
- GPIO usados por SD,
- GPIO del LED RGB integrado,
- GPIO de botones integrados,
- pines de arranque o boot,
- pines USB,
- pines UART.

No se debe asumir que un pin está libre solo porque exista en el ESP32-S3.

---

## 9. Reglas para recursos gráficos

Los recursos visuales deben guardarse en `assets/`.

```text
assets/
├── images/
├── diagrams/
└── schematics/
```

Uso recomendado:

- `assets/images/` para fotos y capturas.
- `assets/diagrams/` para diagramas conceptuales.
- `assets/schematics/` para esquemas eléctricos y cableados.

Evitar nombres con espacios, tildes o caracteres especiales.

Correcto:

```text
assets/images/esp32_s3_cam_board.jpg
assets/schematics/button_gpio_connection.png
```

Menos recomendable:

```text
assets/imágenes/foto placa final.jpg
```

---

## 10. Credenciales y datos sensibles

No se deben subir credenciales al repositorio.

Archivos como los siguientes deben quedar excluidos por `.gitignore`:

```text
secrets.h
credentials.h
wifi_credentials.h
.env
```

Cuando un ejercicio necesite credenciales, se debe crear un archivo de ejemplo:

```text
secrets.example.h
```

Ejemplo:

```cpp
#pragma once

#define WIFI_SSID "TU_SSID"
#define WIFI_PASSWORD "TU_PASSWORD"
#define MQTT_SERVER "broker.example.com"
```

El archivo real con credenciales privadas no debe subirse al repositorio.

---

## 11. Commits recomendados

Los commits deben ser pequeños y descriptivos.

Ejemplos:

```text
Initial repository structure
Add roadmap document
Add RGB NeoPixel exercise
Document GPIO reserved pins
Add button GPIO exercise
Fix NeoPixel brightness handling
```

Evitar mensajes poco descriptivos:

```text
changes
update
stuff
fix
```

---

## 12. CHANGELOG

Los cambios relevantes deben registrarse en `CHANGELOG.md`.

Ejemplos de cambios que deben documentarse:

- creación de nuevos ejercicios,
- cambios en estructura del repositorio,
- incorporación de documentación de hardware,
- correcciones importantes,
- nuevos proyectos,
- cambios relevantes en el roadmap.

---

## 13. Journal de aprendizaje

La carpeta `journal/` se usará para registrar sesiones de aprendizaje.

Cada entrada puede incluir:

- fecha,
- objetivo de la sesión,
- conceptos aprendidos,
- errores encontrados,
- soluciones aplicadas,
- dudas pendientes,
- próximos pasos.

Ejemplo:

```text
journal/001_platformio_rgb.md
journal/002_control_brillo_serial.md
journal/003_boton_gpio.md
```

---

## 14. Criterios para considerar terminado un ejercicio

Un ejercicio se considera terminado cuando cumple:

- [ ] Compila correctamente.
- [ ] Ha sido probado en la placa.
- [ ] Tiene un `README.md` propio.
- [ ] Explica el objetivo.
- [ ] Indica el hardware usado.
- [ ] Incluye resultado esperado.
- [ ] Documenta problemas encontrados.
- [ ] Propone mejoras futuras si procede.

---

## 15. Flujo de trabajo recomendado

Para cada nuevo ejercicio:

1. Crear carpeta del ejercicio.
2. Crear proyecto PlatformIO.
3. Escribir versión mínima funcional.
4. Probar en placa.
5. Corregir errores.
6. Comentar pedagógicamente el código.
7. Crear o completar el `README.md` del ejercicio.
8. Registrar lecciones aprendidas si procede.
9. Actualizar `CHANGELOG.md`.
10. Hacer commit.

---

## 16. Filosofía del proyecto

Este repositorio debe servir como:

- cuaderno de aprendizaje,
- colección de ejemplos reutilizables,
- base de consulta futura,
- entrenamiento progresivo hacia proyectos IoT más complejos,
- documentación de una placa ESP32-S3-CAM concreta.

La prioridad es aprender bien, entender los errores y construir una base sólida antes de avanzar hacia proyectos más complejos.
