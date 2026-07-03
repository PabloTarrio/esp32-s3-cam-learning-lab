# Glosario

Glosario de términos utilizados en el repositorio **ESP32-S3-CAM Learning Lab**.

Este documento recoge conceptos de electrónica, programación embebida, ESP32, PlatformIO, Arduino Framework, ESP-IDF, comunicaciones e IoT que aparecerán durante el aprendizaje.

---

## A

### ADC

**Analog-to-Digital Converter**. Conversor analógico-digital.

Permite convertir una tensión analógica en un valor digital que puede leer el microcontrolador.

En ESP32-S3, las lecturas ADC suelen representarse como valores numéricos dentro de un rango, por ejemplo `0-4095` para una resolución de 12 bits.

---

### Arduino Framework

Capa de abstracción que permite programar microcontroladores usando una estructura sencilla basada en:

```cpp
void setup()
{
}

void loop()
{
}
```

En ESP32, Arduino Framework se ejecuta sobre componentes de bajo nivel de Espressif y sobre FreeRTOS, aunque al principio esto quede oculto para el programador.

---

### API

**Application Programming Interface**. Interfaz de programación de aplicaciones.

Define cómo un programa, librería o servicio expone funcionalidades para que otro código pueda utilizarlas.

En proyectos IoT aparecerá en contextos como:

- API REST,
- API HTTP,
- API de librerías,
- API de ESP-IDF.

---

## B

### Baud rate

Velocidad de transmisión de datos en una comunicación serie.

En los primeros ejercicios se utiliza:

```text
115200 baudios
```

Ejemplo:

```cpp
Serial.begin(115200);
```

---

### Boot

Proceso de arranque del microcontrolador.

Algunos pines pueden afectar al modo de arranque. En ESP32 es habitual que ciertos GPIO tengan funciones especiales durante el boot.

---

### Broker MQTT

Servidor que gestiona la comunicación MQTT entre clientes.

Los dispositivos publican mensajes en `topics` y otros dispositivos se suscriben a esos `topics` para recibirlos.

---

### Brownout

Caída de tensión de alimentación por debajo de un umbral seguro.

Puede provocar:

- reinicios,
- errores de cámara,
- fallos WiFi,
- comportamiento inestable.

En placas ESP32 suele aparecer cuando la alimentación no es suficientemente estable o no puede entregar la corriente necesaria.

---

## C

### C/C++

Lenguajes principales usados para programar el ESP32.

C se usa mucho en ESP-IDF y bajo nivel. C++ se usa habitualmente en Arduino Framework y en muchas librerías.

---

### Cámara OV2640

Sensor de cámara utilizado en muchas placas ESP32-CAM.

Permite capturar imágenes y trabajar con formatos como JPEG.

En esta placa se conecta mediante una interfaz paralela DVP y ocupa varios GPIO.

---

### CHANGELOG

Archivo donde se registran los cambios relevantes del repositorio.

En este proyecto se usa:

```text
CHANGELOG.md
```

Sirve para documentar:

- nuevos ejercicios,
- cambios de estructura,
- correcciones,
- documentación añadida,
- decisiones relevantes.

---

### Cliente HTTP

Programa o dispositivo que realiza peticiones HTTP a un servidor.

Ejemplo: un ESP32 que solicita datos a una API web.

---

### Cliente MQTT

Dispositivo o programa que se conecta a un broker MQTT para publicar o recibir mensajes.

Un ESP32 puede actuar como cliente MQTT.

---

### Commit

Registro de un cambio en Git.

Un commit debe representar un cambio lógico y tener un mensaje descriptivo.

Ejemplo:

```text
Add RGB NeoPixel exercise
```

---

### CONtributing / CONTRIBUTING

Documento que define normas para contribuir o trabajar en el repositorio.

En este proyecto se usa:

```text
CONTRIBUTING.md
```

---

## D

### Debounce / Antirrebote

Técnica usada para evitar múltiples lecturas falsas cuando se pulsa o libera un botón mecánico.

Los pulsadores reales no cambian de estado de forma perfectamente limpia; generan rebotes durante unos milisegundos.

---

### Debug

Proceso de diagnóstico y corrección de errores.

En los primeros ejercicios se hará principalmente con:

```cpp
Serial.println();
printf();
```

Más adelante, en ESP-IDF, se usará logging con macros como:

```c
ESP_LOGI();
ESP_LOGW();
ESP_LOGE();
```

---

### DHCP

**Dynamic Host Configuration Protocol**.

Protocolo que permite asignar automáticamente una dirección IP a un dispositivo dentro de una red.

Cuando el ESP32 se conecte a una red WiFi, normalmente recibirá su IP mediante DHCP.

---

### DNS

**Domain Name System**.

Sistema que traduce nombres de dominio a direcciones IP.

Ejemplo:

```text
example.com -> dirección IP
```

---

### DVP

**Digital Video Port**.

Interfaz paralela usada por algunas cámaras, como la OV2640, para transmitir datos de imagen al microcontrolador.

---

## E

### ESP32

Familia de microcontroladores de Espressif con conectividad WiFi y Bluetooth.

Existen muchas variantes, como ESP32, ESP32-S2, ESP32-S3, ESP32-C3, etc.

---

### ESP32-S3

Variante del ESP32 con CPU Xtensa LX7 dual-core, WiFi, Bluetooth LE y capacidades avanzadas para periféricos, USB y procesamiento.

Es el microcontrolador usado por la placa de este repositorio.

---

### ESP-IDF

**Espressif IoT Development Framework**.

Framework oficial de Espressif para programar ESP32 de forma más profesional y cercana al hardware.

Usa conceptos como:

- `app_main()`,
- componentes,
- FreeRTOS,
- drivers nativos,
- `sdkconfig`,
- logging con `ESP_LOGx`.

---

### ESP_LOGI / ESP_LOGW / ESP_LOGE

Macros de logging usadas en ESP-IDF.

Ejemplos:

```c
ESP_LOGI(TAG, "Mensaje informativo");
ESP_LOGW(TAG, "Advertencia");
ESP_LOGE(TAG, "Error");
```

---

## F

### Flash

Memoria no volátil donde se almacena el firmware y otros datos persistentes.

La placa usada en este repositorio tiene 16 MB de Flash.

---

### Frame buffer

Bloque de memoria donde se almacena una imagen capturada por la cámara.

En ejercicios de cámara aparecerá con estructuras como:

```cpp
camera_fb_t *fb = esp_camera_fb_get();
```

Después de usarlo debe liberarse:

```cpp
esp_camera_fb_return(fb);
```

---

### FreeRTOS

Sistema operativo en tiempo real usado internamente por ESP32.

Permite trabajar con:

- tareas,
- colas,
- semáforos,
- mutex,
- prioridades,
- planificación multitarea.

Arduino sobre ESP32 también se ejecuta sobre FreeRTOS, aunque inicialmente no sea evidente.

---

## G

### Git

Sistema de control de versiones usado para registrar cambios en el repositorio.

Permite trabajar con commits, ramas, historial y colaboración.

---

### GitHub

Plataforma online para alojar repositorios Git.

En este proyecto se usará para mantener documentación, ejemplos, ejercicios y proyectos.

---

### GPIO

**General Purpose Input/Output**.

Pin de propósito general que puede configurarse como entrada o salida digital, y en algunos casos con funciones alternativas como ADC, PWM, I2C, SPI, UART, etc.

En una placa ESP32-S3-CAM no todos los GPIO están libres porque muchos están conectados a periféricos internos.

---

### GND

Referencia de tierra del circuito.

Cuando dos sistemas intercambian señales eléctricas, normalmente deben compartir GND para tener una referencia común.

---

## H

### HTTP

**HyperText Transfer Protocol**.

Protocolo usado para comunicación web.

En ESP32 se usará para:

- cliente HTTP,
- servidor HTTP,
- API REST,
- control desde navegador,
- transmisión de imágenes.

---

## I

### I2C

Bus de comunicación serie síncrono que usa normalmente dos líneas:

```text
SDA -> datos
SCL -> reloj
```

Permite conectar varios dispositivos usando direcciones.

---

### IoT

**Internet of Things**. Internet de las cosas.

Conjunto de dispositivos conectados que capturan datos, actúan sobre el entorno y se comunican por red.

En este repositorio se trabajará con IoT mediante WiFi, HTTP, MQTT, sensores y cámara.

---

### ISR

**Interrupt Service Routine**. Rutina de servicio de interrupción.

Función que se ejecuta cuando ocurre una interrupción hardware.

Debe ser breve y evitar operaciones pesadas.

---

## J

### JPEG

Formato de imagen comprimida.

Muy usado en capturas de cámara porque reduce mucho el tamaño de la imagen frente a formatos sin comprimir.

---

### JSON

**JavaScript Object Notation**.

Formato de texto usado para representar datos estructurados.

Ejemplo:

```json
{
  "temperatura": 23.5,
  "humedad": 60
}
```

Se usará en APIs HTTP e IoT.

---

## L

### LED

Diodo emisor de luz.

Puede ser un LED simple controlado con `HIGH/LOW` o un LED direccionable como NeoPixel / WS2812.

---

### LittleFS

Sistema de archivos ligero para memoria Flash.

Puede usarse para guardar configuración, ficheros web o pequeños datos persistentes.

---

### Loop

Función principal repetitiva en Arduino Framework:

```cpp
void loop()
{
}
```

Se ejecuta continuamente después de `setup()`.

---

## M

### Máquina de estados

Modelo de programación donde el sistema se organiza en estados y transiciones.

Muy útil para programas embebidos no bloqueantes.

---

### MQTT

**Message Queuing Telemetry Transport**.

Protocolo ligero muy usado en IoT.

Se basa en:

- broker,
- clientes,
- publicación,
- suscripción,
- topics.

---

### Mutex

Mecanismo de sincronización usado para proteger recursos compartidos entre tareas.

Se usará cuando se estudie FreeRTOS.

---

## N

### NeoPixel

Nombre comercial usado habitualmente para LEDs RGB direccionables basados en controladores como WS2812.

El RGB integrado de la placa se controla como NeoPixel en `GPIO48`.

---

### NVS

**Non-Volatile Storage**.

Sistema de almacenamiento no volátil usado en ESP-IDF para guardar datos pequeños como configuración.

---

## O

### OTA

**Over-The-Air update**.

Actualización inalámbrica de firmware.

Permite actualizar un dispositivo sin conectarlo físicamente por USB.

---

### OV2640

Sensor de cámara usado en la placa ESP32-S3-CAM de este repositorio.

Puede capturar imágenes y generar JPEG, dependiendo de la configuración.

---

## P

### PlatformIO

Entorno de desarrollo para sistemas embebidos integrado habitualmente con Visual Studio Code.

Permite gestionar:

- placas,
- frameworks,
- librerías,
- compilación,
- subida de firmware,
- monitor serie.

---

### PSRAM

Memoria RAM externa usada para ampliar la memoria disponible del ESP32.

La placa usada en este repositorio tiene 8 MB de PSRAM.

Es especialmente importante para cámara y buffers grandes.

---

### Pull-down

Resistencia, interna o externa, que mantiene una entrada digital en nivel bajo cuando no hay otra señal activa.

---

### Pull-up

Resistencia, interna o externa, que mantiene una entrada digital en nivel alto cuando no hay otra señal activa.

En botones conectados a GND se suele usar:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

---

### PWM

**Pulse Width Modulation**. Modulación por ancho de pulso.

Permite simular una salida analógica variando el ciclo de trabajo de una señal digital.

Se usa para:

- brillo de LEDs simples,
- control de motores,
- señales de control.

---

## Q

### Queue / Cola

Mecanismo de comunicación entre tareas en FreeRTOS.

Permite enviar datos de una tarea a otra de forma segura.

---

## R

### README

Documento principal de explicación de una carpeta o repositorio.

En GitHub, el `README.md` raíz suele mostrarse como portada del proyecto.

---

### REST

Estilo de arquitectura para APIs web basado normalmente en HTTP.

En ESP32 puede usarse para controlar el dispositivo desde una aplicación o navegador.

---

### RGB

Sistema de color basado en tres canales:

- Red,
- Green,
- Blue.

El LED integrado permite mezclar estos tres canales para generar colores.

---

## S

### SCCB

**Serial Camera Control Bus**.

Bus usado por cámaras como OV2640 para configuración interna.

Es conceptualmente parecido a I2C.

---

### SDKConfig

Archivo de configuración usado por ESP-IDF para definir opciones del proyecto.

---

### Semáforo

Mecanismo de sincronización usado en FreeRTOS.

Permite coordinar tareas o eventos.

---

### Serial

Interfaz de comunicación serie usada para depuración y comunicación básica.

En Arduino se usa con:

```cpp
Serial.begin(115200);
Serial.println("Mensaje");
```

---

### Setup

Función de inicialización en Arduino Framework:

```cpp
void setup()
{
}
```

Se ejecuta una vez al arrancar el programa.

---

### Shadowing

Situación en la que una variable local tiene el mismo nombre que otra variable de un ámbito superior, ocultándola.

Ejemplo problemático:

```cpp
int actual_bright = update_brillo_serie(pixel, actual_bright);
```

---

### SPI

Bus de comunicación serie síncrono que usa normalmente:

```text
MOSI
MISO
SCLK
CS
```

Se usa con pantallas, memorias, sensores, tarjetas SD externas y otros periféricos.

---

### SPIFFS

Sistema de archivos para memoria Flash usado históricamente en ESP32.

En muchos proyectos modernos puede preferirse LittleFS, según soporte y caso de uso.

---

### STA / Station

Modo WiFi en el que el ESP32 se conecta a un punto de acceso existente, como un router.

---

## T

### Task / Tarea

Unidad de ejecución en FreeRTOS.

Una aplicación ESP32 puede tener varias tareas ejecutándose de forma concurrente.

---

### TCP/IP

Conjunto de protocolos base de Internet y redes locales.

WiFi, HTTP y MQTT se apoyan sobre TCP/IP.

---

### Topic

Canal lógico de comunicación en MQTT.

Ejemplo:

```text
casa/salon/temperatura
esp32/camara/comando
```

---

## U

### UART

Interfaz de comunicación serie asíncrona.

Usa normalmente señales TX y RX.

Se emplea para monitor serie, módulos externos o comunicación entre dispositivos.

---

### USB

Bus usado para alimentación, programación y comunicación con la placa.

En esta placa algunos GPIO están asociados a señales USB y no deben usarse como GPIO generales.

---

## V

### Volatile

Calificador de C/C++ que indica que una variable puede cambiar fuera del flujo normal del programa.

Es habitual en variables compartidas con interrupciones.

Ejemplo:

```cpp
volatile bool boton_pulsado = false;
```

---

## W

### WiFi AP

Modo punto de acceso.

El ESP32 crea su propia red WiFi para que otros dispositivos se conecten.

---

### WiFi STA

Modo estación.

El ESP32 se conecta a una red WiFi existente.

---

### WS2812

Controlador usado en muchos LEDs RGB direccionables.

El LED RGB integrado de la placa se controla como WS2812/NeoPixel mediante una señal digital específica.

---

## X

### XCLK

Reloj externo proporcionado a la cámara.

En esta placa está asociado a la cámara OV2640.

---

## Z

### Zero-based index

Convención de programación en la que el primer elemento tiene índice `0`.

Ejemplo en NeoPixel:

```cpp
pixel.setPixelColor(0, pixel.Color(255, 0, 0));
```

El índice `0` hace referencia al primer LED.
