# Cámara OV2640 - ESP32-S3-CAM

Documento de referencia sobre la cámara **OV2640** integrada en la placa **ESP32-S3-CAM** utilizada en este repositorio.

Este documento tiene como objetivo recoger la información necesaria para usar la cámara de forma ordenada cuando se desarrollen los ejercicios y proyectos relacionados con captura de imagen, servidor web, streaming e IoT.

---

## 1. Resumen

| Elemento | Valor |
|---|---|
| Sensor de cámara | OV2640 |
| Tipo de interfaz | Paralela DVP |
| Control de configuración | SCCB, compatible conceptualmente con I2C |
| Uso previsto | Captura JPEG, streaming MJPEG, cámara IP, proyectos IoT |
| Requisito importante | Uso de PSRAM recomendado |
| Framework inicial | Arduino Framework sobre PlatformIO |
| Framework avanzado previsto | ESP-IDF |

---

## 2. Importancia de la cámara en esta placa

La cámara es uno de los periféricos principales de la placa ESP32-S3-CAM.

A diferencia de un sensor simple conectado por I2C o SPI, la OV2640 utiliza una interfaz paralela que ocupa muchos GPIO. Esto implica que varios pines de la placa deben considerarse reservados para la cámara y no deben reutilizarse en ejercicios generales.

La cámara también puede consumir una cantidad significativa de memoria, especialmente cuando se usan resoluciones altas o varios frame buffers. Por este motivo, la PSRAM de la placa es importante para los ejercicios de imagen.

---

## 3. Pines usados por la cámara

Según el pinout usado como referencia para esta placa, la cámara OV2640 utiliza los siguientes GPIO:

| Señal cámara | GPIO | Descripción |
|---|---:|---|
| CAM_SIOD | GPIO4 | Línea de datos de configuración SCCB/I2C. |
| CAM_SIOC | GPIO5 | Línea de reloj de configuración SCCB/I2C. |
| CAM_VSYNC | GPIO6 | Sincronismo vertical. |
| CAM_HREF | GPIO7 | Referencia horizontal. |
| CAM_Y4 | GPIO8 | Línea de datos paralelos. |
| CAM_Y3 | GPIO9 | Línea de datos paralelos. |
| CAM_Y5 | GPIO10 | Línea de datos paralelos. |
| CAM_Y2 | GPIO11 | Línea de datos paralelos. |
| CAM_Y6 | GPIO12 | Línea de datos paralelos. |
| CAM_PCLK | GPIO13 | Reloj de píxel. |
| CAM_XCLK | GPIO15 | Reloj externo para la cámara. |
| CAM_Y9 | GPIO16 | Línea de datos paralelos. |
| CAM_Y8 | GPIO17 | Línea de datos paralelos. |
| CAM_Y7 | GPIO18 | Línea de datos paralelos. |

---

## 4. Consecuencia práctica sobre los GPIO

Los GPIO usados por la cámara deben considerarse reservados.

No se utilizarán para ejercicios generales como:

- botones externos,
- LEDs externos,
- sensores analógicos,
- PWM,
- I2C externo,
- SPI externo,
- interrupciones generales,
- pruebas de GPIO aleatorias.

Aunque alguno de estos pines pudiera funcionar si la cámara no se inicializa, el criterio del repositorio será mantenerlos reservados para evitar conflictos futuros.

---

## 5. Señales principales de la OV2640

### XCLK

`XCLK` es el reloj externo que el ESP32 proporciona a la cámara.

En esta placa está asociado a:

```text
GPIO15 -> CAM_XCLK
```

Sin este reloj la cámara no puede funcionar correctamente.

---

### SIOD y SIOC

`SIOD` y `SIOC` son las líneas usadas para configurar internamente la cámara.

Funcionan de forma similar a un bus I2C, aunque en cámaras suele hablarse de SCCB.

```text
GPIO4 -> CAM_SIOD
GPIO5 -> CAM_SIOC
```

A través de estas líneas se configuran parámetros como:

- resolución,
- formato de imagen,
- brillo,
- contraste,
- saturación,
- calidad JPEG,
- orientación,
- ajustes automáticos.

---

### VSYNC, HREF y PCLK

Estas señales sincronizan la transferencia de imagen:

| Señal | Función |
|---|---|
| VSYNC | Indica sincronismo vertical de imagen. |
| HREF | Indica datos válidos en una línea de imagen. |
| PCLK | Marca el ritmo de lectura de píxeles. |

En esta placa:

```text
GPIO6  -> CAM_VSYNC
GPIO7  -> CAM_HREF
GPIO13 -> CAM_PCLK
```

---

### Líneas de datos Y2-Y9

La cámara entrega datos de imagen mediante varias líneas paralelas.

| Señal | GPIO |
|---|---:|
| Y2 | GPIO11 |
| Y3 | GPIO9 |
| Y4 | GPIO8 |
| Y5 | GPIO10 |
| Y6 | GPIO12 |
| Y7 | GPIO18 |
| Y8 | GPIO17 |
| Y9 | GPIO16 |

Estas líneas forman el bus paralelo de datos de la cámara.

---

## 6. PSRAM y memoria

La placa usada en este repositorio incorpora:

```text
8 MB PSRAM
```

La PSRAM es muy importante para trabajar con cámara porque los frame buffers pueden ocupar bastante memoria.

En PlatformIO se debe activar la PSRAM con una configuración equivalente a:

```ini
build_flags =
    -DBOARD_HAS_PSRAM
```

En ejercicios de cámara también se revisará la configuración de particiones, resolución y número de buffers.

---

## 7. Resoluciones habituales

La OV2640 puede trabajar con varias resoluciones. Algunas habituales en ejemplos de ESP32-CAM son:

| Nombre | Resolución aproximada | Uso típico |
|---|---:|---|
| QQVGA | 160 x 120 | Pruebas muy ligeras. |
| QVGA | 320 x 240 | Pruebas iniciales y bajo consumo de memoria. |
| VGA | 640 x 480 | Calidad media. |
| SVGA | 800 x 600 | Mayor calidad, más memoria. |
| UXGA | 1600 x 1200 | Máxima resolución típica de OV2640. |

Para los primeros ejercicios se recomienda comenzar con resoluciones bajas, como QVGA o VGA, para reducir problemas de memoria y rendimiento.

---

## 8. Formatos de imagen

Los formatos más relevantes para los ejercicios serán:

| Formato | Uso |
|---|---|
| JPEG | Captura de imagen comprimida. |
| RGB565 | Procesamiento de imagen simple. |
| YUV | Formato interno o procesamiento avanzado. |

Para servidores web y streaming, lo más habitual será usar JPEG o MJPEG.

---

## 9. Captura frente a streaming

### Captura simple

Una captura simple consiste en:

1. Inicializar la cámara.
2. Solicitar un frame buffer.
3. Obtener una imagen.
4. Usar o enviar la imagen.
5. Liberar el frame buffer.

Este modo será útil para:

- tomar una foto,
- guardar imagen en SD,
- enviar imagen por HTTP,
- capturar imagen bajo evento.

---

### Streaming MJPEG

El streaming MJPEG consiste en enviar muchas imágenes JPEG consecutivas a través de HTTP.

Esto permite visualizar vídeo desde un navegador, aunque realmente se trata de una secuencia rápida de imágenes JPEG.

Este modo exige más a la placa:

- más uso de CPU,
- más uso de memoria,
- más consumo,
- más tráfico WiFi,
- mayor probabilidad de problemas de alimentación.

---

## 10. Consideraciones de alimentación

La cámara y el WiFi pueden aumentar notablemente el consumo de la placa.

Problemas típicos si la alimentación no es suficiente:

- reinicios inesperados,
- fallo al inicializar la cámara,
- errores de captura,
- inestabilidad durante streaming,
- desconexiones WiFi,
- mensajes de brownout o reset.

La documentación específica de alimentación se mantendrá en:

```text
hardware/alimentacion.md
```

---

## 11. Configuración de cámara en Arduino

En Arduino Framework se suele configurar la cámara mediante una estructura de configuración que incluye:

- pines de datos,
- pines de sincronismo,
- resolución,
- calidad JPEG,
- uso de PSRAM,
- número de frame buffers,
- frecuencia de XCLK.

Ejemplo conceptual simplificado:

```cpp
camera_config_t config;

config.pin_sccb_sda = 4;
config.pin_sccb_scl = 5;
config.pin_vsync = 6;
config.pin_href = 7;
config.pin_xclk = 15;
config.pin_pclk = 13;

config.pin_d0 = 11;
config.pin_d1 = 9;
config.pin_d2 = 8;
config.pin_d3 = 10;
config.pin_d4 = 12;
config.pin_d5 = 18;
config.pin_d6 = 17;
config.pin_d7 = 16;
```

> Nota: este fragmento es una referencia conceptual de pines. La configuración completa se desarrollará y validará en los ejercicios de cámara.

---

## 12. Relación con modelos de cámara existentes

En algunos ejemplos de ESP32 se usan macros como:

```cpp
CAMERA_MODEL_ESP32S3_EYE
```

La documentación inicial de la placa indica que el pinout de cámara es compatible con el modelo `CAMERA_MODEL_ESP32S3_EYE` usado en algunos archivos `camera_pins.h`.

Aun así, para este repositorio se documentarán explícitamente los pines propios de la placa, evitando depender únicamente de una macro externa.

---

## 13. Errores frecuentes

### Usar GPIO de cámara para otros periféricos

Incorrecto:

```cpp
#define BUTTON_PIN 4
```

Motivo:

```text
GPIO4 -> CAM_SIOD
```

Ese pin pertenece a la cámara y no debe usarse como botón en este repositorio.

---

### No activar PSRAM

La cámara puede necesitar PSRAM para resoluciones medias o altas.

En PlatformIO debe revisarse:

```ini
build_flags =
    -DBOARD_HAS_PSRAM
```

---

### Empezar con resolución demasiado alta

Para las primeras pruebas no conviene empezar directamente con UXGA.

Es preferible comenzar con resoluciones más pequeñas y subir progresivamente.

---

### Alimentación insuficiente

La cámara puede funcionar en un ejemplo simple y fallar al activar WiFi o streaming.

Esto suele estar relacionado con consumo, estabilidad de alimentación o cable USB de baja calidad.

---

### Olvidar liberar el frame buffer

Cuando se obtiene un frame buffer de la cámara, debe devolverse cuando ya no se necesita.

Conceptualmente:

```cpp
camera_fb_t *fb = esp_camera_fb_get();

// usar fb

esp_camera_fb_return(fb);
```

No liberar buffers puede causar problemas de memoria.

---

## 14. Uso previsto en el repositorio

La cámara se introducirá después de haber trabajado previamente:

- GPIO,
- entradas y salidas digitales,
- interrupciones,
- temporización no bloqueante,
- WiFi,
- HTTP,
- gestión básica de memoria,
- estructura de proyectos PlatformIO.

El objetivo es llegar a la cámara con una base suficiente para entender tanto el hardware como la parte de comunicaciones.

---

## 15. Ejercicios relacionados

Los ejercicios o proyectos relacionados con esta documentación estarán en:

```text
arduino/13_camara_ov2640/
proyectos/camara_ip_basica/
proyectos/sistema_iot_completo/
```

En la fase ESP-IDF se documentarán ejemplos equivalentes en:

```text
esp-idf/07_camera/
```

---

## 16. Estado de validación

| Elemento | Estado |
|---|---|
| Pinout de cámara documentado | Pendiente de validación práctica completa |
| Cámara OV2640 inicializada | No validado todavía |
| Captura JPEG | No validado todavía |
| Streaming MJPEG | No validado todavía |
| Uso de PSRAM con cámara | No validado todavía |
| Compatibilidad de pines con ejercicios futuros | Pendiente de validación |

---

## 17. Relación con otros documentos

Este documento se relaciona con:

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/alimentacion.md
arduino/13_camara_ov2640/
```

Para decidir si un pin puede usarse o no en otro ejercicio, consultar primero:

```text
hardware/gpio_reservados.md
```
