# Tarjeta SD / SDMMC - ESP32-S3-CAM

Documento de referencia sobre el posible uso de tarjeta SD en la placa **ESP32-S3-CAM** utilizada en este repositorio.

La información de este documento debe considerarse una referencia técnica inicial. El uso real de la tarjeta SD debe validarse experimentalmente sobre la placa concreta, ya que algunas variantes de ESP32-S3-CAM pueden exponer pines similares pero no montar físicamente el mismo slot o el mismo cableado.

---

## 1. Resumen

| Elemento | Valor / criterio |
|---|---|
| Periférico | Tarjeta SD / microSD |
| Interfaz prevista | SDMMC en modo 1-bit, según documentación inicial de la placa |
| Pines asociados | GPIO38, GPIO39, GPIO40 |
| Uso previsto | Guardar imágenes, logs, configuración o datos de sensores |
| Estado | Pendiente de validación práctica |
| Framework inicial | Arduino Framework sobre PlatformIO |
| Framework avanzado previsto | ESP-IDF |

---

## 2. Pines asociados a SD

Según el pinout de referencia de la placa, la tarjeta SD puede estar asociada a los siguientes GPIO:

| GPIO | Función SD | Recomendación |
|---:|---|---|
| GPIO38 | SD_CMD | Reservar para SD. |
| GPIO39 | SD_CLK | Reservar para SD. |
| GPIO40 | SD_DATA | Reservar para SD. |

Estos pines no deben usarse para ejercicios generales mientras se mantenga la posibilidad de utilizar tarjeta SD en el futuro.

---

## 3. Modo de bus previsto

La documentación inicial de la placa indica un uso de SDMMC en modo de bus de 1 bit.

En un bus SDMMC de 1 bit se suelen utilizar señales equivalentes a:

| Señal | Función |
|---|---|
| CMD | Línea de comando. |
| CLK | Reloj. |
| DATA0 | Línea de datos. |

En esta placa, la correspondencia documentada es:

```text
GPIO38 -> SD_CMD
GPIO39 -> SD_CLK
GPIO40 -> SD_DATA
```

---

## 4. Diferencia entre SDMMC y SPI

Las tarjetas SD pueden usarse habitualmente mediante dos enfoques:

1. **SDMMC**
2. **SPI**

### SDMMC

SDMMC es una interfaz específica para tarjetas SD. Puede usar varios anchos de bus, por ejemplo 1 bit o 4 bits.

Ventajas:

- Mejor rendimiento potencial.
- Interfaz pensada específicamente para SD.
- Muy habitual en placas ESP32 con slot SD integrado.

Inconvenientes:

- Pines menos flexibles.
- Puede entrar en conflicto con otros periféricos.
- Requiere que el cableado de la placa coincida con la configuración usada.

---

### SPI

SPI es un bus más genérico que también puede usarse con tarjetas SD mediante adaptadores o módulos externos.

Ventajas:

- Pines más configurables.
- Fácil de entender si ya se conoce SPI.
- Útil con módulos SD externos.

Inconvenientes:

- Normalmente menor rendimiento que SDMMC.
- Requiere señales adicionales como CS.
- Puede compartir bus con otros dispositivos SPI y requerir más gestión.

---

## 5. Criterio del repositorio

Para esta placa se asumirá inicialmente que la SD integrada, si se usa, se gestionará mediante SDMMC en modo 1-bit.

Por ese motivo:

```text
GPIO38
GPIO39
GPIO40
```

quedan reservados para SD hasta que se demuestre lo contrario.

No se usarán como:

- GPIO genéricos,
- botones,
- LEDs,
- ADC,
- PWM,
- I2C,
- SPI externo,
- interrupciones generales.

---

## 6. Uso típico en proyectos

La tarjeta SD será útil para:

- guardar imágenes capturadas por la cámara,
- guardar logs,
- guardar configuración,
- almacenar datos de sensores,
- almacenar ficheros HTML/CSS/JS para un servidor web,
- registrar eventos de un sistema IoT,
- guardar capturas bajo demanda o por evento.

---

## 7. Relación con la cámara

La tarjeta SD y la cámara tienen una relación natural en proyectos ESP32-CAM.

Ejemplos de uso:

- capturar imagen con OV2640,
- obtener un frame JPEG,
- crear un nombre de archivo,
- guardar el JPEG en la tarjeta SD,
- registrar la fecha o evento asociado,
- servir imágenes guardadas mediante HTTP.

Este flujo exige coordinar correctamente:

- cámara,
- PSRAM,
- sistema de archivos,
- alimentación,
- WiFi si la imagen se consulta desde red.

---

## 8. Consideraciones de alimentación

El uso de tarjeta SD puede incrementar el consumo del sistema, especialmente durante escritura.

Cuando se combine:

- cámara,
- WiFi,
- tarjeta SD,
- streaming,
- escritura frecuente,

pueden aparecer problemas de alimentación si el cable USB o la fuente no son suficientes.

Síntomas posibles:

- reinicios durante escritura,
- errores al montar la SD,
- fallos al guardar archivos,
- corrupción de datos,
- errores intermitentes,
- desconexiones durante streaming.

La alimentación se documenta con más detalle en:

```text
hardware/alimentacion.md
```

---

## 9. Consideraciones sobre archivos

Al trabajar con SD se deben tener en cuenta algunos criterios:

- evitar escribir continuamente sin necesidad,
- cerrar correctamente los archivos,
- comprobar si la escritura ha sido correcta,
- evitar nombres de archivo ambiguos,
- evitar sobrescribir datos importantes sin querer,
- considerar el desgaste de la tarjeta si se escriben logs con mucha frecuencia,
- retirar la alimentación solo cuando no haya escrituras en curso.

---

## 10. Montaje de SD en Arduino

En Arduino Framework, para SDMMC suele utilizarse una librería como:

```cpp
#include "SD_MMC.h"
```

Inicialización conceptual:

```cpp
if (!SD_MMC.begin())
{
    Serial.println("Fallo al montar la tarjeta SD");
    return;
}

Serial.println("Tarjeta SD montada correctamente");
```

> Nota: este fragmento es conceptual. La inicialización exacta debe validarse con la placa y el modo de bus correcto.

---

## 11. Ejemplo conceptual de escritura

Ejemplo simplificado de escritura de texto:

```cpp
File file = SD_MMC.open("/log.txt", FILE_APPEND);

if (!file)
{
    Serial.println("No se pudo abrir el archivo");
    return;
}

file.println("Evento registrado");
file.close();
```

Este tipo de ejemplo se desarrollará de forma completa cuando se trabaje la fase de almacenamiento.

---

## 12. Ejemplo conceptual de guardado de imagen

Flujo conceptual para guardar una imagen capturada por cámara:

```cpp
camera_fb_t *fb = esp_camera_fb_get();

if (!fb)
{
    Serial.println("Error capturando imagen");
    return;
}

File file = SD_MMC.open("/foto.jpg", FILE_WRITE);

if (file)
{
    file.write(fb->buf, fb->len);
    file.close();
}

esp_camera_fb_return(fb);
```

Aspectos importantes:

- comprobar que la cámara devuelve un frame válido,
- comprobar que el archivo se abre correctamente,
- escribir exactamente `fb->len` bytes,
- cerrar el archivo,
- devolver el frame buffer con `esp_camera_fb_return()`.

---

## 13. Errores frecuentes

### Usar pines SD como GPIO generales

Incorrecto:

```cpp
#define BUTTON_PIN 38
```

Motivo:

```text
GPIO38 -> SD_CMD
```

Aunque el botón pudiera funcionar en una prueba aislada, se estaría ocupando un pin reservado para la SD.

---

### No comprobar si la SD está montada

No se debe asumir que la tarjeta está disponible.

Siempre debe comprobarse el resultado de la inicialización.

---

### No cerrar archivos

Si no se llama a:

```cpp
file.close();
```

pueden perderse datos o quedar escrituras incompletas.

---

### Escribir durante una caída de alimentación

Interrumpir alimentación durante una escritura puede corromper archivos o el sistema de archivos.

---

### Usar tarjetas de mala calidad

Tarjetas SD defectuosas, muy antiguas o de baja calidad pueden producir errores difíciles de diagnosticar.

---

## 14. Buenas prácticas

- Reservar GPIO38, GPIO39 y GPIO40 para SD.
- Comprobar siempre si la SD se monta correctamente.
- Comprobar si cada archivo se abre correctamente.
- Cerrar los archivos después de escribir.
- No escribir en bucles muy rápidos sin necesidad.
- Añadir mensajes de diagnóstico por serie.
- Usar nombres de archivo claros.
- Evitar retirar alimentación durante escrituras.
- Comprobar alimentación si aparecen errores intermitentes.

---

## 15. Estado de validación

| Elemento | Estado |
|---|---|
| Pines SD documentados | Pendiente de validación práctica |
| Montaje SD con `SD_MMC` | No validado todavía |
| Escritura de archivo de texto | No validado todavía |
| Lectura de archivo de texto | No validado todavía |
| Guardado de imagen JPEG | No validado todavía |
| Uso combinado cámara + SD | No validado todavía |
| Uso combinado cámara + SD + WiFi | No validado todavía |

---

## 16. Relación con otros documentos

Este documento se relaciona con:

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/camara_ov2640.md
hardware/alimentacion.md
```

También se relacionará con futuros ejercicios o proyectos de almacenamiento, cámara y servidor web.
