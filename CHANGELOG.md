# CHANGELOG

Todos los cambios relevantes de este repositorio se documentarán en este archivo.

El formato está inspirado en [Keep a Changelog](https://keepachangelog.com/) y el versionado seguirá, cuando tenga sentido, una aproximación compatible con [Semantic Versioning](https://semver.org/).

---

## [Unreleased]

### Added

- Añadida licencia MIT en `LICENSE`.
- Añadida documentación base en `docs/`:
  - `docs/lecciones_aprendidas.md`
  - `docs/buenas_practicas.md`
  - `docs/glosario.md`
- Añadida documentación inicial de hardware en `hardware/`:
  - `hardware/README.md`
  - `hardware/pinout.md`
  - `hardware/gpio_reservados.md`
  - `hardware/rgb_neopixel.md`
  - `hardware/camara_ov2640.md`
  - `hardware/alimentacion.md`
  - `hardware/SD.md`
- Añadida imagen de referencia del pinout en `assets/images/` y enlazada desde la documentación de hardware.
- Añadida documentación del primer ejercicio Arduino:
  - `arduino/01_rgb_neopixel/README.md`
- Añadida configuración PlatformIO del primer ejercicio:
  - `arduino/01_rgb_neopixel/platformio.ini`
- Añadido código del primer ejercicio:
  - `arduino/01_rgb_neopixel/src/main.cpp`
- Añadido ejercicio Arduino `02_boton_gpio`:
  - lectura de pulsador externo en `GPIO3`;
  - uso de `INPUT_PULLUP`;
  - antirrebote por software con `millis()`;
  - detección de flanco descendente y ascendente;
  - cambio de color del LED RGB integrado en cada pulsación válida.

### Changed

- Reducido el `README.md` del ejercicio `arduino/01_rgb_neopixel/` para evitar duplicar la explicación detallada del código.
- Adoptado el criterio de que la explicación pedagógica del código debe estar principalmente en comentarios dentro de `src/main.cpp`.
- Ajustados documentos técnicos para evitar secciones de planificación diaria como “próximos pasos”.
- Corregida la codificación del archivo `main.cpp` a UTF-8.
- Actualizado `hardware/pinout.md`:
  - conversión a tablas Markdown limpias;
  - corrección de numeración de secciones;
  - actualización de `GPIO3` como entrada digital validada;
  - actualización de bitácora de validación.
- Actualizado `hardware/gpio_reservados.md`:
  - conversión a tablas Markdown limpias;
  - actualización de `GPIO3` como GPIO validado para pulsador externo;
  - mantenimiento de `GPIO46` como candidato pendiente de validación;
  - actualización de tabla resumen y bitácora.
- Actualizado `docs/lecciones_aprendidas.md`:
  - limpieza general de formato Markdown;
  - nuevas lecciones sobre antirrebote, flancos y pulsador externo;
  - nuevas lecciones de C++ sobre paso por referencia y uso correcto de parámetros.

### Documented

- Documentada la regla de usar `assets/` para imágenes, diagramas y esquemas.
- Documentada la reserva de GPIO usados por cámara, SD, UART, USB, PSRAM, Flash y RGB integrado.
- Documentado el uso de `GPIO48` para el LED RGB integrado NeoPixel / WS2812.
- Documentado el criterio de no usar GPIO de cámara para ejercicios generales.
- Documentado el uso de `Adafruit_NeoPixel` para el RGB integrado.
- Documentada la separación entre documentación estable, roadmap, changelog y journal.
- Documentado el cableado externo del pulsador con protoboard:

```text
GPIO3 ---- pulsador ---- GND
```

- Documentada la lógica usada con `INPUT_PULLUP`:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
```

- Documentado `GPIO3` como entrada digital validada para pulsador externo.
- Documentada la diferencia entre estado del botón y evento de flanco.
- Documentado el patrón:

```text
lectura -> antirrebote -> estado estable -> detección de flanco -> acción
```

### Learned

- Diferencia entre lectura directa de un pulsador y lectura filtrada con antirrebote.
- Los pulsadores mecánicos generan rebote y pueden producir múltiples lecturas por una sola pulsación física.
- `millis()` permite implementar antirrebote sin bloquear el programa.
- Con `INPUT_PULLUP`, una pulsación corresponde a un flanco descendente `HIGH -> LOW`.
- Para ejecutar una acción una sola vez por pulsación, conviene detectar flancos y no solo estados.
- Una función que debe modificar una variable externa debe recibirla por referencia o devolver el nuevo valor.
- Si una función recibe un objeto como parámetro, debe operar sobre ese objeto y no sobre una variable global oculta.

### Fixed

- Corregido el comentario de prueba de `rgb_blink()` para reflejar que la función requiere el argumento `pixel`.
- Corregidos problemas de codificación UTF-8 en comentarios del código fuente.
- Corregida la creación del objeto `Adafruit_NeoPixel` en el ejercicio `02_boton_gpio` para usar `RGB_PIN` en lugar de `BUTTON_PIN`.
- Corregidos prototipos de funciones para que coincidan con las definiciones con referencias:
  - `Adafruit_NeoPixel &rgb_led`
  - `int &index`
- Corregido el uso interno de funciones para operar sobre el objeto recibido por parámetro (`rgb_led`) en lugar de depender del objeto global `pixel`.

---

## [0.1.0] - 2026-07-03

### Added

- Creación inicial de la estructura del repositorio `esp32-s3-cam-learning-lab`.
- Creación del documento `ROADMAP.md` con la hoja de ruta general del aprendizaje.
- Creación del documento `README.md` principal del repositorio.
- Creación del documento `CONTRIBUTING.md` con normas de contribución, documentación y estilo.
- Creación del archivo `.gitignore` para excluir:
  - archivos de PlatformIO;
  - archivos de compilación;
  - archivos temporales de Python;
  - archivos del sistema operativo;
  - credenciales;
  - logs.
- Definición de la estructura principal de carpetas:
  - `docs/`
  - `hardware/`
  - `arduino/`
  - `esp-idf/`
  - `proyectos/`
  - `journal/`
  - `assets/`
- Definición de la estructura recomendada para ejercicios Arduino:

```text
arduino/XX_nombre_ejercicio/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

- Definición de las reglas iniciales del proyecto:
  - aprendizaje paso a paso;
  - separación entre Arduino y ESP-IDF;
  - documentación obligatoria por ejercicio;
  - código comentado pedagógicamente;
  - revisión previa de GPIO antes de usarlos;
  - no subir credenciales al repositorio.

### Documented

- Placa principal del proyecto:
  - ESP32-S3-CAM basada en ESP32-S3-WROOM-1 N16R8;
  - 16 MB Flash;
  - 8 MB PSRAM;
  - cámara OV2640;
  - LED RGB integrado NeoPixel / WS2812 en GPIO48.
- Estado inicial del aprendizaje:
  - PlatformIO instalado y probado;
  - primer programa compilado y subido correctamente;
  - monitor serie funcionando;
  - LED RGB integrado controlado correctamente;
  - brillo del LED RGB controlado mediante comandos serie `+` y `-`.

### Learned

- Diferencia entre un LED normal y un NeoPixel / WS2812.
- Uso básico de la librería `Adafruit_NeoPixel`.
- Importancia de llamar a `show()` después de modificar el estado del NeoPixel.
- Necesidad de reenviar el color con `setPixelColor()` al modificar el brillo con `setBrightness()` en ciertos casos.
- Diferencia entre pasar objetos por valor y por referencia en C++.
- Problema de sombreado de variables (*shadowing*) al declarar una variable local con el mismo nombre que una global.
- Importancia de separar responsabilidades en funciones.
- Conveniencia de mantener las impresiones por serie fuera de funciones cuya responsabilidad principal sea otra.

### Fixed

- Corrección del uso de `digitalWrite()` para el LED integrado: el LED de la placa es NeoPixel / WS2812 y no un LED digital simple.
- Corrección del pin del LED integrado: uso de `GPIO48`.
- Corrección del paso por valor de `Adafruit_NeoPixel`; se pasa ahora por referencia mediante `Adafruit_NeoPixel &`.
- Corrección de shadowing en la variable `actual_bright`.
- Corrección del control de brillo del NeoPixel para aplicar correctamente el cambio visual.

---

## Convenciones de secciones

### Added

Para nuevas funcionalidades, documentos, ejercicios, carpetas o proyectos.

### Changed

Para cambios en contenido existente.

### Deprecated

Para funcionalidades o enfoques que se mantengan temporalmente pero se prevea eliminar.

### Removed

Para contenido eliminado.

### Fixed

Para correcciones de errores.

### Security

Para cambios relacionados con seguridad, credenciales, TLS, MQTT seguro, OTA o protección del dispositivo.

### Documented

Para documentación nueva o ampliada.

### Learned

Para registrar aprendizajes relevantes derivados de errores, pruebas o decisiones técnicas.
