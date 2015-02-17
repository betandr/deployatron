#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define NUM_PIXELS 4

#define SPEAKER_PIN 2
#define DEPLOY_BUTTON_PIN 3
#define ARM_SWITCH_PIN 4
#define PIXELS_PIN 6

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIXELS_PIN, NEO_KHZ400);

int armSwitchState = 0;
int deployButtonState = 0;

boolean isArmed = false;
boolean isDeployed = false;

void setup() {
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(ARM_SWITCH_PIN, INPUT);
  pinMode(DEPLOY_BUTTON_PIN, INPUT);
  
  pixels.begin();
  pixels.show(); // Initialize all pixels to 'off'
}

void loop() {  
  armSwitchState = digitalRead(ARM_SWITCH_PIN);
  deployButtonState = digitalRead(DEPLOY_BUTTON_PIN);
  
  if (armSwitchState == LOW) {
    
    if (!isArmed) {
      fadeUpRed();
      pixels.show();
      isArmed = true;
      isDeployed = false;
    }
    
  } else {
   if (isArmed) {
     turnAllOff();
     pixels.show();
     isArmed = false;
   }
  }
  
  if (deployButtonState == LOW) {
      if (isArmed && !isDeployed) {
        pulseAll();
        isDeployed = true;
      }
  }
  
}

void turnAllOff() {
  for (uint16_t pixel=0; pixel<pixels.numPixels(); pixel++) {
    pixels.setPixelColor(pixel, pixels.Color(0, 0, 0));
  }
  pixels.show();
}

void fadeUpRed() {
  for (uint16_t brightness=0; brightness<256; brightness++) {
    for (uint16_t pixel=0; pixel<pixels.numPixels(); pixel++) {
      pixels.setPixelColor(pixel, pixels.Color(brightness, 0, 0));
    }
    
    tone(SPEAKER_PIN, 184 + brightness);
    pixels.show();
    delay(5);
  }
  
  noTone(SPEAKER_PIN);

  setPixel(500, pixels.Color(0, 255, 0), 3);
  setPixel(400, pixels.Color(0, 255, 0), 2);
  setPixel(400, pixels.Color(0, 255, 0), 1);
  setPixel(600, pixels.Color(0, 255, 0), 0);
}

void setPixel(uint16_t wait, uint32_t colour, uint16_t pixel) {
  pixels.setPixelColor(pixel, colour);
  pixels.show();
  tone(SPEAKER_PIN, 440);
  delay(30);
  noTone(SPEAKER_PIN);
  delay(wait - 30);
}

void pulseAll() {
  for (uint16_t pulses=0; pulses<20; pulses++) {

    for (uint16_t pixel=0; pixel<pixels.numPixels(); pixel++) {
      pixels.setPixelColor(pixel, pixels.Color(255-pulses, 0, 0));
    }    
    pixels.show();
    tone(SPEAKER_PIN, 100);
    delay(50);

    for (uint16_t pixel=0; pixel<pixels.numPixels(); pixel++) {
      pixels.setPixelColor(pixel, pixels.Color(0, 0, 255-pulses));
    }    
    pixels.show();
    tone(SPEAKER_PIN, 200);
    delay(50);
  }
  
  for (uint16_t pixel=0; pixel<pixels.numPixels(); pixel++) {
    pixels.setPixelColor(pixel, pixels.Color(255, 128, 0));
  }   
  
  noTone(SPEAKER_PIN);
}

void setToBlue() {
    for (uint16_t pixel=0; pixel<pixels.numPixels(); pixel++) {
    pixels.setPixelColor(pixel, pixels.Color(0, 0, 255));
  }
  pixels.show();  
}
