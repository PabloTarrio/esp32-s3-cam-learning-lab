// ====== librerías ============================================================
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ====== define ===============================================================
#define LED_PIN 48
#define NUM_LEDS 1

// ===== Constantes y Variables ================================================
Adafruit_NeoPixel pixel (NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
int actual_bright = 20;

// ===== INICIALIZACION ========================================================
void setup() {
  Serial.begin(115200);
  pixel.begin();
  pixel.clear();
  pixel.show();
  pixel.setBrightness(actual_bright);
  pixel.setPixelColor(0, pixel.Color(0, 0, 255));
  pixel.show();
  Serial.println("RGB_listo");
}

// ===== funciones =============================================================
void rgb_blink (Adafruit_NeoPixel &rgb_led) {
  // Establece el brillo del led interno WS2812 NeoPixel y parpadeo del mismo 
  // cambiando de color.
  rgb_led.setBrightness(50);
  rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
  rgb_led.show();
  delay(500);

  rgb_led.setPixelColor(0, rgb_led.Color(0, 255, 0));
  rgb_led.show();
  delay(500);

  rgb_led.setPixelColor(0, rgb_led.Color(0, 0, 255));
  rgb_led.show();
  delay(500);
}

void progressive_brightness (Adafruit_NeoPixel &rgb_led){
  // Incrementa el brillo del led interno WS2812 NeoPixel automáticamente.
  for (int brillo = 1; brillo <= 100; brillo++) {
    rgb_led.setBrightness(brillo);
    rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
    rgb_led.show();
    delay(50);
  }
}

int update_brillo_serie (Adafruit_NeoPixel &rgb_led, int brillo){

  bool pulsacion_detectada = false;
    
  if (Serial.available() > 0){
    char tecla_pulsada = Serial.read();
    if (tecla_pulsada == '+' && brillo < 255) {
      brillo++;
      pulsacion_detectada = true;
    }
    else if (tecla_pulsada == '-' && brillo > 0) {
      brillo--;
      pulsacion_detectada = true;
    }
  }

  if (pulsacion_detectada){
    rgb_led.setBrightness(brillo);
    rgb_led.setPixelColor(0, rgb_led.Color(255, 0, 0));
    rgb_led.show();
  }
  return brillo;
}

// ===== BUCLE PRINCIPAL =======================================================
void loop() {
  // rgb_blink();
  // progressive_brightness(pixel);
  int new_bright = update_brillo_serie(pixel, actual_bright);
  if (new_bright != actual_bright){
    actual_bright = new_bright;
    printf("El brillo actual es: %d\n", actual_bright);
  }
}