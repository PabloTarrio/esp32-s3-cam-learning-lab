# ESP32-S3-CAM Learning Lab

Repositorio personal de aprendizaje, documentación y experimentación con una placa **ESP32-S3-CAM** basada en el módulo **ESP32-S3-WROOM-1 N16R8**, con **16 MB de Flash**, **8 MB de PSRAM**, cámara **OV2640** y LED RGB integrado tipo **NeoPixel / WS2812**.

El objetivo es aprender de forma progresiva el ecosistema ESP32, comenzando con **Arduino Framework sobre PlatformIO** y avanzando posteriormente hacia **ESP-IDF**, comunicaciones, IoT, cámara, FreeRTOS y proyectos integradores.

---

## Hardware principal

- **Placa:** ESP32-S3-CAM
- **Módulo:** ESP32-S3-WROOM-1 N16R8
- **Flash:** 16 MB
- **PSRAM:** 8 MB
- **Cámara:** OV2640
- **LED RGB integrado:** NeoPixel / WS2812 en GPIO48
- **Entorno principal:** Visual Studio Code + PlatformIO

> Esta placa no debe tratarse como un ESP32-S3 genérico. La cámara, la memoria, la SD, el LED RGB, los botones y otros periféricos pueden ocupar GPIO concretos. Antes de usar un pin, se debe consultar la documentación de `hardware/`.

---

## Objetivos

- Aprender ESP32-S3-CAM paso a paso.
- Usar PlatformIO como entorno principal.
- Empezar con Arduino Framework.
- Avanzar posteriormente hacia ESP-IDF.
- Documentar hardware, ejercicios y decisiones técnicas.
- Practicar comunicaciones locales y de red.
- Trabajar con WiFi, HTTP, MQTT, cámara, almacenamiento y FreeRTOS.
- Construir proyectos IoT completos y documentados.

---

## Estructura del repositorio

```text
esp32-s3-cam-learning-lab/
├── README.md
├── ROADMAP.md
├── CHANGELOG.md
├── CONTRIBUTING.md
├── LICENSE
├── .gitignore
│
├── docs/
│   ├── glosario.md
│   ├── lecciones_aprendidas.md
│   └── buenas_practicas.md
│
├── hardware/
│   ├── README.md
│   ├── pinout.md
│   ├── gpio_reservados.md
│   ├── rgb_neopixel.md
│   ├── camara_ov2640.md
│   ├── alimentacion.md
│   └── SD.md
│
├── arduino/
│   ├── 01_rgb_neopixel/
│   ├── 02_boton_gpio/
│   └── 03_boton_gpio_estado/
│
├── esp-idf/
│   └── README.md
│
├── proyectos/
│   └── README.md
│
├── journal/
└── assets/
    ├── images/
    ├── diagrams/
    └── schematics/
```

---

## Carpetas principales

### `docs/`

Documentación general del aprendizaje:

- glosario,
- buenas prácticas,
- lecciones aprendidas.

### `hardware/`

Documentación específica de la placa:

- pinout,
- GPIO reservados,
- LED RGB integrado,
- cámara OV2640,
- alimentación,
- tarjeta SD.

### `arduino/`

Ejercicios y ejemplos usando Arduino Framework sobre ESP32.

Cada ejercicio debe ser un proyecto PlatformIO independiente:

```text
arduino/XX_nombre_ejercicio/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

### `esp-idf/`

Ejercicios equivalentes o avanzados usando ESP-IDF.

### `proyectos/`

Proyectos integradores que combinan varios conceptos: cámara, WiFi, MQTT, servidor web, almacenamiento y FreeRTOS.

### `journal/`

Diario de aprendizaje con sesiones, errores, decisiones y avances.

### `assets/`

Recursos visuales usados por la documentación:

- `assets/images/` para imágenes y capturas,
- `assets/diagrams/` para diagramas conceptuales,
- `assets/schematics/` para esquemas eléctricos y cableados.

---

## Documentación principal

- [`ROADMAP.md`](ROADMAP.md): hoja de ruta del aprendizaje.
- [`CHANGELOG.md`](CHANGELOG.md): registro de cambios relevantes.
- [`CONTRIBUTING.md`](CONTRIBUTING.md): normas de estructura, documentación y estilo.
- [`docs/buenas_practicas.md`](docs/buenas_practicas.md): criterios técnicos y de trabajo.
- [`docs/lecciones_aprendidas.md`](docs/lecciones_aprendidas.md): errores, decisiones y aprendizajes reutilizables.
- [`docs/glosario.md`](docs/glosario.md): términos técnicos usados en el proyecto.
- [`hardware/pinout.md`](hardware/pinout.md): referencia de pines de la placa.
- [`hardware/gpio_reservados.md`](hardware/gpio_reservados.md): GPIO reservados, conflictivos y candidatos.

---

## Primer ejercicio

El primer ejercicio documentado es:

```text
arduino/01_rgb_neopixel/
```

Este ejercicio valida:

- compilación con PlatformIO,
- subida de firmware,
- monitor serie,
- control del LED RGB integrado en GPIO48,
- control básico de brillo mediante comandos serie.

---

## Roadmap resumido

El aprendizaje se organiza en fases:

1. Preparación del repositorio.
2. PlatformIO y primer contacto con la placa.
3. GPIO digital básico.
4. Interrupciones.
5. Temporización no bloqueante.
6. PWM, ADC y señales básicas.
7. Comunicaciones locales: UART, I2C y SPI.
8. WiFi básico.
9. TCP/IP y HTTP.
10. MQTT e IoT.
11. Cámara OV2640.
12. Almacenamiento.
13. FreeRTOS en Arduino.
14. Transición a ESP-IDF.
15. Seguridad y buenas prácticas IoT.
16. Proyectos integradores.

El detalle completo está en [`ROADMAP.md`](ROADMAP.md).

---

## Reglas esenciales

- No usar un GPIO sin comprobar antes el pinout.
- No subir credenciales al repositorio.
- Separar ejercicios Arduino y ESP-IDF.
- Mantener cada ejercicio como proyecto PlatformIO autocontenido.
- Priorizar claridad y aprendizaje antes que optimización prematura.
- Documentar errores importantes y decisiones técnicas.

Las normas completas están en [`CONTRIBUTING.md`](CONTRIBUTING.md) y [`docs/buenas_practicas.md`](docs/buenas_practicas.md).

---

## Credenciales

No se deben subir credenciales reales al repositorio.

Archivos como los siguientes deben estar excluidos por `.gitignore`:

```text
secrets.h
credentials.h
wifi_credentials.h
.env
```

Cuando un ejercicio necesite credenciales, se usará un archivo de ejemplo como:

```text
secrets.example.h
```

---

## Licencia

Este repositorio utiliza licencia MIT. Ver [`LICENSE`](LICENSE).

---

## Autor

Repositorio de aprendizaje desarrollado por **Pablo Manuel Tarrío Guirao** con apoyo didáctico de Microsoft 365 Copilot.

---

## Aviso

Este repositorio está orientado al aprendizaje. Algunos ejemplos priorizarán la claridad didáctica frente a la máxima eficiencia o robustez industrial.
