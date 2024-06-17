#include <FastLED.h>
#include <NewPing.h>

#define DATA_PIN 8
#define NUM_LEDS 32
#define BRIGHTNESS 255  // Brilho máximo
#define LED_TYPE WS2812
#define COLOR_ORDER GRB

#define TRIG_PIN 3
#define ECHO_PIN 2

CRGB leds[NUM_LEDS];

// Variáveis de controle do ruído
uint16_t x = 0;  // Coordenada x para a função de ruído
uint16_t y = 0;  // Coordenada y para a função de ruído
uint16_t z = 0;  // Coordenada z para a função de ruído

// Configuração do sensor ultrassônico
NewPing sonar(TRIG_PIN, ECHO_PIN, 400); // Define TRIG_PIN como trigger e ECHO_PIN como echo, com um limite máximo de 400cm

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  randomSeed(analogRead(0));  // Seed aleatório para piscadas irregulares

  Serial.begin(9600);  // Inicializa a comunicação serial
}

void loop() {
  // Medir a distância
  unsigned int distance = sonar.ping_cm(); // Mede a distância em centímetros

  // Exibir a distância no monitor serial
  Serial.print("Distância: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Escolhe o efeito baseado na distância
  if (distance > 160) {
    fadeEffect();
  } else if (distance > 140) {
    fillLEDsFromNoise();
  } else if (distance > 100) {
    blinkRandom();
  } else if (distance > 70) {
    scanEffect();
  } else {
    suddenFlash();
  }

  FastLED.show();
  delay(20);  // Pequeno atraso para suavizar a transição dos efeitos
}

void fadeEffect() {
  for (int brightness = 0; brightness < 255; brightness++) {
    fill_solid(leds, NUM_LEDS, CRGB(brightness, 0, 0));
    FastLED.show();
    delay(10);
  }
  for (int brightness = 255; brightness >= 0; brightness--) {
    fill_solid(leds, NUM_LEDS, CRGB(brightness, 0, 0));
    FastLED.show();
    delay(10);
  }
}

void fillLEDsFromNoise() {
  // Aumente o valor de z para criar movimento ao longo do tempo
  z += 1;

  for (int i = 0; i < NUM_LEDS; i++) {
    // Use coordenadas x, y e z para gerar valores de ruído
    uint8_t noise = inoise8(i * 10 + x, y, z);

    // Converta o valor de ruído em uma intensidade de vermelho
    leds[i] = CRGB(noise, 0, 0);  // Variando apenas a intensidade do vermelho

    // Adiciona piscadas aleatórias nos LEDs
    if (random(0, 100) < 5) {
      leds[i] = CRGB::Black;
    }
  }

  // Aumente x e y para criar um efeito de movimento espacial
  x += 1;
  y += 1;
}

void scanEffect() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
    leds[i] = CRGB::Black;
  }
  for (int i = NUM_LEDS - 1; i >= 0; i--) {
    leds[i] = CRGB::Red;
    FastLED.show();
    delay(50);
    leds[i] = CRGB::Black;
  }
}

void blinkRandom() {
  int numBlinks = random(1, 5);
  for (int i = 0; i < numBlinks; i++) {
    int ledIndex = random(NUM_LEDS);
    leds[ledIndex] = CRGB::Red;
    FastLED.show();
    delay(random(50, 200));
    leds[ledIndex] = CRGB::Black;
    FastLED.show();
    delay(random(50, 200));
  }
}

void suddenFlash() {
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(100);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  delay(100);
}
