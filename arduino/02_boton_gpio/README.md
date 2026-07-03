# Ejercicio 02 -Botón GPIO

## Objetivo

Leer un pulsador externo conectado a GPIO3, aplicar antirrebote por software, detectar flancos y cambiar el color del LED RGB integrado con cada pulsación válida.

---

## Hardware utilizado

* ESP32-S3-CAM
* Pulsador externo
* Protoboard
* Cables Dupont
* LED RGB integrado en GPIO48

---

## Cableado

Este ejercicio usa un pulsador externo conectado en protoboard.

Se utiliza la resistencia `pull-up` interna del ESP32-S3, por lo que no es necesaria una resistencia externa.

```text
ESP32-S3-CAM                         PROTOBOARD

GPIO_3  -----------------------------┐
                                     │
                                  [ PULSADOR ]
                                     │
GND     -----------------------------┘
```

Se usa la resistenica pull-up interna

```text
pinMode (BUTTON_PIN, INPUT_PULLUP);
```

Lógica esperada

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
```

---

## Funcionamiento

El programa:

1. Lee el pulsador en GPIO3.
2. Filtra el rebote mecánico mediante `millis()`
3. Detecta flanco descendente `HIGH -> LOW`.
4. Cambia el color del LED RGB integrado en cada pulsación válida.
5. Imprime eventos por el monitor serie.
   
---

## Resultado esperado

Al pulsar:

```text
Flanco descendente: Pulsación Válida
Color index: X
```

Al soltar:
```text
Flanco ascendente: Pulsador liberado
```
El RGB cambia de colr una sola vez por pulsación válida.

---

## Documentación relacionada

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/rgb_neopixel.md
docs/lecciones_aprendidas.md
docs/buenas_practicas.md
```

---

## Estado del ejercicio

| Elemento | Estado |
|---|---|
| Lectura GPIO3 | Validado |
| INPUT_PULLUP | Validado |
| Antirrebote software | Validado |
| Flanco descendente | Validado |
| Flanco ascendente | Validado |
| Control de color RGB | Validado |

---