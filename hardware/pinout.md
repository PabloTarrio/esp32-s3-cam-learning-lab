# Pinout - ESP32-S3-CAM

Documento de referencia de pines para la placa **ESP32-S3-CAM** utilizada en este repositorio.

> **Importante:** esta información parte de la documentación inicial recopilada para la placa y debe validarse progresivamente con pruebas reales. En una placa ESP32-S3-CAM no todos los GPIO están disponibles para uso general: la cámara, el RGB integrado, botones, USB, UART, SD, Flash y PSRAM pueden ocupar o condicionar determinados pines.

---

## 1. Resumen de la placa

- **Placa:** ESP32-S3-CAM
- **Módulo:** ESP32-S3-WROOM-1 N16R8
- **Flash:** 16 MB
- **PSRAM:** 8 MB
- **Cámara:** OV2640
- **LED RGB integrado:** NeoPixel / WS2812
- **GPIO LED RGB:** GPIO48
- **Framework inicial:** Arduino Framework con PlatformIO
- **Framework avanzado previsto:** ESP-IDF

---

## 2. Pinout visual de referencia

La siguiente imagen muestra el pinout visual de la placa GOOUUU ESP32-S3-CAM utilizada como referencia inicial.

> **Nota:** esta imagen es una referencia visual. Algunos pines pueden tener funciones alternativas o restricciones según el uso de cámara, SD, PSRAM, USB, UART, JTAG o arranque. Antes de utilizar un GPIO en un ejercicio, se debe comprobar su disponibilidad en `hardware/gpio_reservados.md` y validarlo experimentalmente.

![Pinout GOOUUU ESP32-S3-CAM](../assets/images/goouuu_esp32_s3_cam_pinout.jpeg)

---

## 3. Regla principal de uso de GPIO

Antes de usar un GPIO en cualquier ejercicio, se debe comprobar:

- si está usado por la cámara;
- si está reservado por Flash o PSRAM;
- si está asociado a USB, UART, boot o SD;
- si está conectado a un periférico integrado;
- si está expuesto físicamente en la placa;
- si admite la función prevista: entrada, salida, ADC, I2C, SPI, PWM, interrupción, etc.

> Si un pin no está confirmado como libre o adecuado para el ejercicio, no debe usarse todavía como GPIO general.

---

## 4. Tabla general de GPIO

| GPIO | Uso conocido / función | Estado recomendado | Notas |
|---:|---|---|---|
| GPIO0 | BOOT / arranque | Evitar salvo necesidad | Puede afectar al modo de arranque. |
| GPIO1 | SDA / A0 según pinout | Usar con precaución | Posible I2C externo si se valida previamente. |
| GPIO2 | SCL / A1 según pinout | Usar con precaución | Posible I2C externo si se valida previamente. |
| GPIO3 | Switch según pinout / entrada digital validada | Validado con pulsador externo | Probado con pulsador externo a GND usando `INPUT_PULLUP`, antirrebote por software y detección de flancos. |
| GPIO4 | CAM_SIOD / SDA cámara | Reservado cámara | No usar para ejercicios generales. |
| GPIO5 | CAM_SIOC / SCL cámara | Reservado cámara | No usar para ejercicios generales. |
| GPIO6 | CAM_VSYNC | Reservado cámara | No usar para ejercicios generales. |
| GPIO7 | CAM_HREF | Reservado cámara | No usar para ejercicios generales. |
| GPIO8 | CAM_Y4 / D2 | Reservado cámara | No usar para ejercicios generales. |
| GPIO9 | CAM_Y3 / D1 | Reservado cámara | No usar para ejercicios generales. |
| GPIO10 | CAM_Y5 / D3 | Reservado cámara | No usar para ejercicios generales. |
| GPIO11 | CAM_Y2 / D0 | Reservado cámara | No usar para ejercicios generales. |
| GPIO12 | CAM_Y6 / D4 | Reservado cámara | No usar para ejercicios generales. |
| GPIO13 | CAM_PCLK | Reservado cámara | No usar para ejercicios generales. |
| GPIO14 | A13 según pinout | Revisar | Posible GPIO disponible, pendiente de validación. |
| GPIO15 | CAM_XCLK | Reservado cámara | No usar para ejercicios generales. |
| GPIO16 | CAM_Y9 / D7 | Reservado cámara | No usar para ejercicios generales. |
| GPIO17 | CAM_Y8 / D6 | Reservado cámara | No usar para ejercicios generales. |
| GPIO18 | CAM_Y7 / D5 | Reservado cámara | No usar para ejercicios generales. |
| GPIO19 | USB D+ | Evitar | Relacionado con USB. No usar como GPIO general. |
| GPIO20 | USB D- | Evitar | Relacionado con USB. No usar como GPIO general. |
| GPIO21 | BLK / Backlight según pinout | Usar con precaución | Puede estar asociado a periféricos externos o LCD. |
| GPIO22-GPIO25 | No existen / no disponibles | No usar | Según documentación de la placa. |
| GPIO26-GPIO32 | Flash / PSRAM | No usar | Reservados internamente. |
| GPIO33-GPIO34 | Missing / no expuestos | No usar | No disponibles en esta placa. |
| GPIO35-GPIO37 | PSRAM según pinout | No usar | Reservados o conflictivos. |
| GPIO38 | SD_CMD | Reservado SD | Evitar si se usa o se quiere conservar compatibilidad con SD. |
| GPIO39 | SD_CLK | Reservado SD | Evitar si se usa o se quiere conservar compatibilidad con SD. |
| GPIO40 | SD_DATA | Reservado SD | Evitar si se usa o se quiere conservar compatibilidad con SD. |
| GPIO41 | MOSI / SPI LCD según pinout | Usar con precaución | Posible SPI, revisar periféricos conectados. |
| GPIO42 | SCLK / SPI LCD según pinout | Usar con precaución | Posible SPI, revisar periféricos conectados. |
| GPIO43 | U0TXD / TX | Evitar para GPIO general | Usado para UART / monitor serie. |
| GPIO44 | U0RXD / RX | Evitar para GPIO general | Usado para UART / monitor serie. |
| GPIO45 | DC / función especial según pinout | Usar con mucha precaución | Puede estar asociado a funciones especiales/periféricos. Pendiente de validación. |
| GPIO46 | Shutter según pinout | Candidato con precaución | Pendiente de validación experimental. Puede tener funciones especiales. |
| GPIO47 | CS / SPI LCD según pinout | Usar con precaución | Puede estar asociado a SPI/periféricos. |
| GPIO48 | RGB NeoPixel / WS2812 | Reservado RGB | Usado para LED RGB integrado. Validado. |

---

## 5. Pines usados por la cámara OV2640

La cámara ocupa varios GPIO. Estos pines deben considerarse **reservados** cuando se quiera usar la cámara o mantener compatibilidad futura con ella.

| Señal cámara | GPIO | Descripción |
|---|---:|---|
| CAM_SIOD | GPIO4 | Datos SCCB/I2C de cámara. |
| CAM_SIOC | GPIO5 | Reloj SCCB/I2C de cámara. |
| CAM_VSYNC | GPIO6 | Sincronismo vertical. |
| CAM_HREF | GPIO7 | Referencia horizontal. |
| CAM_Y4 | GPIO8 | Dato paralelo cámara. |
| CAM_Y3 | GPIO9 | Dato paralelo cámara. |
| CAM_Y5 | GPIO10 | Dato paralelo cámara. |
| CAM_Y2 | GPIO11 | Dato paralelo cámara. |
| CAM_Y6 | GPIO12 | Dato paralelo cámara. |
| CAM_PCLK | GPIO13 | Pixel clock. |
| CAM_XCLK | GPIO15 | Reloj externo de cámara. |
| CAM_Y9 | GPIO16 | Dato paralelo cámara. |
| CAM_Y8 | GPIO17 | Dato paralelo cámara. |
| CAM_Y7 | GPIO18 | Dato paralelo cámara. |

### Consecuencia práctica

No se utilizarán estos pines para ejercicios generales como:

- botón externo;
- LED externo;
- I2C externo;
- SPI externo;
- sensores analógicos;
- PWM;
- interrupciones generales.

Aunque alguno pudiera funcionar si la cámara no se inicializa, se evitará para mantener una arquitectura limpia y compatible con futuros ejercicios de cámara.

---

## 6. LED RGB integrado

| Elemento | Valor |
|---|---|
| Tipo | NeoPixel / WS2812 |
| GPIO | GPIO48 |
| Control | Protocolo digital direccionable |
| Librería usada | `Adafruit_NeoPixel` |
| Estado | Validado |

### Nota importante

El LED RGB integrado **no se controla como un LED normal**.

No se debe usar:

```cpp
digitalWrite(48, HIGH);
```

Se debe usar una librería compatible con NeoPixel / WS2812, por ejemplo:

```cpp
Adafruit_NeoPixel pixel(1, 48, NEO_GRB + NEO_KHZ800);
```

---

## 7. Botones y pulsadores

Según la documentación inicial de la placa, existen al menos estos pines asociados a botones o funciones de switch/shutter:

| GPIO | Nombre asociado | Estado |
|---:|---|---|
| GPIO3 | Switch según pinout | Validado como entrada digital con pulsador externo. |
| GPIO46 | Shutter según pinout | Candidato pendiente de validación. |

### GPIO3 validado con pulsador externo

El GPIO3 se ha validado con un pulsador externo conectado en protoboard:

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
- uso de `INPUT_PULLUP`;
- antirrebote por software con `millis()`;
- detección de flanco descendente `HIGH -> LOW`;
- detección de flanco ascendente `LOW -> HIGH`;
- cambio de color del RGB integrado mediante pulsación válida.

---

## 8. UART y USB

| GPIO | Función | Recomendación |
|---:|---|---|
| GPIO43 | U0TXD / TX | Reservar para comunicación serie. |
| GPIO44 | U0RXD / RX | Reservar para comunicación serie. |
| GPIO19 | USB D+ | No usar como GPIO general. |
| GPIO20 | USB D- | No usar como GPIO general. |

Durante el aprendizaje se usará el monitor serie de PlatformIO. Por tanto, los pines asociados a UART/USB deben tratarse con precaución.

---

## 9. Flash y PSRAM

| Rango GPIO | Uso | Recomendación |
|---|---|---|
| GPIO26-GPIO32 | Flash / PSRAM | No usar. |
| GPIO35-GPIO37 | PSRAM según pinout | No usar. |

Estos pines están relacionados con la memoria interna del módulo o aparecen marcados como conflictivos. No deben utilizarse como GPIO de propósito general.

---

## 10. Tarjeta SD / SDMMC

Según la documentación inicial, la tarjeta SD puede estar asociada a:

| GPIO | Función SD | Recomendación |
|---:|---|---|
| GPIO38 | SD_CMD | Reservar para SD. |
| GPIO39 | SD_CLK | Reservar para SD. |
| GPIO40 | SD_DATA | Reservar para SD. |

Evitar el uso de GPIO38, GPIO39 y GPIO40 hasta confirmar si la placa utiliza SD y si esos pines están cableados al slot correspondiente.

---

## 11. Uso previsto por tipo de ejercicio

| Tipo de ejercicio | Pines recomendados inicialmente | Observaciones |
|---|---|---|
| RGB integrado | GPIO48 | Validado. |
| Pulsador externo | GPIO3 | Validado con `INPUT_PULLUP`, antirrebote y flancos. |
| Botón integrado / shutter | GPIO46 | Pendiente de prueba. |
| I2C externo | GPIO1 / GPIO2 posibles | Revisar conflictos antes. |
| SPI externo | GPIO41 / GPIO42 / GPIO47 posibles | Revisar si hay periféricos conectados. |
| Cámara | GPIO4-GPIO13, GPIO15-GPIO18 | Reservados para OV2640. |
| UART debug | GPIO43 / GPIO44 o USB | No usar como GPIO general. |
| SD | GPIO38 / GPIO39 / GPIO40 | Reservar para SD. |

---

## 12. Bitácora de validación de pines

| Fecha | GPIO | Prueba realizada | Resultado | Notas |
|---|---:|---|---|---|
| 2026-07-03 | GPIO48 | Control RGB NeoPixel | Correcto | Probado con `Adafruit_NeoPixel`. |
| 2026-07-03 | GPIO3 | Pulsador externo con `INPUT_PULLUP` | Correcto | Validado como entrada digital con antirrebote software y detección de flancos. |
| 2026-07-03 | GPIO3 + GPIO48 | Pulsador externo controlando RGB integrado | Correcto | Cada flanco descendente válido cambia el color del LED RGB. |
| Pendiente | GPIO46 | Botón integrado / Shutter | Pendiente | Candidato secundario. |

---

## 13. Documentos relacionados

```text
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
hardware/camara_ov2640.md
hardware/alimentacion.md
hardware/SD.md
arduino/01_rgb_neopixel/
arduino/02_boton_gpio/
```
