# Buenas prácticas

Guía de buenas prácticas para el repositorio **ESP32-S3-CAM Learning Lab**.

Este documento reúne criterios generales de trabajo para escribir código, documentar ejercicios, usar hardware de forma segura y mantener el repositorio limpio y útil a largo plazo.

---

## 1. Filosofía general

El objetivo principal del repositorio es aprender bien, de forma progresiva y documentada.

Por tanto, las buenas prácticas se aplicarán con este orden de prioridad:

1. Seguridad eléctrica.
2. Claridad conceptual.
3. Código comprensible.
4. Documentación útil.
5. Reutilización razonable.
6. Optimización cuando sea necesaria.

En los primeros ejercicios se priorizará código explícito y pedagógico. La abstracción avanzada se introducirá progresivamente.

---

## 2. Buenas prácticas de repositorio

### 2.1 Mantener una estructura coherente

Cada tipo de contenido debe estar en su lugar:

```text
hardware/   -> documentación de placa y periféricos
arduino/    -> ejercicios con Arduino Framework
esp-idf/    -> ejercicios con ESP-IDF
proyectos/  -> proyectos integradores
docs/       -> documentación general
journal/    -> diario de aprendizaje
assets/     -> imágenes, esquemas y diagramas
```

---

### 2.2 Separar ejercicios de proyectos

Un ejercicio debe enseñar un concepto concreto.

Un proyecto debe integrar varios conceptos.

Ejemplo de ejercicio:

```text
arduino/02_boton_gpio/
```

Ejemplo de proyecto:

```text
proyectos/camara_ip_basica/
```

---

### 2.3 Cada ejercicio debe ser autocontenido

Cada ejercicio Arduino debe tener esta estructura mínima:

```text
arduino/XX_nombre_ejercicio/
├── README.md
├── platformio.ini
└── src/
    └── main.cpp
```

Esto permite abrir, compilar y probar cada ejercicio de forma independiente.

---

### 2.4 Evitar documentación duplicada innecesaria

La documentación común debe estar en documentos de referencia.

Ejemplo:

- El pinout general debe estar en `hardware/pinout.md`.
- Los GPIO reservados deben estar en `hardware/gpio_reservados.md`.
- El ejercicio debe enlazar o mencionar esos documentos, no copiarlos completos.

---

## 3. Buenas prácticas de documentación

### 3.1 Documentos técnicos estables

Los documentos técnicos deben describir información de referencia.

Evitar en ellos secciones como:

```markdown
## Planificación futura
## Tareas pendientes
## Siguiente sesión
```

La planificación debe ir en:

```text
ROADMAP.md
CHANGELOG.md
journal/
```

---

### 3.2 Documentar con intención pedagógica

La documentación debe ayudar a entender:

- qué se está haciendo,
- por qué se hace,
- qué se espera observar,
- qué errores pueden aparecer,
- cómo se relaciona con ejercicios futuros.

No se trata solo de guardar código, sino de construir material de consulta.

---

### 3.3 Cada ejercicio debe tener README propio

El `README.md` de cada ejercicio debe incluir, como mínimo:

```markdown
# Ejercicio XX - Nombre

## Objetivo

## Hardware utilizado

## Conceptos trabajados

## Funcionamiento

## Cableado

## Resultado esperado

## Problemas encontrados

## Mejoras futuras
```

Si una sección no aplica, se indicará explícitamente.

Ejemplo:

```markdown
## Cableado

No requiere cableado externo. Se utiliza el LED RGB integrado de la placa.
```

---

### 3.4 Usar imágenes y esquemas en `assets/`

Los recursos visuales deben guardarse en:

```text
assets/images/      -> fotos, capturas, imágenes generales
assets/diagrams/    -> diagramas conceptuales
assets/schematics/  -> esquemas eléctricos y cableados
```

Evitar nombres con espacios, tildes o caracteres especiales.

Correcto:

```text
assets/images/goouuu_esp32_s3_cam_pinout.jpeg
assets/schematics/button_gpio_pullup.png
```

Menos recomendable:

```text
assets/imágenes/foto placa final.jpeg
```

---

## 4. Buenas prácticas de código

### 4.1 Claridad antes que compacidad

En ejercicios didácticos se prefiere código claro a código demasiado compacto.

Correcto para aprendizaje:

```cpp
if (brillo < 255)
{
    brillo++;
}
```

Menos pedagógico al inicio:

```cpp
brillo = min(brillo + 1, 255);
```

Ambos pueden ser válidos, pero en fases iniciales se prioriza legibilidad.

---

### 4.2 Nombres descriptivos

Usar nombres que expresen intención.

Correcto:

```cpp
int brillo_actual = 20;
int nuevo_brillo = 0;
```

Menos recomendable:

```cpp
int b = 20;
int x = 0;
```

---

### 4.3 Funciones con responsabilidad clara

Una función debe hacer una cosa principal.

Correcto:

```cpp
int actualizar_brillo_desde_serie(int brillo_actual);
void aplicar_brillo_rgb(Adafruit_NeoPixel &rgb_led, int brillo);
```

Menos recomendable:

```cpp
void leer_serie_cambiar_led_imprimir_y_guardar();
```

---

### 4.4 Evitar efectos laterales innecesarios

Si una función calcula un valor, no debería imprimir, modificar hardware y guardar datos al mismo tiempo salvo que ese sea explícitamente su propósito.

Criterio usado en este repositorio:

- La lógica calcula o actualiza estado.
- La capa de salida imprime si es necesario.
- La capa de hardware aplica cambios al periférico.

---

### 4.5 Inicializar variables

No usar variables locales sin inicializar.

Incorrecto:

```cpp
int valor;
Serial.println(valor);
```

Correcto:

```cpp
int valor = 0;
Serial.println(valor);
```

---

### 4.6 Evitar shadowing

No declarar variables locales con el mismo nombre que variables globales si eso puede crear confusión.

Incorrecto:

```cpp
int actual_bright = update_brillo_serie(pixel, actual_bright);
```

Correcto:

```cpp
actual_bright = update_brillo_serie(pixel, actual_bright);
```

---

### 4.7 Pasar objetos grandes por referencia

Evitar copias innecesarias de objetos.

Incorrecto:

```cpp
void rgb_blink(Adafruit_NeoPixel rgb_led)
```

Correcto:

```cpp
void rgb_blink(Adafruit_NeoPixel &rgb_led)
```

Si la función no debe modificar el objeto, se valorará usar referencia constante cuando proceda:

```cpp
void imprimir_configuracion(const MiConfiguracion &config)
```

---

### 4.8 Limitar rangos

Cuando una variable representa un rango físico o lógico, debe limitarse.

Ejemplo para brillo NeoPixel:

```cpp
if (brillo < 255)
{
    brillo++;
}

if (brillo > 0)
{
    brillo--;
}
```

Esto evita valores fuera de rango.

---

## 5. Buenas prácticas con `Serial`

### 5.1 Usar mensajes de arranque

Incluir un mensaje básico al inicio ayuda a confirmar que el firmware ha arrancado.

```cpp
Serial.begin(115200);
Serial.println("Inicio OK");
```

---

### 5.2 No saturar el monitor serie

Evitar imprimir continuamente en cada iteración de `loop()`.

Mejor imprimir solo cuando cambie el estado:

```cpp
if (nuevo_valor != valor_actual)
{
    valor_actual = nuevo_valor;
    Serial.println(valor_actual);
}
```

---

### 5.3 Usar mensajes útiles

Un buen mensaje de diagnóstico debe decir qué ocurrió y, si es posible, dónde.

Menos útil:

```cpp
Serial.println("Error");
```

Más útil:

```cpp
Serial.println("Error: no se pudo inicializar la cámara");
```

---

## 6. Buenas prácticas de GPIO

### 6.1 Revisar el pinout antes de usar un GPIO

Antes de usar un pin, consultar:

```text
hardware/pinout.md
hardware/gpio_reservados.md
```

No asumir que un GPIO está libre solo porque aparece en el header.

---

### 6.2 No usar pines de cámara para ejercicios generales

Los pines de cámara deben considerarse reservados.

Ejemplo de pin no recomendable para botón:

```cpp
#define BUTTON_PIN 4
```

Motivo:

```text
GPIO4 -> CAM_SIOD
```

---

### 6.3 Usar `INPUT_PULLUP` para botones simples

Para un pulsador conectado entre GPIO y GND:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

Lectura esperada:

```text
Sin pulsar -> HIGH
Pulsado    -> LOW
```

---

### 6.4 Documentar cada GPIO validado

Cuando un pin se pruebe correctamente, debe registrarse en:

```text
hardware/pinout.md
hardware/gpio_reservados.md
```

si procede.

---

## 7. Buenas prácticas eléctricas

### 7.1 Respetar niveles de 3.3 V

Los GPIO del ESP32-S3 trabajan a 3.3 V.

No conectar señales de 5 V directamente a los GPIO.

---

### 7.2 Usar GND común

Cuando un periférico externo comparte señales con la placa, normalmente debe compartir también GND.

```text
GND fuente externa ---- GND ESP32-S3-CAM
Señal periférico ------ GPIO ESP32-S3-CAM
```

---

### 7.3 No alimentar cargas desde GPIO

Un GPIO no debe alimentar directamente:

- motores,
- relés,
- solenoides,
- tiras LED,
- cargas inductivas.

Usar drivers, transistores, MOSFETs, optoacopladores o módulos adecuados.

---

### 7.4 Revisar alimentación en problemas extraños

Si aparecen reinicios o errores intermitentes, revisar:

- cable USB,
- fuente de alimentación,
- consumo de periféricos,
- cámara,
- WiFi,
- SD,
- conexiones de GND.

---

## 8. Buenas prácticas con NeoPixel

### 8.1 No usar `digitalWrite()`

El RGB integrado es un NeoPixel / WS2812, no un LED simple.

Usar la librería correspondiente:

```cpp
#include <Adafruit_NeoPixel.h>
```

---

### 8.2 Llamar siempre a `show()`

Después de cambiar color o brillo:

```cpp
pixel.setPixelColor(0, pixel.Color(255, 0, 0));
pixel.show();
```

---

### 8.3 Usar brillo moderado

Para pruebas iniciales:

```cpp
pixel.setBrightness(20);
```

---

## 9. Buenas prácticas con cámara

### 9.1 Activar PSRAM

La cámara puede necesitar PSRAM para resoluciones medias o altas.

En PlatformIO se usará una configuración equivalente a:

```ini
build_flags =
    -DBOARD_HAS_PSRAM
```

---

### 9.2 Empezar con resolución baja

Para primeras pruebas con cámara, comenzar con resoluciones bajas como QVGA o VGA.

Evitar empezar directamente con resoluciones altas.

---

### 9.3 Liberar frame buffers

Cuando se obtenga un frame buffer de cámara, debe devolverse después de usarlo.

```cpp
camera_fb_t *fb = esp_camera_fb_get();

// usar fb

esp_camera_fb_return(fb);
```

---

## 10. Buenas prácticas con credenciales

### 10.1 No subir secretos

No subir al repositorio:

- contraseñas WiFi,
- tokens,
- claves API,
- certificados privados,
- credenciales MQTT,
- variables `.env` reales.

---

### 10.2 Usar archivos de ejemplo

Para ejercicios con credenciales, usar archivos tipo:

```text
secrets.example.h
```

Ejemplo:

```cpp
#pragma once

#define WIFI_SSID "TU_SSID"
#define WIFI_PASSWORD "TU_PASSWORD"
#define MQTT_SERVER "broker.example.com"
```

El archivo real debe estar excluido por `.gitignore`.

---

## 11. Buenas prácticas de commits

### 11.1 Commits pequeños y descriptivos

Ejemplos recomendados:

```text
Add initial repository structure
Add hardware pinout documentation
Add RGB NeoPixel exercise
Fix NeoPixel brightness handling
Document reserved GPIO pins
```

Evitar:

```text
update
fix
changes
stuff
```

---

### 11.2 Un commit por cambio lógico

No mezclar cambios no relacionados en un mismo commit.

Ejemplo de separación:

- un commit para documentación de hardware,
- otro commit para ejercicio RGB,
- otro commit para corregir `.gitignore`.

---

## 12. Buenas prácticas de aprendizaje

### 12.1 Registrar errores importantes

Los errores útiles deben documentarse en:

```text
docs/lecciones_aprendidas.md
journal/
```

---

### 12.2 Validar antes de abstraer

Primero hacer que algo funcione de forma simple.

Después:

- limpiar,
- refactorizar,
- comentar,
- documentar,
- generalizar si merece la pena.

---

### 12.3 No avanzar si la base no está clara

Antes de pasar a temas como WiFi, MQTT, cámara o FreeRTOS, deben estar claros:

- GPIO,
- entradas y salidas,
- interrupciones,
- temporización no bloqueante,
- uso básico de PlatformIO,
- monitor serie,
- estructura del repositorio.

---

## 13. Criterio general de calidad

Un ejercicio o documento debe responder razonablemente a estas preguntas:

- ¿Qué problema resuelve o qué concepto enseña?
- ¿Qué hardware usa?
- ¿Cómo se compila y ejecuta?
- ¿Qué resultado se espera?
- ¿Qué errores pueden aparecer?
- ¿Qué se ha aprendido?
- ¿Dónde encaja dentro del roadmap?

Si no responde a esas preguntas, debe mejorarse antes de considerarlo terminado.
