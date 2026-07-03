# ROADMAP - ESP32-S3-CAM Learning Lab

Hoja de ruta de aprendizaje para trabajar con una placa **ESP32-S3-CAM** basada en **ESP32-S3-WROOM-1 N16R8**, cámara **OV2640**, entorno **PlatformIO**, **Arduino Framework** y posterior transición a **ESP-IDF**.

Este documento es una guía viva: podrá modificarse, ampliarse o reordenarse conforme avance el aprendizaje.

---

## Propósito

El objetivo no es únicamente crear ejemplos aislados, sino construir progresivamente un repositorio técnico y didáctico con:

- documentación del hardware,
- ejercicios básicos,
- ejemplos reutilizables,
- proyectos completos,
- código comentado pedagógicamente,
- registro de errores y decisiones,
- transición progresiva desde Arduino Framework hacia ESP-IDF.

---


## Fase 0 - Preparación del repositorio

**Objetivo:** crear una base ordenada para todo el aprendizaje.

### Tareas

- [x] Crear estructura principal de carpetas.
- [x] Crear `README.md` principal.
- [x] Crear `ROADMAP.md`.
- [x] Crear `CHANGELOG.md`.
- [x] Crear `CONTRIBUTING.md`.
- [x] Crear `.gitignore`.
- [x] Crear `LICENSE`.
- [x] Crear documentación inicial de `hardware/`.
- [x] Crear documentación inicial de `docs/`.
- [x] Crear documentación del primer ejercicio.
- [x] Crear primera entrada en `journal/`.
- [x] Realizar primer commit.

---

## Fase 1 - PlatformIO y primer contacto con la placa

**Objetivo:** validar entorno, compilación, subida de firmware y monitor serie.

### Ejercicios

- [x] Crear primer proyecto PlatformIO.
- [x] Compilar firmware.
- [x] Subir firmware a la placa.
- [x] Abrir monitor serie.
- [x] Imprimir mensaje de arranque.
- [x] Controlar LED RGB integrado.
- [x] Controlar brillo del NeoPixel por puerto serie.
- [x] Documentar `arduino/01_rgb_neopixel/`.
- [x] Registrar lecciones aprendidas.

### Conceptos trabajados

- PlatformIO.
- `platformio.ini`.
- `src/main.cpp`.
- Librerías externas.
- Monitor serie.
- NeoPixel WS2812.
- Paso por referencia en C++.
- Variables globales y locales.
- Shadowing de variables.
- Separación básica de responsabilidades.

---

## Fase 2 - GPIO digital básico

**Objetivo:** aprender entradas y salidas digitales en ESP32-S3.

### Ejercicios

- [ ] Salida digital simple.
- [ ] Entrada digital simple.
- [ ] Uso de `INPUT_PULLUP`.
- [ ] Lectura de botón integrado.
- [ ] Lectura de botón externo.
- [ ] Detección de estado pulsado/liberado.
- [ ] Detección de flanco.
- [ ] Cambio de color del RGB mediante pulsador.
- [ ] Documentar cableado.

### Conceptos

- GPIO.
- Niveles lógicos de 3.3 V.
- Pull-up interno.
- Pull-down.
- Rebotes mecánicos.
- Diferencia entre estado y evento.

---

## Fase 3 - Interrupciones

**Objetivo:** entender eventos hardware, ISR y limitaciones de interrupciones.

### Ejercicios

- [ ] Interrupción por flanco descendente.
- [ ] Interrupción por flanco ascendente.
- [ ] Uso de `volatile`.
- [ ] Comunicación entre ISR y `loop()`.
- [ ] Antirrebote básico con interrupciones.
- [ ] Antirrebote mediante temporización.
- [ ] Comparar polling frente a interrupción.

### Conceptos

- ISR.
- `attachInterrupt()`.
- `volatile`.
- Código seguro dentro de interrupciones.
- Qué no hacer dentro de una ISR.
- Analogía y diferencias con sistemas PLC.

---

## Fase 4 - Temporización no bloqueante

**Objetivo:** abandonar progresivamente el uso de `delay()` en programas con varias tareas.

### Ejercicios

- [ ] Parpadeo con `millis()`.
- [ ] Temporizador software simple.
- [ ] Máquina de estados básica.
- [ ] RGB + botón sin bloqueos.
- [ ] Varias tareas simuladas en `loop()`.

### Conceptos

- `delay()` bloqueante.
- `millis()`.
- Temporización cooperativa.
- Máquina de estados.
- Escalabilidad del código.

---

## Fase 5 - PWM, ADC y señales básicas

**Objetivo:** conectar el ESP32-S3 con señales físicas simples.

### Ejercicios

- [ ] PWM sobre LED externo.
- [ ] Control de brillo por PWM.
- [ ] Lectura de potenciómetro.
- [ ] Escalado ADC 0-4095.
- [ ] Conversión a voltaje.
- [ ] Filtro simple por media móvil.
- [ ] Control de RGB a partir de entrada analógica.

### Conceptos

- PWM.
- ADC de 12 bits.
- Resolución.
- Ruido en señales analógicas.
- Escalado de magnitudes.
- Comparación con señales industriales.

---

## Fase 6 - Comunicaciones locales

**Objetivo:** dominar buses y comunicaciones locales antes de entrar en redes.

### UART

- [ ] Envío de texto por puerto serie.
- [ ] Recepción de comandos.
- [ ] Parser simple de comandos.
- [ ] Protocolo textual básico.

### I2C

- [ ] Escáner I2C.
- [ ] Lectura de sensor I2C.
- [ ] Escritura en periférico I2C.
- [ ] Documentar direcciones.

### SPI

- [ ] Conceptos MOSI, MISO, SCLK y CS.
- [ ] Uso de periférico SPI simple.
- [ ] Precauciones con pines compartidos.

---

## Fase 7 - WiFi básico

**Objetivo:** conectar la placa a una red WiFi y entender los modos principales.

### Ejercicios

- [ ] Conexión en modo Station.
- [ ] Mostrar IP asignada.
- [ ] Reconexión automática.
- [ ] Escaneo de redes.
- [ ] Modo Access Point.
- [ ] Modo Station + Access Point.
- [ ] Gestión segura de credenciales.

### Conceptos

- SSID.
- Dirección IP.
- Gateway.
- DNS.
- DHCP.
- Modo STA.
- Modo AP.
- Reconexión.

---

## Fase 8 - TCP/IP y HTTP

**Objetivo:** crear comunicaciones de red básicas y servicios web embebidos.

### Ejercicios

- [ ] Cliente HTTP.
- [ ] Servidor HTTP básico.
- [ ] Página web servida desde el ESP32.
- [ ] Control del RGB desde navegador.
- [ ] API REST simple.
- [ ] Envío y recepción de JSON.
- [ ] Diagnóstico de errores de red.

### Conceptos

- Cliente/servidor.
- HTTP GET.
- HTTP POST.
- Códigos de estado HTTP.
- JSON.
- Latencia.
- Bloqueos en red.

---

## Fase 9 - MQTT e IoT

**Objetivo:** trabajar con un protocolo típico de IoT.

### Ejercicios

- [ ] Conectar a broker MQTT.
- [ ] Publicar estado del dispositivo.
- [ ] Suscribirse a comandos.
- [ ] Controlar RGB por MQTT.
- [ ] Publicar lecturas de sensores.
- [ ] Usar retained messages.
- [ ] Usar Last Will and Testament.
- [ ] Integración con Node-RED.
- [ ] Posible integración con Home Assistant.

### Conceptos

- Broker.
- Cliente MQTT.
- Topic.
- Publish.
- Subscribe.
- QoS.
- Retain.
- LWT.
- Arquitectura IoT.

---

## Fase 10 - Cámara OV2640

**Objetivo:** utilizar la cámara integrada y comprender implicaciones de memoria, pines y rendimiento.

### Ejercicios

- [ ] Inicializar cámara.
- [ ] Capturar imagen JPEG.
- [ ] Configurar resolución.
- [ ] Configurar calidad JPEG.
- [ ] Usar PSRAM.
- [ ] Servidor web con captura de imagen.
- [ ] Streaming MJPEG.
- [ ] Ajustar rendimiento.
- [ ] Diagnosticar errores de cámara.

### Conceptos

- OV2640.
- DVP camera interface.
- Frame buffer.
- JPEG.
- PSRAM.
- Resolución frente a memoria.
- Cámara y WiFi simultáneos.

---

## Fase 11 - Almacenamiento

**Objetivo:** guardar datos, configuración, logs e imágenes.

### Ejercicios

- [ ] Uso de LittleFS o SPIFFS.
- [ ] Guardar configuración.
- [ ] Leer configuración en arranque.
- [ ] Guardar logs.
- [ ] Uso de tarjeta SD si la placa lo permite.
- [ ] Guardar imágenes capturadas.
- [ ] Servir archivos desde sistema de ficheros.

### Conceptos

- Sistema de archivos embebido.
- Persistencia.
- Desgaste de memoria Flash.
- Configuración local.
- SD/MMC.

---

## Fase 12 - FreeRTOS en Arduino

**Objetivo:** comprender la multitarea real que hay debajo del ESP32.

### Ejercicios

- [ ] Crear una tarea FreeRTOS.
- [ ] Ejecutar dos tareas en paralelo.
- [ ] Usar `vTaskDelay()`.
- [ ] Prioridades de tareas.
- [ ] Tareas fijadas a núcleo.
- [ ] Colas.
- [ ] Semáforos.
- [ ] Mutex.
- [ ] Separar tareas de lectura, red y control.

### Conceptos

- Tarea.
- Scheduler.
- Core 0 y Core 1.
- Prioridad.
- Cola.
- Semáforo.
- Mutex.
- Condiciones de carrera.

---

## Fase 13 - Transición a ESP-IDF

**Objetivo:** pasar del estilo Arduino al entorno nativo profesional de Espressif.

### Ejercicios

- [ ] Crear proyecto ESP-IDF en PlatformIO.
- [ ] Entender `app_main()`.
- [ ] GPIO con drivers ESP-IDF.
- [ ] Logging con `ESP_LOGI`, `ESP_LOGW`, `ESP_LOGE`.
- [ ] FreeRTOS nativo.
- [ ] WiFi con ESP-IDF.
- [ ] MQTT con ESP-IDF.
- [ ] Cámara con ESP-IDF.
- [ ] Comparar implementación Arduino frente a ESP-IDF.

### Conceptos

- Componentes ESP-IDF.
- `sdkconfig`.
- Drivers nativos.
- Logging.
- Event loop.
- Manejo de errores con `esp_err_t`.

---

## Fase 14 - Seguridad y buenas prácticas IoT

**Objetivo:** acercar los proyectos a prácticas más robustas y reales.

### Temas

- [ ] Gestión segura de credenciales.
- [ ] Separación de secretos.
- [ ] TLS.
- [ ] MQTT sobre TLS.
- [ ] Certificados.
- [ ] OTA updates.
- [ ] Watchdog.
- [ ] Manejo de errores.
- [ ] Recuperación ante fallos.
- [ ] Particiones.
- [ ] Logs de diagnóstico.

---

## Fase 15 - Proyectos integradores

**Objetivo:** combinar lo aprendido en aplicaciones completas.

### Proyectos propuestos

- [ ] Cámara IP básica.
- [ ] Cámara con servidor web de control.
- [ ] Cámara con captura bajo evento.
- [ ] Publicación de estado por MQTT.
- [ ] Sensor IoT con dashboard.
- [ ] Nodo compatible con Home Assistant.
- [ ] Sistema de vigilancia local.
- [ ] Proyecto con Node-RED.
- [ ] Sistema IoT completo con:
  - cámara,
  - WiFi,
  - MQTT,
  - servidor web,
  - almacenamiento,
  - FreeRTOS,
  - documentación completa.

---

## Criterios de finalización de cada ejercicio

Un ejercicio se considerará terminado cuando tenga:

- [ ] Código que compila.
- [ ] Código probado en placa.
- [ ] `README.md` del ejercicio.
- [ ] Hardware utilizado documentado.
- [ ] Resultado esperado documentado.
- [ ] Problemas encontrados documentados, si los hubo.
- [ ] Relación con documentación general o hardware, si aplica.

---

## Notas finales

Este roadmap no pretende ser rígido. Es una guía de navegación.

A medida que avance el aprendizaje, algunas fases podrán:

- ampliarse,
- dividirse,
- reordenarse,
- simplificarse,
- convertirse en proyectos propios.

El objetivo final es doble:

- aprender ESP32-S3-CAM con profundidad,
- construir un repositorio técnico útil, consultable y mantenible.
