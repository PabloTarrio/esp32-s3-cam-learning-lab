# Hardware - ESP32-S3-CAM

Documentación hardware de la placa **ESP32-S3-CAM** utilizada en este repositorio.

Esta carpeta reúne la información necesaria para trabajar de forma segura y ordenada con la placa antes de usar GPIO, cámara, alimentación, periféricos o buses de comunicación.

---

## Objetivo de esta carpeta

El objetivo de `hardware/` es centralizar toda la información física y eléctrica de la placa:

- Pinout general.
- GPIO disponibles y reservados.
- LED RGB integrado.
- Cámara OV2640.
- Alimentación.
- Periféricos internos.
- Precauciones eléctricas.
- Notas obtenidas durante las pruebas reales.

Esta documentación es especialmente importante porque la placa **no debe tratarse como un ESP32-S3 genérico**. Al tener cámara, PSRAM, Flash, LED RGB, botones y otros periféricos integrados, algunos GPIO están ocupados o tienen restricciones.

---

## Placa utilizada

- **Placa:** ESP32-S3-CAM
- **Módulo principal:** ESP32-S3-WROOM-1 N16R8
- **Flash:** 16 MB
- **PSRAM:** 8 MB
- **Cámara:** OV2640
- **LED RGB integrado:** NeoPixel / WS2812
- **GPIO del LED RGB:** GPIO48
- **Entorno de desarrollo principal:** PlatformIO
- **Framework inicial:** Arduino Framework
- **Framework avanzado previsto:** ESP-IDF

---

## Archivos de esta carpeta

```text
hardware/
├── README.md
├── pinout.md
├── gpio_reservados.md
├── rgb_neopixel.md
├── camara_ov2640.md
├── alimentacion.md
└── SD.md
```

---

## Descripción de archivos

### [`pinout.md`](./pinout.md)

Documento principal de referencia de pines.

Debe incluir:

- Tabla general de GPIO.
- Funciones asociadas a cada pin.
- Pines usados por la cámara.
- Pines usados por periféricos internos.
- Pines recomendados para pruebas.
- Pines que deben evitarse.

---

### [`gpio_reservados.md`](./gpio_reservados.md)

Documento específico para identificar pines que no deben usarse libremente.

Debe incluir:

- GPIO usados por la cámara OV2640.
- GPIO asociados a Flash y PSRAM.
- GPIO asociados a SD si aplica.
- GPIO asociados a USB, UART o boot.
- GPIO del LED RGB integrado.
- GPIO de botones integrados.

Este archivo debe consultarse antes de crear cualquier ejercicio nuevo que use GPIO.

---

### [`rgb_neopixel.md`](./rgb_neopixel.md)

Documento dedicado al LED RGB integrado.

Debe incluir:

- Tipo de LED: NeoPixel / WS2812.
- GPIO utilizado: GPIO48.
- Librerías usadas.
- Diferencias frente a un LED digital normal.
- Ejemplos mínimos.
- Problemas encontrados.
- Notas sobre brillo y color.

---

### [`camara_ov2640.md`](./camara_ov2640.md)

Documento dedicado a la cámara OV2640.

Debe incluir:

- GPIO usados por la cámara.
- Configuración inicial.
- Relación con PSRAM.
- Resoluciones soportadas.
- Consideraciones de memoria.
- Errores frecuentes.
- Ejemplos de captura y streaming cuando se desarrollen.

---

### [`alimentacion.md`](./alimentacion.md)

Documento dedicado a alimentación y seguridad eléctrica.

Debe incluir:

- Tensiones de alimentación aceptadas.
- Uso de 5 V y 3.3 V.
- Limitaciones de corriente.
- Precauciones al conectar periféricos externos.
- Advertencias sobre señales de 5 V en GPIO.
- Problemas típicos de alimentación con cámara y WiFi.

### [`SD.md`](./SD.md)

Documento dedicado a tarjeta de memoria SD

Debe incluir:

- Pines asociados a SD: GPIO38, GPIO39, GPIO40
- Diferencia entre SDMC y SPI.
- Criterio de reserva de pines.
- Relación con la cámara.
- Consideraciones de alimentación.
- Uso conceptual con SD_MMC.h
- Ejemplo conceptual de escritura.
- Ejemplo conceptual de guardado de imagen.
- Errores frecuentes.
- Buenas prácticas.
- Estado de Validación.

---

## Reglas de uso de GPIO

Antes de usar cualquier GPIO se debe comprobar:

1. Si está usado por la cámara.
2. Si está relacionado con Flash o PSRAM.
3. Si está asociado a SD, USB, UART o boot.
4. Si está conectado a algún periférico integrado.
5. Si está realmente expuesto físicamente en la placa.
6. Si admite la función deseada: entrada, salida, ADC, PWM, I2C, SPI, etc.

Regla general:

> Si un GPIO no está documentado como libre, no se debe usar todavía.

---

## Precauciones eléctricas

- Los GPIO del ESP32-S3 trabajan a **3.3 V**.
- No se deben aplicar señales de **5 V** directamente a los GPIO.
- Si se conectan sensores o módulos de 5 V, debe comprobarse si sus salidas son compatibles con 3.3 V.
- Para señales de 5 V puede ser necesario usar divisores resistivos, conversores de nivel o interfaces adecuadas.
- La cámara y el WiFi pueden aumentar el consumo de corriente, por lo que la alimentación debe ser estable.

---

## Recursos visuales relacionados

Las imágenes, diagramas y esquemas no deben guardarse directamente en esta carpeta, sino en `assets/`.

Uso recomendado:

```text
assets/images/       -> fotos de placa, capturas, imágenes generales
assets/diagrams/     -> diagramas conceptuales
assets/schematics/   -> esquemas eléctricos y cableados
```

Ejemplo:

```markdown
![Placa ESP32-S3-CAM](../assets/images/esp32_s3_cam_board.jpg)
```

---

## Estado actual de la documentación hardware

### Confirmado en pruebas

- El entorno PlatformIO compila y sube firmware correctamente.
- El monitor serie funciona correctamente.
- El LED RGB integrado se controla como NeoPixel / WS2812.
- El LED RGB integrado está conectado a GPIO48.
- El control de brillo requiere actualizar brillo, color y ejecutar `show()` para garantizar que el cambio se refleje correctamente.

### Pendiente de documentar

- Pinout completo revisado.
- GPIO realmente libres para prácticas.
- Botones integrados.
- Pines exactos de la cámara OV2640.
- Pines asociados a SD si aplica.
- Recomendaciones de alimentación.
- Esquemas de cableado para ejercicios básicos.

---

## Relación con los ejercicios

Antes de crear un ejercicio que use hardware externo, debe revisarse esta carpeta.

Ejemplos:

- `arduino/01_rgb_neopixel/` debe apoyarse en `hardware/rgb_neopixel.md`.
- `arduino/02_boton_gpio/` debe apoyarse en `hardware/gpio_reservados.md` y `hardware/pinout.md`.
- Los ejercicios de cámara deben apoyarse en `hardware/camara_ov2640.md`.
- Los ejercicios con sensores externos deben consultar `hardware/alimentacion.md`.

---

## Notas de trabajo

Esta documentación debe actualizarse cada vez que se descubra o confirme información nueva sobre la placa.

Ejemplos de información que debe añadirse:

- Un GPIO probado como entrada correctamente.
- Un GPIO descartado por conflicto.
- Un error de cámara relacionado con PSRAM.
- Un problema de alimentación.
- Una incompatibilidad con un sensor externo.
- Un esquema de cableado validado.
