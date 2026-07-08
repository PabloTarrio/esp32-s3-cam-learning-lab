# 004 - Interrupciones, antirrebote y RGB

## Objetivo

Trabajar con interrupciones externas en GPIO3 usando un pulsador externo, comprender el papel de la ISR, usar `volatile`, aplicar antirrebote por temporización y ejecutar una acción sobre el RGB integrado cuando la interrupción sea válida.

## Trabajo realizado

- Configuración de interrupción externa con `attachInterrupt()`.
- Uso de `FALLING` con pulsador configurado mediante `INPUT_PULLUP`.
- Creación de una ISR mínima.
- Comunicación entre ISR y `loop()` mediante una bandera `volatile`.
- Observación de rebote mecánico también en interrupciones.
- Implementación de antirrebote en `loop()` usando `millis()`.
- Validación posterior del estado estable del pin.
- Cambio de color del RGB integrado en `GPIO48` cuando la interrupción es válida.

## Problemas encontrados

### Rebote mecánico

Las interrupciones detectaban varios eventos por una sola pulsación física.

Conclusión:

> Las interrupciones no eliminan el rebote. Solo permiten detectar cambios de forma más rápida.

### Borrado prematuro de la bandera

Inicialmente se limpiaba `button_interrupt_pending` antes de que pasara el tiempo de antirrebote.

Eso provocaba que el evento se perdiera antes de ser validado.

La solución fue mantener la bandera activa hasta completar el proceso:

```text
interrupción -> bandera pendiente -> esperar debounce -> validar pin -> aceptar/descartar -> limpiar bandera
```

## Deciciones tomadas

* Mantener la ISR lo más corta posible.
* No usar `Serial.print()` dentro de la ISR.
* No controlar la RGB dentro de la ISR.
* Procesar el evento en `loop()`
* Usar `volatile` en las variables compartidas con la ISR.
* Usar `FALLING` porque con `INPUT_PULLUP` representa la pulsación.
* Considerar `RISING` cubierto conceptualmente como evento de liberación.

## Conceptos aprendidos.

* `attachInterrupt()`
* `digitalPinToInterrupt()`
* ISR
* `volatile`
* `IRAM_ATTR`
* Flanco descendente `FALLING`
* Flanco ascendente `RISING`
* Antirrebote con interrupciones
* Validación de estado estable tras debounce
* Diferencia entre polling e interrupciones
* Importancia de no hacer trabajo pesado dentro de una ISR

## Resultado

El ejercicio queda validado:

* interrupción en `GPIO3` funcionando;
* pulsador externo con `INPUT_PULLUP`;
* antirrebote por temporización funcionando;
* RGB integrado cambia de color tras interrupción válida;
* comparación polling vs interrupciones documentada.

## Archivos relacionados:
```text
arduino/04_interrupciones/README.md
arduino/04_interrupciones/platformio.ini
arduino/04_interrupciones/src/main.cpp

hardware/pinout.md
hardware/gpio_reservados.md
docs/lecciones_aprendidas.md
docs/buenas_practicas.md
CHANGELOG.md
```