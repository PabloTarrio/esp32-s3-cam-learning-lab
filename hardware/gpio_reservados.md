# GPIO reservados y recomendados - ESP32-S3-CAM

Documento de referencia rápida para decidir qué GPIO se pueden usar, cuáles están reservados y cuáles requieren precaución en la placa **ESP32-S3-CAM** usada en este repositorio.

Este documento complementa a:

```text
hardware/pinout.md
```

Si hay duda entre este documento y el pinout general, se debe consultar primero `hardware/pinout.md` y después validar el pin mediante una prueba real.

---

## 1. Objetivo

El objetivo de este documento es evitar errores típicos al usar GPIO en una placa ESP32-S3-CAM.

En esta placa no todos los GPIO del ESP32-S3 están disponibles para uso general, porque algunos están conectados a:

- cámara OV2640;
- memoria Flash;
- PSRAM;
- USB;
- UART;
- tarjeta SD;
- LED RGB integrado;
- botones integrados o funciones especiales;
- señales de arranque;
- posibles periféricos externos.

---

## 2. Clasificación rápida

| Categoría | Significado |
|---|---|
| Reservado | No usar para ejercicios generales. |
| Precaución | Puede usarse solo si se entiende el posible conflicto. |
| Candidato | Puede probarse en ejercicios básicos, pero todavía necesita validación. |
| Validado | Probado correctamente en esta placa para un uso concreto. |

> Un GPIO validado para un uso concreto no debe considerarse automáticamente libre para cualquier otro uso.

---

## 3. GPIO reservados por la cámara OV2640

Estos pines están conectados a la cámara y deben considerarse reservados.

| GPIO | Función cámara | Recomendación |
|---:|---|---|
| GPIO4 | CAM_SIOD | No usar. |
| GPIO5 | CAM_SIOC | No usar. |
| GPIO6 | CAM_VSYNC | No usar. |
| GPIO7 | CAM_HREF | No usar. |
| GPIO8 | CAM_Y4 | No usar. |
| GPIO9 | CAM_Y3 | No usar. |
| GPIO10 | CAM_Y5 | No usar. |
| GPIO11 | CAM_Y2 | No usar. |
| GPIO12 | CAM_Y6 | No usar. |
| GPIO13 | CAM_PCLK | No usar. |
| GPIO15 | CAM_XCLK | No usar. |
| GPIO16 | CAM_Y9 | No usar. |
| GPIO17 | CAM_Y8 | No usar. |
| GPIO18 | CAM_Y7 | No usar. |

### Regla práctica

Aunque alguno de estos pines pudiera funcionar si la cámara no está inicializada, se evitarán en ejercicios generales para mantener compatibilidad con futuros proyectos de cámara.

---

## 4. GPIO reservados por Flash y PSRAM

Estos pines están asociados a memoria externa o aparecen marcados como conflictivos en el pinout de la placa.

| GPIO | Uso | Recomendación |
|---:|---|---|
| GPIO26 | Flash / PSRAM | No usar. |
| GPIO27 | Flash / PSRAM | No usar. |
| GPIO28 | Flash / PSRAM | No usar. |
| GPIO29 | Flash / PSRAM | No usar. |
| GPIO30 | Flash / PSRAM | No usar. |
| GPIO31 | Flash / PSRAM | No usar. |
| GPIO32 | Flash / PSRAM | No usar. |
| GPIO35 | PSRAM | No usar. |
| GPIO36 | PSRAM | No usar. |
| GPIO37 | PSRAM | No usar. |

### Regla práctica

No se usarán estos pines en ejercicios de GPIO, comunicación, sensores o periféricos externos.

---

## 5. GPIO relacionados con USB y UART

Estos pines están relacionados con depuración, programación, monitor serie o USB.

| GPIO | Función | Recomendación |
|---:|---|---|
| GPIO19 | USB D+ | No usar como GPIO general. |
| GPIO20 | USB D- | No usar como GPIO general. |
| GPIO43 | U0TXD / TX | Reservar para comunicación serie. |
| GPIO44 | U0RXD / RX | Reservar para comunicación serie. |

### Regla práctica

Durante el aprendizaje se usará el monitor serie de PlatformIO. Estos pines deben mantenerse libres para comunicación y diagnóstico.

---

## 6. GPIO relacionados con tarjeta SD

Según el pinout inicial de la placa, la tarjeta SD puede usar los siguientes pines:

| GPIO | Función SD | Recomendación |
|---:|---|---|
| GPIO38 | SD_CMD | Reservar para SD. |
| GPIO39 | SD_CLK | Reservar para SD. |
| GPIO40 | SD_DATA | Reservar para SD. |

### Regla práctica

No se usarán estos pines para ejercicios generales mientras se mantenga la posibilidad de utilizar tarjeta SD en el futuro.

---

## 7. GPIO del LED RGB integrado

| GPIO | Uso | Estado |
|---:|---|---|
| GPIO48 | NeoPixel / WS2812 | Validado para RGB integrado. |

### Recomendación

`GPIO48` queda reservado para el LED RGB integrado.

No debe usarse como GPIO general.

---

## 8. GPIO de arranque o funciones especiales

| GPIO | Función conocida | Recomendación |
|---:|---|---|
| GPIO0 | BOOT | Evitar salvo necesidad. |
| GPIO45 | Función especial / posible periférico | Usar con mucha precaución. |
| GPIO46 | Shutter / función especial posible | Candidato, validar antes. |

### Nota sobre GPIO0

`GPIO0` puede afectar al modo de arranque del ESP32-S3. No se usará en ejercicios generales.

### Nota sobre GPIO45 y GPIO46

Estos pines pueden tener funciones especiales o estar conectados a elementos de la placa. Se validarán experimentalmente antes de usarlos en ejercicios definitivos.

---

## 9. GPIO validados

| GPIO | Uso validado | Estado | Notas |
|---:|---|---|---|
| GPIO3 | Entrada digital con pulsador externo | Validado | Probado con pulsador externo conectado a GND usando `INPUT_PULLUP`. Validado con antirrebote por software y detección de flancos. |
| GPIO48 | LED RGB integrado NeoPixel / WS2812 | Validado | Probado con `Adafruit_NeoPixel`. Reservado para RGB integrado. |

### Nota sobre GPIO3

`GPIO3` se ha validado con el siguiente cableado externo:

```text
GPIO3 ---- pulsador ---- GND
```

Configuración usada:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

Lógica observada:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
```

Se ha validado:

- lectura digital;
- `INPUT_PULLUP`;
- antirrebote por software con `millis()`;
- flanco descendente `HIGH -> LOW`;
- flanco ascendente `LOW -> HIGH`;
- cambio de color del RGB integrado con pulsación válida.

---

## 10. GPIO candidatos para pruebas futuras

Los siguientes pines son candidatos para ejercicios básicos, pero deben validarse antes de considerarse libres.

| GPIO | Motivo | Estado |
|---:|---|---|
| GPIO46 | Shutter integrado | Candidato con precaución. |
| GPIO1 | Posible SDA / A0 | Candidato con precaución. |
| GPIO2 | Posible SCL / A1 | Candidato con precaución. |
| GPIO14 | Posible GPIO disponible | Candidato con precaución. |
| GPIO21 | Posible BLK / GPIO | Candidato con precaución. |
| GPIO41 | Posible MOSI / SPI | Candidato con precaución. |
| GPIO42 | Posible SCLK / SPI | Candidato con precaución. |
| GPIO47 | Posible CS / SPI | Candidato con precaución. |

---

## 11. GPIO no disponibles o no recomendados

| GPIO | Motivo |
|---:|---|
| GPIO22-GPIO25 | No disponibles según pinout de la placa. |
| GPIO33-GPIO34 | No expuestos / missing según pinout. |

---

## 12. Tabla resumen de decisión rápida

| Grupo | GPIO | Uso recomendado |
|---|---|---|
| RGB integrado | GPIO48 | Usar solo para NeoPixel. |
| Pulsador externo validado | GPIO3 | Validado como entrada digital con `INPUT_PULLUP`. |
| Cámara | GPIO4-GPIO13, GPIO15-GPIO18 | No usar. |
| Flash / PSRAM | GPIO26-GPIO32, GPIO35-GPIO37 | No usar. |
| USB | GPIO19-GPIO20 | No usar. |
| UART | GPIO43-GPIO44 | Reservar para comunicación serie. |
| SD | GPIO38-GPIO40 | Reservar para SD. |
| Boot | GPIO0 | Evitar. |
| Botón / shutter candidato | GPIO46 | Validar antes de usar. |
| I2C candidato | GPIO1-GPIO2 | Validar antes. |
| SPI candidato | GPIO41-GPIO42-GPIO47 | Validar antes. |

---

## 13. Reglas prácticas antes de cablear

Antes de conectar un componente externo:

- consultar este archivo;
- consultar `hardware/pinout.md`;
- comprobar si el GPIO está físicamente expuesto;
- comprobar si el GPIO está reservado o validado para otro uso;
- confirmar que el componente trabaja a 3.3 V o adaptar niveles;
- hacer una prueba mínima de lectura/escritura;
- registrar el resultado en la bitácora de validación.

---

## 14. Bitácora de validación

| Fecha | GPIO | Prueba | Resultado | Estado actualizado |
|---|---:|---|---|---|
| 2026-07-03 | GPIO48 | Control de NeoPixel integrado | Correcto | Validado para RGB. |
| 2026-07-03 | GPIO3 | Pulsador externo con `INPUT_PULLUP` | Correcto | Validado como entrada digital. |
| 2026-07-03 | GPIO3 | Antirrebote por software con `millis()` | Correcto | Validado para pulsador externo con filtrado de rebote. |
| 2026-07-03 | GPIO3 | Detección de flanco descendente y ascendente | Correcto | Validado para detección de eventos de pulsación y liberación. |
| 2026-07-03 | GPIO3 + GPIO48 | Pulsador externo controlando RGB integrado | Correcto | Cada pulsación válida cambia el color del LED RGB. |
| Pendiente | GPIO46 | Lectura de botón / shutter | Pendiente | Candidato con precaución. |

---

## 15. Advertencia final

Este documento es una guía práctica. No sustituye las pruebas reales sobre la placa.

Cada vez que se confirme un GPIO como válido, conflictivo o reservado, debe actualizarse este archivo y, si procede, también:

```text
hardware/pinout.md
```
