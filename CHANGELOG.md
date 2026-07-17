# CHANGELOG

Todos los cambios relevantes de este repositorio se documentarán en este archivo.

El formato está inspirado en [Keep a Changelog](https://keepachangelog.com/) y el versionado seguirá, cuando tenga sentido, una aproximación compatible con [Semantic Versioning](https://semver.org/).

---

## [Unreleased]

### Added

- Añadida licencia MIT en `LICENSE`.
- Añadida documentación base en `docs/`:
  - `docs/lecciones_aprendidas.md`
  - `docs/buenas_practicas.md`
  - `docs/glosario.md`
- Añadida documentación inicial de hardware en `hardware/`:
  - `hardware/README.md`
  - `hardware/pinout.md`
  - `hardware/gpio_reservados.md`
  - `hardware/rgb_neopixel.md`
  - `hardware/camara_ov2640.md`
  - `hardware/alimentacion.md`
  - `hardware/SD.md`
- Añadida imagen de referencia del pinout en `assets/images/` y enlazada desde la documentación de hardware.
- Añadida documentación del primer ejercicio Arduino:
  - `arduino/01_rgb_neopixel/README.md`
- Añadida configuración PlatformIO del primer ejercicio:
  - `arduino/01_rgb_neopixel/platformio.ini`
- Añadido código del primer ejercicio:
  - `arduino/01_rgb_neopixel/src/main.cpp`
- Añadido ejercicio Arduino `02_boton_gpio`:
  - lectura de pulsador externo en `GPIO3`;
  - uso de `INPUT_PULLUP`;
  - antirrebote por software con `millis()`;
  - detección de flanco descendente y ascendente;
  - cambio de color del LED RGB integrado en cada pulsación válida.
- Added Arduino exercise `03_boton_rgb_estado`.
  - Added consolidation exercise using external button on GPIO3 and RGB NeoPixel on GPIO48.
  - Added RGB color change on button release using rising edge detection.
- Añadido ejercicio Arduino `04_interrupciones`.
  - Añadidad detección de interrupción externa en `GPIO3` usando `attachInterrupt()`.
  - Añadida interrupción por flanco descendente `FALLING` con pulsador externo y `INPUT_PULLUP`.
  - Añadido uso de variable `volatile` para comunicar la ISR con `loop()`.
  - Añadido anti-rebote de interrupciones mediante temporización con `millis()`.
  - Añadido cambio de color RGB integrado mediante interrupción válida.
- Añadido ejercicio Arduino `05_millis_no_bloqueante`.
  - Añadido primer ejemplo de temporización no bloqueante usando `millis()`.
  - Añadido parpadeo del LED RGB integrado en `GPIO48` sin usar `delay()`.
  - Añadida documentación del ejercicio `arduino/05_millis_no_bloqueante/`.
- Añadido ejercicio Arduino `05b_millis_rgb_boton`.
  - Añadido ejemplo de ejecución cooperativa en `loop()` combinando RGB y pulsador.
  - Añadido cambio de modo mediante pulsador externo en `GPIO3`.
  - Añadido parpadeo del RGB integrado en `GPIO48` sin usar `delay()`.
  - Añadida documentación del ejercicio `arduino/05b_millis_rgb_boton/`.
- Añadido ejercicio Arduino `05c_tareas_cooperativas`.
  - Añadido ejemplo de varias tareas cooperativas ejecutándose en `loop()` sin usar `delay()`.
  - Añadida impresión periódica por puerto serie usando `millis()`.
  - Añadida gestión simultánea de:
  - pulsador externo en `GPIO3`;
  - RGB integrado en `GPIO48`;
  - cambio de modo;
  - salida periódica por `Serial`.
- Añadido ejercicio Arduino `06_maquina_estado_basica`.
- Añadida primera máquina de estados básica usando `enum`.
- Añadida separación entre:
  - detección de evento;
  - transición de estado;
  - ejecución del estado actual.
- Añadido control del RGB integrado en `GPIO48` según estado del sistema.
- Añadida impresión periódica del estado actual por monitor serie usando nombres de estado.
- Añadido ejercicio Arduino `07_temporizador_software_simple`.
- Añadida función reutilizable `temporizador_cumplido()` para temporización no bloqueante.
- Añadido uso de paso por referencia para actualizar marcas temporales.
- Añadida reutilización de `temporizador_cumplido()` en:
  - parpadeo RGB;
  - impresión periódica por monitor serie.
- Añadida evolución desde temporización manual con `millis()` hacia una abstracción reutilizable.
- Añadido ejercicio Arduino `08_ton_basico`.
- Añadida primera implementación de un temporizador tipo `TON` inspirado en PLC.
- Añadida clase C++ `TON` con estado interno.
- Añadidos métodos:
  - `update(bool in, unsigned long pt)`;
  - `Q()`;
  - `ET()`.
- Añadida prueba práctica usando pulsador externo en `GPIO3` como entrada `IN`.
- Añadido control del RGB integrado en `GPIO48` mediante la salida `Q` del temporizador.
- Añadido ejercicio Arduino `09_tof_basico`.
- Añadida implementación de un temporizador tipo `TOF` inspirado en PLC.
- Añadida clase C++ `TOF` con estado interno.
- Añadidos métodos:
  - `update(bool in, unsigned long pt)`;
  - `Q()`;
  - `ET()`.
- Añadida prueba práctica usando pulsador externo en `GPIO3` como entrada `IN`.
- Añadido control del RGB integrado en `GPIO48` mediante la salida `Q` del temporizador.
- Añadido ejemplo visual de retardo a la desconexión: el RGB permanece encendido tras soltar el pulsador.
- Añadido ejercicio Arduino `10_tp_basico`.
- Añadida implementación de un temporizador tipo `TP` inspirado en PLC.
- Añadida clase C++ `TP` con estado interno.
- Añadidos métodos:
  - `update(bool in, unsigned long pt)`;
  - `Q()`;
  - `ET()`.
- Añadida detección interna de flanco ascendente mediante `previous_in`.
- Añadida prueba práctica usando pulsador externo en `GPIO3` como entrada `IN`.
- Añadido control del RGB integrado en `GPIO48` mediante la salida `Q` del temporizador.
- Añadido ejemplo visual de pulso temporizado: el RGB permanece encendido durante `PT` tras una pulsación.
- Añadido ejercicio Arduino `11_rtrig_ftrig_basico`.
- Añadida implementación de detectores de flanco inspirados en PLC.
- Añadida clase C++ `R_TRIG` para detectar flanco ascendente.
- Añadida clase C++ `F_TRIG` para detectar flanco descendente.
- Añadida detección de pulsación mediante `R_TRIG`.
- Añadida detección de liberación mediante `F_TRIG`.
- Añadida prueba práctica usando pulsador externo en `GPIO3`.
- Añadida transformación de lectura física `INPUT_PULLUP` a señal lógica booleana `boton`.

### Changed

- Reducido el `README.md` del ejercicio `arduino/01_rgb_neopixel/` para evitar duplicar la explicación detallada del código.
- Adoptado el criterio de que la explicación pedagógica del código debe estar principalmente en comentarios dentro de `src/main.cpp`.
- Ajustados documentos técnicos para evitar secciones de planificación diaria como “próximos pasos”.
- Corregida la codificación del archivo `main.cpp` a UTF-8.
- Actualizado `hardware/pinout.md`:
  - conversión a tablas Markdown limpias;
  - corrección de numeración de secciones;
  - actualización de `GPIO3` como entrada digital validada;
  - actualización de bitácora de validación.
- Actualizado `hardware/gpio_reservados.md`:
  - conversión a tablas Markdown limpias;
  - actualización de `GPIO3` como GPIO validado para pulsador externo;
  - mantenimiento de `GPIO46` como candidato pendiente de validación;
  - actualización de tabla resumen y bitácora.
- Actualizado `docs/lecciones_aprendidas.md`:
  - limpieza general de formato Markdown;
  - nuevas lecciones sobre antirrebote, flancos y pulsador externo;
  - nuevas lecciones de C++ sobre paso por referencia y uso correcto de parámetros.


### Documented

- Documentada la regla de usar `assets/` para imágenes, diagramas y esquemas.
- Documentada la reserva de GPIO usados por cámara, SD, UART, USB, PSRAM, Flash y RGB integrado.
- Documentado el uso de `GPIO48` para el LED RGB integrado NeoPixel / WS2812.
- Documentado el criterio de no usar GPIO de cámara para ejercicios generales.
- Documentado el uso de `Adafruit_NeoPixel` para el RGB integrado.
- Documentada la separación entre documentación estable, roadmap, changelog y journal.
- Documentado el cableado externo del pulsador con protoboard:
- Documentado el ejercicio `arduino/04_interrupciones/`.
- Documentada la diferencia entre `FALLING` y `RISING` con `INPUT_PULLUP`.
- Documentada la comparación entre polling e interrupciones.
- Documentado qué debe y qué no debe hacerse dentro de una ISR.
- Documentado el error detectado al limpiar la bandera de interrupción antes de validar el antirrebote.
- Documentado el patrón básico de temporización no bloqueante basado en `current_millis`, `previous_millis` e `interval`. 
- Documentada la diferencia entre `delay()` bloqueante y temporización con `millis()`.
- Documentada la relación conceptual entre `millis()` y futuros temporizadores tipo PLC.
- Documentado el uso combinado de `millis()`, antirrebote y cambio de modos.
- Documentada la estructura de tareas cooperativas dentro de `loop()`.
- Documentada la relación del ejercicio con futuras máquinas de estado.
- Documentados errores comunes detectados:
    - llamada a función sin paréntesis;
    - uso de comparación `!=` en lugar de asignación con negación `= !`.
- Documentao el patrón encapsulado:
```cpp
current_time - previous_time >= interval
```
- Documentacto el uso de `temporizador_cumplido()` como paso previo a temporizadores **TON**, **TOF**, **TP**.
- Documentada la diferencia entre temporización periódica y antirrebote.
- Documentado por qué el antirrebote mantiene comparación explícita con `millis()` en lugar de usar `temporizador_cumplido()`

```text
GPIO3 ---- pulsador ---- GND
```

- Documentada la lógica usada con `INPUT_PULLUP`:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
```

- Documentado `GPIO3` como entrada digital validada para pulsador externo.
- Documentada la diferencia entre estado del botón y evento de flanco.
- Documentado el patrón:

```text
lectura -> antirrebote -> estado estable -> detección de flanco -> acción
```

- Documentada la estructura de tareas cooperativas dentro de `loop()`.
- Documentado el uso de varios temporizadores basados en `millis()`:
  - `previous_blink_millis`;
  - `previous_serial_millis`;
  - `last_debounce_time`.
- Documentada la relación entre modos de funcionamiento y futura máquina de estados.
- Documentados errores encontrados:
  - reinicio continuo del parpadeo al llamar repetidamente a `aplicar_modo_actual()`;
  - comparación incorrecta contra `last_reading` en el antirrebote;
  - impresión periódica sin actualizar `previous_serial_millis`.
- Documentada la evolución desde modos numéricos hacia estados con nombre.
- Documentada la estructura básica de una máquina de estados:
  - estados;
  - eventos;
  - transiciones;
  - acciones asociadas a estado.
- Documentado el uso de `enum EstadoSistema`.
- Documentada la relación del ejercicio con máquinas de estado más avanzadas y bloques tipo PLC.
- Documentada la equivalencia entre un bloque `TON` de PLC y una clase C++.
- Documentados los conceptos `IN`, `PT`, `Q` y `ET`.
- Documentada la diferencia entre una función auxiliar de temporización y un objeto con estado interno.
- Documentado el uso de `millis()` dentro de un temporizador no bloqueante con estado propio.
- Documentada la diferencia entre `TON` y `TOF`.
- Documentado el concepto de retardo a la desconexión.
- Documentada la equivalencia entre un bloque `TOF` de PLC y una clase C++.
- Documentados los conceptos `IN`, `PT`, `Q` y `ET` aplicados a un `TOF`.
- Documentado que `TOF` temporiza cuando `IN` pasa a `false`, no cuando pasa a `true`.
- Documentada la diferencia entre `TON`, `TOF` y `TP`.
- Documentado el concepto de pulso temporizado.
- Documentada la equivalencia entre un bloque `TP` de PLC y una clase C++.
- Documentados los conceptos `IN`, `PT`, `Q` y `ET` aplicados a un `TP`.
- Documentado el uso de `previous_in` para detectar el flanco de activación.
- Documentada la relación del ejercicio con futuros bloques `R_TRIG` y `F_TRIG`.
- Documentado el concepto de flanco ascendente `false -> true`.
- Documentado el concepto de flanco descendente `true -> false`.
- Documentada la diferencia entre estado continuo y evento puntual.
- Documentada la equivalencia entre bloques PLC `R_TRIG` / `F_TRIG` y clases C++.
- Documentado el uso de `previous_clk` como memoria interna del estado anterior.
- Documentado que `R_TRIG` y `F_TRIG` no eliminan rebotes mecánicos.
- Documentada la secuencia recomendada para uso real:
  - lectura de entrada;
  - antirrebote;
  - señal estable;
  - detección de flanco.

### Learned

- Diferencia entre lectura directa de un pulsador y lectura filtrada con antirrebote.
- Los pulsadores mecánicos generan rebote y pueden producir múltiples lecturas por una sola pulsación física.
- `millis()` permite implementar antirrebote sin bloquear el programa.
- Con `INPUT_PULLUP`, una pulsación corresponde a un flanco descendente `HIGH -> LOW`.
- Para ejecutar una acción una sola vez por pulsación, conviene detectar flancos y no solo estados.
- Una función que debe modificar una variable externa debe recibirla por referencia o devolver el nuevo valor.
- Si una función recibe un objeto como parámetro, debe operar sobre ese objeto y no sobre una variable global oculta.
- Re-inforced debounce, edge detection and pass-by-reference concepts.
- Las interrupciones no eliminan el rebote mecánico de un pulsador.
- La ISR debe mantenerse lo más corta posible.
- El trabajo pesado debe hacerse en `loop()`, no dentro de la ISR.
- Una bandera de interrupción pendiente no debe limpiarse antes de completar la validación del evento.
- Con `INPUT_PULLUP`, `FALLING` representa la pulsación y `RISING` representa la liberación.
- delay() bloquea la ejecución del programa e impide atender otras tareas durante ese tiempo.
- millis() permite comprobar el paso del tiempo sin detener loop().
- Una variable de estado permite alternar una salida sin bloquear el programa.
- El patrón con millis() será la base para construir temporizadores tipo TON, TOF y TP.
- Un programa puede atender varias tareas dentro de `loop()` si ninguna bloquea la ejecución.
- `millis()` permite mantener un parpadeo activo mientras se sigue leyendo un pulsador.
- Una función debe llamarse con paréntesis para ejecutarse.
- `blink_state = !blink_state` invierte un estado booleano; `blink_state != blink_state` solo compara y no modifica la variable.
- Los modos de funcionamiento son una aproximación inicial a una máquina de estados.
- Una tarea cooperativa debe devolver el control rápidamente si no tiene que actuar.
- Cada temporización no bloqueante necesita su propia referencia temporal.
- `aplicar_modo_actual()` debe ejecutarse al cambiar de modo, no continuamente en cada vuelta de `loop()`.
- Para validar un cambio estable del pulsador, la lectura debe compararse contra el estado confirmado `button_state`.
- Una impresión periódica con `millis()` debe actualizar su marca temporal después de imprimir.
- Usar `enum` mejora la legibilidad frente a modos numéricos.
- Una máquina de estados sencilla puede separar claramente entrada, transición y salida.
- La función que detecta eventos no debe mezclarse con la función que ejecuta transiciones.
- Imprimir nombres de estado facilita la depuración frente a imprimir valores numéricos del `enum`.
- Las acciones cíclicas, como el parpadeo no bloqueante, deben ejecutarse solo en los estados que las necesitan.
- Una fucnión reutilizable puede reducir código repetido al trabajar con varias tareas temporizadas.
- El paso por referencia permite actualizar la marca temporal original desde una función.
- No todas las variables temporales tienen el mismo significado: una referencia periódica no es lo mismo que una marca de ultima cambio de entrada.
- `temporizador_cumplido()` es adecuado para tareas periódicas, pero no necesariamente para antirrebote.
- Este patrón prepara el cambio hacia temporizadores con estado interno tipo **TON**, **TOF** y **TP**.
- Un temporizador tipo `TON` necesita recordar estado interno entre llamadas.
- La variable `timing` evita reiniciar `start_time` en cada ciclo mientras `IN` permanece activo.
- Un bloque PLC puede representarse en C++ mediante una clase con métodos de actualización y consulta.
- `update()` debe llamarse cíclicamente desde `loop()`, igual que un bloque PLC se evalúa en cada ciclo.
- `Q()` y `ET()` deben funcionar como métodos de consulta y no modificar el estado interno del objeto.
- `TON` retarda la activación, mientras que `TOF` retarda la desactivación.
- En un `TOF`, `Q` se activa inmediatamente cuando `IN` es `true`.
- En un `TOF`, la temporización comienza cuando `IN` pasa a `false`.
- La variable `timing` evita reiniciar `start_time` mientras se temporiza la desconexión.
- Una clase C++ puede representar bloques PLC con estado interno y métodos de consulta.
- `TP` genera un pulso de duración fija ante un flanco de activación.
- Mantener `IN` en `true` no debe reiniciar el pulso.
- Para generar un nuevo pulso, `IN` debe volver a `false` y después pasar otra vez a `true`.
- `previous_in` permite detectar flancos comparando el estado anterior y el estado actual de la entrada.
- El `TP` introduce de forma natural la necesidad de bloques reutilizables de flanco como `R_TRIG` y `F_TRIG`.
- Un detector de flanco necesita recordar el estado anterior de la señal.
- `R_TRIG` convierte una transición `false -> true` en un evento de un solo ciclo.
- `F_TRIG` convierte una transición `true -> false` en un evento de un solo ciclo.
- Es más claro trabajar con una señal lógica `boton` que con el nivel eléctrico directo del GPIO.
- La detección de flancos y el antirrebote son conceptos distintos.
- Los bloques `R_TRIG` y `F_TRIG` completan el conjunto básico previo a la futura librería `lib/plc_blocks`.

### Fixed

- Corregido el comentario de prueba de `rgb_blink()` para reflejar que la función requiere el argumento `pixel`.
- Corregidos problemas de codificación UTF-8 en comentarios del código fuente.
- Corregida la creación del objeto `Adafruit_NeoPixel` en el ejercicio `02_boton_gpio` para usar `RGB_PIN` en lugar de `BUTTON_PIN`.
- Corregidos prototipos de funciones para que coincidan con las definiciones con referencias:
  - `Adafruit_NeoPixel &rgb_led`
  - `int &index`
- Corregido el uso interno de funciones para operar sobre el objeto recibido por parámetro (`rgb_led`) en lugar de depender del objeto global `pixel`.
- Corregida la lógica de antirrebote con interrupciones para no perder eventos pendientes antes de que finalice `DEBOUNCE_TIME_MS`.
- Corregida la doble llamada a `detectar_pulsacion_valida()` para evitar consumir el evento antes de ejecutar la transición de estado.
- Mejorada la impresión del estado actual para mostrar nombres explícitos en lugar de valores numéricos.
- Corregido el constructor `TON()` para inicializar todas las variables internas.
- Corregida la lógica de `update()` separando claramente:
  - reset cuando `IN = false`;
  - arranque de temporización;
  - actualización de `ET`;
  - activación de `Q`.
- Corregida la lógica inicial del `TOF`, que confundía el comportamiento con un `TON`.
- Corregido el uso de asignación en lugar de comparación en la condición de temporización:
  - incorrecto: `if (timing = false)`;
  - correcto: `if (!timing)` o `if (timing == false)`.
- Corregidos comentarios heredados de `TON` para reflejar correctamente la lógica de `TOF`.
- Corregida la implementación inicial incompleta del constructor `TP()`.
- Corregida la implementación inicial incompleta del método `update()`.
- Añadida actualización de `previous_in` al final de `update()`.
- Corregido texto de diagnóstico heredado de ejercicios anteriores, cambiando `Salida TON` por `Salida TP`.
- Aclarada la diferencia entre flanco eléctrico del GPIO y flanco lógico de la señal `boton`.
- Ajustada la interpretación de `INPUT_PULLUP` para trabajar con `boton = true` cuando el pulsador está físicamente pulsado.

---

## [0.1.0] - 2026-07-03

### Added

- Creación inicial de la estructura del repositorio `esp32-s3-cam-learning-lab`.
- Creación del documento `ROADMAP.md` con la hoja de ruta general del aprendizaje.
- Creación del documento `README.md` principal del repositorio.
- Creación del documento `CONTRIBUTING.md` con normas de contribución, documentación y estilo.
- Creación del archivo `.gitignore` para excluir:
  - archivos de PlatformIO;
  - archivos de compilación;
  - archivos temporales de Python;
  - archivos del sistema operativo;
  - credenciales;
  - logs.
- Definición de la estructura principal de carpetas:
  - `docs/`
  - `hardware/`
  - `arduino/`
  - `esp-idf/`
  - `proyectos/`
  - `journal/`
  - `assets/`
- Definición de la estructura recomendada para ejercicios Arduino:

```text
arduino/XX_nombre_ejercicio/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

- Definición de las reglas iniciales del proyecto:
  - aprendizaje paso a paso;
  - separación entre Arduino y ESP-IDF;
  - documentación obligatoria por ejercicio;
  - código comentado pedagógicamente;
  - revisión previa de GPIO antes de usarlos;
  - no subir credenciales al repositorio.

### Documented

- Placa principal del proyecto:
  - ESP32-S3-CAM basada en ESP32-S3-WROOM-1 N16R8;
  - 16 MB Flash;
  - 8 MB PSRAM;
  - cámara OV2640;
  - LED RGB integrado NeoPixel / WS2812 en GPIO48.
- Estado inicial del aprendizaje:
  - PlatformIO instalado y probado;
  - primer programa compilado y subido correctamente;
  - monitor serie funcionando;
  - LED RGB integrado controlado correctamente;
  - brillo del LED RGB controlado mediante comandos serie `+` y `-`.

### Learned

- Diferencia entre un LED normal y un NeoPixel / WS2812.
- Uso básico de la librería `Adafruit_NeoPixel`.
- Importancia de llamar a `show()` después de modificar el estado del NeoPixel.
- Necesidad de reenviar el color con `setPixelColor()` al modificar el brillo con `setBrightness()` en ciertos casos.
- Diferencia entre pasar objetos por valor y por referencia en C++.
- Problema de sombreado de variables (*shadowing*) al declarar una variable local con el mismo nombre que una global.
- Importancia de separar responsabilidades en funciones.
- Conveniencia de mantener las impresiones por serie fuera de funciones cuya responsabilidad principal sea otra.

### Fixed

- Corrección del uso de `digitalWrite()` para el LED integrado: el LED de la placa es NeoPixel / WS2812 y no un LED digital simple.
- Corrección del pin del LED integrado: uso de `GPIO48`.
- Corrección del paso por valor de `Adafruit_NeoPixel`; se pasa ahora por referencia mediante `Adafruit_NeoPixel &`.
- Corrección de shadowing en la variable `actual_bright`.
- Corrección del control de brillo del NeoPixel para aplicar correctamente el cambio visual.

---

## Convenciones de secciones

### Added

Para nuevas funcionalidades, documentos, ejercicios, carpetas o proyectos.

### Changed

Para cambios en contenido existente.

### Deprecated

Para funcionalidades o enfoques que se mantengan temporalmente pero se prevea eliminar.

### Removed

Para contenido eliminado.

### Fixed

Para correcciones de errores.

### Security

Para cambios relacionados con seguridad, credenciales, TLS, MQTT seguro, OTA o protección del dispositivo.

### Documented

Para documentación nueva o ampliada.

### Learned

Para registrar aprendizajes relevantes derivados de errores, pruebas o decisiones técnicas.

