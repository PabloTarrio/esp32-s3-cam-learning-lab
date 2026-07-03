# 002 - Botón GPIO, antirrebote y flancos

## Objetivo

Validar un pulsador externo conectado a GPIO3 y usarlo para cambiar el color del RGB integrado.

## Trabajo realizado

- Conexión de pulsador externo en protoboard.
- Lectura digital con `INPUT_PULLUP`.
- Observación del rebote mecánico.
- Implementación de antirrebote con `millis()`.
- Detección de flanco descendente y ascendente.
- Cambio de color del RGB integrado por pulsación válida.

## Problemas encontrados

- Rebote mecánico inicial.
- Necesidad de distinguir estado y evento.
- Corrección en el uso de variables por referencia.

## Resultado

GPIO3 validado como entrada digital para pulsador externo.

## Conceptos aprendidos

- `INPUT_PULLUP`
- Antirrebote
- Flanco descendente
- Flanco ascendente
- Evento frente a estado
- Variable global frente a variable local
- Paso por referencia