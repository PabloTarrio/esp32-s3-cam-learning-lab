# Ejercicio 03 - Botón RGB Estado

Ejercicio de consolidación de la Fase 2: GPIO digital básico.

Este ejercicio combina lectura de pulsador externo, antirrebote por software, detección de flanco y control del LED RGB integrado de la placa ESP32-S3-CAM.

---

## Objetivo

Consolidar los conceptos trabajados hasta ahora:

- lectura digital de un pulsador externo;
- uso de `INPUT_PULLUP`;
- antirrebote por software con `millis()`;
- detección de flanco ascendente;
- cambio de estado;
- control del LED RGB integrado;
- uso de funciones con paso por referencia;
- separación básica de responsabilidades.

En este ejercicio el color del LED RGB cambia cuando se detecta el **flanco ascendente** del pulsador, es decir, cuando el botón se libera.

---

## Hardware utilizado

- ESP32-S3-CAM.
- Pulsador externo.
- Protoboard.
- Cables Dupont.
- LED RGB integrado NeoPixel / WS2812 en `GPIO48`.

---

## Cableado

El pulsador externo se conecta entre `GPIO3` y `GND`.

```text
ESP32-S3-CAM                         PROTOBOARD

GPIO3  ------------------------------┐
                                     │
                                  [ PULSADOR ]
                                     │
GND    ------------------------------┘
```

## Resultado esperado
```text
Ejercicio 03 - Boton RGB Estado
GPIO Boton: 3
GPIO RGB: 48
```
