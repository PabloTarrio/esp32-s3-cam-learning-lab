# Alimentación y seguridad eléctrica - ESP32-S3-CAM

Documento de referencia sobre alimentación, niveles eléctricos y precauciones básicas para trabajar con la placa **ESP32-S3-CAM** utilizada en este repositorio.

La alimentación es un aspecto crítico en proyectos con ESP32-S3-CAM, especialmente cuando se combinan cámara, WiFi, tarjeta SD, periféricos externos y funcionamiento continuo.

---

## 1. Resumen

| Elemento | Valor / criterio |
|---|---|
| Tensión lógica de GPIO | 3.3 V |
| Alimentación habitual por USB | 5 V desde USB |
| Alimentación interna del ESP32-S3 | 3.3 V regulados en placa |
| GPIO tolerantes a 5 V | No asumir tolerancia a 5 V |
| Cámara | Incrementa consumo |
| WiFi | Incrementa consumo, especialmente transmitiendo |
| Riesgo habitual | Reinicios, brownout, cámara inestable |

---

## 2. Regla principal

Los GPIO del ESP32-S3 trabajan a **3.3 V**.

No se deben aplicar señales de **5 V** directamente a ningún GPIO.

Esta regla afecta a:

- botones externos,
- sensores digitales,
- sensores analógicos,
- módulos I2C,
- módulos SPI,
- UART externo,
- salidas de placas Arduino de 5 V,
- relés o módulos comerciales con lógica de 5 V.

---

## 3. Alimentación por USB

Durante los primeros ejercicios, la placa se alimentará normalmente desde el puerto USB conectado al ordenador.

Esto permite:

- alimentar la placa,
- programarla desde PlatformIO,
- usar el monitor serie,
- depurar mensajes de arranque,
- trabajar con ejemplos sencillos.

### Precauciones

No todos los cables USB son iguales.

Problemas posibles con cables o puertos USB de baja calidad:

- fallos al subir firmware,
- desconexiones del puerto serie,
- reinicios aparentes aleatorios,
- errores al inicializar la cámara,
- inestabilidad al activar WiFi,
- fallos durante streaming.

Para pruebas con cámara y WiFi conviene usar:

- un cable USB corto y de buena calidad,
- un puerto USB capaz de entregar corriente suficiente,
- preferentemente una conexión directa al ordenador o a una fuente fiable.

---

## 4. Línea de 5 V

La placa puede exponer un pin de `5V` asociado normalmente a la alimentación de entrada o al bus USB.

Este pin no debe confundirse con una señal lógica de GPIO.

Uso típico:

- alimentar la placa desde una fuente externa adecuada,
- alimentar determinados módulos externos si el diseño lo permite,
- tomar alimentación de 5 V para periféricos que lo requieran.

### Advertencia

Aunque un módulo externo se alimente con 5 V, sus señales hacia el ESP32-S3 deben ser compatibles con 3.3 V.

Ejemplo peligroso:

```text
Sensor alimentado a 5 V -> salida digital de 5 V -> GPIO ESP32-S3
```

Esto no debe conectarse directamente.

---

## 5. Línea de 3.3 V

La placa también proporciona una línea de `3V3`.

Esta línea se puede usar para alimentar pequeños periféricos compatibles con 3.3 V, siempre que el consumo sea bajo y razonable.

Ejemplos típicos:

- pulsadores,
- sensores I2C de bajo consumo,
- pequeños módulos digitales de 3.3 V,
- divisores resistivos,
- referencias de nivel lógico.

### Precaución

No se deben alimentar cargas importantes desde el pin `3V3` sin conocer la capacidad del regulador de la placa.

Ejemplos de cargas que pueden ser problemáticas:

- motores,
- servos,
- relés,
- tiras LED,
- módulos GSM,
- módulos WiFi externos,
- periféricos de alto consumo.

---

## 6. Compatibilidad de niveles lógicos

El ESP32-S3 usa lógica de 3.3 V.

Esto significa:

| Señal externa | Conexión directa a GPIO | Comentario |
|---|---|---|
| 0 V / 3.3 V | Sí, normalmente | Compatible. |
| 0 V / 5 V | No | Requiere adaptación de nivel. |
| Señal analógica 0-3.3 V | Sí, si el pin admite ADC | No superar 3.3 V. |
| Señal analógica 0-5 V | No | Requiere divisor o acondicionamiento. |

---

## 7. Adaptación de señales de 5 V

Si un módulo externo entrega señales de 5 V, se debe adaptar el nivel antes de conectarlo al ESP32-S3.

Opciones habituales:

- divisor resistivo,
- conversor de nivel lógico,
- transistor/MOSFET,
- optoacoplador,
- módulo compatible directamente con 3.3 V.

### Divisor resistivo básico

Para señales digitales lentas o entradas analógicas simples se puede usar un divisor resistivo.

Ejemplo conceptual:

```text
Señal 5 V ----[ R1 ]----+---- GPIO ESP32-S3
                        |
                       [ R2 ]
                        |
                       GND
```

La tensión en el GPIO será:

```text
Vgpio = Vin * R2 / (R1 + R2)
```

Para bajar de 5 V a aproximadamente 3.3 V se pueden usar valores como:

```text
R1 = 10 kΩ
R2 = 20 kΩ
```

Resultado aproximado:

```text
Vgpio = 5 V * 20 kΩ / (10 kΩ + 20 kΩ) ≈ 3.33 V
```

---

## 8. Botones externos

Para botones externos se recomienda empezar usando `INPUT_PULLUP`.

Esquema típico:

```text
GPIO ---- Pulsador ---- GND
```

Configuración:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

Lectura esperada:

| Estado físico | Lectura GPIO |
|---|---|
| Botón sin pulsar | HIGH |
| Botón pulsado | LOW |

### Ventajas

- No requiere resistencia externa.
- Cableado simple.
- Es adecuado para ejercicios iniciales.

### Precaución

El pin elegido para el botón debe estar confirmado como libre o ser un botón integrado ya documentado.

---

## 9. Sensores analógicos

Las entradas ADC del ESP32-S3 no deben recibir más de 3.3 V.

Para sensores analógicos:

- comprobar tensión máxima de salida,
- comprobar si el sensor se alimenta a 3.3 V o 5 V,
- adaptar la señal si supera 3.3 V,
- evitar conectar señales industriales directamente.

### Señales industriales

Señales típicas de automatización como:

```text
0-10 V
4-20 mA
24 V digital
```

no deben conectarse directamente al ESP32-S3.

Requieren acondicionamiento eléctrico mediante circuitos adecuados.

---

## 10. Relés, motores y cargas inductivas

No se deben conectar cargas inductivas directamente a un GPIO.

Ejemplos de cargas inductivas:

- relés,
- motores DC,
- solenoides,
- contactores,
- electroválvulas.

Un GPIO del ESP32-S3 solo puede entregar una corriente pequeña y no está diseñado para alimentar cargas.

Para controlar cargas se deben usar elementos intermedios:

- transistor,
- MOSFET,
- driver,
- optoacoplador,
- módulo de relé compatible,
- fuente externa adecuada.

En cargas inductivas debe considerarse protección contra transitorios, por ejemplo diodo flyback en cargas DC.

---

## 11. Cámara y consumo

La cámara OV2640 incrementa el consumo de la placa.

El consumo puede aumentar especialmente durante:

- inicialización de cámara,
- captura de imagen,
- uso de resoluciones altas,
- streaming MJPEG,
- funcionamiento simultáneo con WiFi.

Problemas típicos relacionados con alimentación:

- fallo al inicializar la cámara,
- frames corruptos,
- reinicios durante streaming,
- desconexiones WiFi,
- comportamiento inestable tras varios segundos de uso.

---

## 12. WiFi y consumo

El WiFi del ESP32-S3 puede generar picos de consumo, especialmente durante transmisión.

Esto es importante en ejercicios como:

- conexión WiFi STA,
- servidor HTTP,
- streaming de cámara,
- publicación MQTT,
- OTA,
- proyectos con comunicación continua.

Si un ejercicio funciona sin WiFi pero falla al activarlo, una posible causa es alimentación insuficiente o inestable.

---

## 13. Brownout y reinicios

Un problema típico en placas ESP32 es el reinicio por caída de tensión.

Síntomas posibles:

- mensajes de reinicio en el monitor serie,
- arranques repetidos,
- fallo al activar WiFi,
- fallo al inicializar cámara,
- desconexión del puerto USB,
- comportamiento aparentemente aleatorio.

Causas habituales:

- cable USB deficiente,
- fuente con poca corriente,
- consumo excesivo de periféricos,
- alimentación desde un puerto USB débil,
- picos de corriente al activar radio o cámara.

---

## 14. Buenas prácticas de alimentación

Recomendaciones generales:

- Usar cables USB cortos y de calidad.
- Evitar alimentar cargas externas desde la placa sin calcular consumo.
- Mantener masa común cuando se usen fuentes externas y señales compartidas.
- No aplicar 5 V a GPIO.
- Comprobar niveles lógicos de módulos externos.
- Separar alimentación de cargas de potencia y lógica cuando sea necesario.
- Añadir condensadores de desacoplo en montajes inestables.
- Empezar con montajes simples y añadir periféricos gradualmente.

---

## 15. Alimentación externa

Si se alimenta la placa desde una fuente externa, se debe comprobar:

- tensión correcta,
- polaridad,
- corriente disponible,
- punto exacto de entrada de alimentación,
- conexión de GND común con otros módulos,
- ausencia de conflictos con alimentación USB simultánea.

### Advertencia

No se debe alimentar la placa simultáneamente desde distintas fuentes sin entender cómo está diseñada la alimentación de la placa.

Algunas placas permiten ciertas combinaciones; otras pueden dañarse o comportarse de forma inestable.

---

## 16. GND común

Cuando se conecta un periférico externo alimentado por otra fuente, normalmente debe existir una referencia común de tierra.

Ejemplo:

```text
Fuente externa GND ---- GND ESP32-S3-CAM
Señal periférico ------ GPIO ESP32-S3-CAM
```

Sin GND común, las señales pueden no tener una referencia válida y las lecturas pueden ser erráticas.

### Excepción

En sistemas aislados mediante optoacopladores u otros aislamientos galvánicos, la conexión de tierra puede no ser necesaria o puede seguir otro criterio de diseño.

---

## 17. Uso con protoboard

Para los primeros ejercicios con protoboard:

- usar cables cortos,
- evitar falsos contactos,
- revisar dos veces GND y 3V3,
- no mezclar 5 V y 3.3 V sin etiquetar,
- desconectar alimentación antes de modificar cableado,
- medir tensiones si hay dudas.

---

## 18. Comprobaciones antes de alimentar

Antes de conectar alimentación a un montaje externo:

- verificar polaridad,
- verificar que no hay cortocircuitos evidentes,
- comprobar que el GPIO elegido es correcto,
- comprobar que la señal no supera 3.3 V,
- verificar que GND está conectado donde corresponde,
- revisar que no se usan pines reservados.

---

## 19. Errores frecuentes

### Conectar 5 V a un GPIO

Error grave.

El ESP32-S3 trabaja con señales de 3.3 V y no debe recibir 5 V directamente en sus GPIO.

---

### Alimentar motores desde el pin 3V3

No recomendable.

Los motores requieren más corriente y generan ruido eléctrico.

---

### Usar módulos de relé sin comprobar nivel lógico

Algunos módulos de relé se alimentan a 5 V y pueden no ser compatibles directamente con lógica de 3.3 V.

---

### No unir GND entre sistemas

Si un sensor externo tiene alimentación independiente y envía una señal al ESP32-S3, normalmente debe compartir GND con la placa.

---

### Pensar que todos los pines del header son GPIO libres

En una ESP32-S3-CAM muchos pines están ocupados por cámara, memoria, USB, SD, RGB o funciones especiales.

---

## 20. Estado de validación

| Elemento | Estado |
|---|---|
| Alimentación por USB para ejercicios básicos | Validado |
| Uso del monitor serie por USB | Validado |
| Control RGB integrado por USB | Validado |
| Alimentación con cámara activa | No validado todavía |
| Alimentación con WiFi activo | No validado todavía |
| Alimentación con cámara + WiFi + streaming | No validado todavía |
| Alimentación externa | No validado todavía |

---

## 21. Relación con otros documentos

Este documento se relaciona con:

```text
hardware/pinout.md
hardware/gpio_reservados.md
hardware/camara_ov2640.md
hardware/rgb_neopixel.md
```

Para decidir si un pin se puede usar en un montaje externo, consultar primero:

```text
hardware/gpio_reservados.md
```

Para ejercicios con cámara, consultar también:

```text
hardware/camara_ov2640.md
```
