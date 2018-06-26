//fade in, fade out until released


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <FastLED.h>
#include "AudioSample1.h"


// GUItool: begin automatically generated code
AudioPlayMemory          playMem3;       //xy=385.5555419921875,235.2222137451172
AudioPlayMemory          playMem2;       //xy=387.5555419921875,194.2222137451172
AudioPlayMemory          playMem4;       //xy=389.8888931274414,268.33335971832275
AudioPlayMemory          playMem1;       //xy=399.5555419921875,162.2222137451172
AudioPlayMemory          playMem5;       //xy=418.5555419921875,336.2222137451172
AudioPlayMemory          playMem6;       //xy=430.5555419921875,369.2222137451172
AudioPlayMemory          playMem9;       //xy=448.5555419921875,473.2222137451172
AudioPlayMemory          playMem8;       //xy=451.5555419921875,438.2222137451172
AudioPlayMemory          playMem10;      //xy=459.5555419921875,518.2222137451172
AudioPlayMemory          playMem7;       //xy=463.5555419921875,403.2222137451172
AudioMixer4              Mix1;           //xy=639.5555419921875,202.2222137451172
AudioMixer4              Mix2;           //xy=773.6666564941406,332.66669845581055
AudioMixer4              Mix3;           //xy=1083.6667518615723,348.88890647888184
AudioOutputI2S           i2s2;           //xy=1275.1112747192383,345.7778148651123
AudioConnection          patchCord1(playMem3, 0, Mix1, 2);
AudioConnection          patchCord2(playMem2, 0, Mix1, 1);
AudioConnection          patchCord3(playMem4, 0, Mix1, 3);
AudioConnection          patchCord4(playMem1, 0, Mix1, 0);
AudioConnection          patchCord5(playMem5, 0, Mix2, 1);
AudioConnection          patchCord6(playMem6, 0, Mix2, 2);
AudioConnection          patchCord7(playMem9, 0, Mix3, 2);
AudioConnection          patchCord8(playMem8, 0, Mix2, 0);
AudioConnection          patchCord9(playMem10, 0, Mix3, 3);
AudioConnection          patchCord10(playMem7, 0, Mix2, 3);
AudioConnection          patchCord11(Mix1, 0, Mix3, 0);
AudioConnection          patchCord12(Mix2, 0, Mix3, 1);
AudioConnection          patchCord13(Mix3, 0, i2s2, 0);
AudioConnection          patchCord14(Mix3, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=891.5555419921875,210.2222137451172
// GUItool: end automatically generated code
#define NUM_LEDS 2
#define DATA_PIN 2
CRGB leds[NUM_LEDS];
uint8_t hue = 0;
#define COLOR_ORDER GRB  // I had to change this for my strip if your color is off then you know.
int initialFadeAmount = 5;  // Set the amount to fade I usually do 5, 10, 15, 20, 25 etc even up to 255.
int fadeAmount = initialFadeAmount;
int brightness = 0;
int g = 0;
int r = 255;
int b = 200;

int pin1 = 0;

int led = 13;
int i1 = 0;


int threshold = 1000;
// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

void setup() {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  pinMode(led, OUTPUT);
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(50);

  // Comment these out if not using the audio adaptor board.
  // This may wait forever if the SDA & SCL pins lack
  // pullup resistors
  sgtl5000_1.enable();
  sgtl5000_1.volume(.8);

  Mix1.gain(0, 0.80);
  Mix1.gain(1, 0.80);
  Mix1.gain(2, 0.80);
  Mix1.gain(3, 0.80);
  Mix2.gain(0, 0.80);
  Mix2.gain(1, 0.80);
  Mix2.gain(2, 0.80);
  Mix2.gain(3, 0.80);
  Mix3.gain(0, 0.80);
  Mix3.gain(1, 0.80);
  Mix3.gain(2, 0.80);
  Mix3.gain(3, 0.80);
}

int numOffs = 0;

void loop() {
  digitalWrite(led, HIGH);
  pin1 = touchRead(0);

//  Serial.print(" pin1 = "); Serial.print(pin1);

  // put your main code here, to run repeatedly:
  if (pin1 > 2600) {
    if (i1 == 0) {
      playMem1.play(AudioSample1); 
      fadeAmount = initialFadeAmount;
      i1 = 1;
      Serial.println("ON");
      Serial.println(pin1);  
    }
  }
  else if (pin1 <= 1999 ) {
    if (i1 == 1) {
      fadeAmount = -initialFadeAmount;
      i1 = 0;
      Serial.print("OFF ");
      Serial.println(pin1);  
    }
    playMem1.stop();
    //playMem1.play(AudioSample1);
//    Serial.print("we're stopping #1");

  }

  if (i1 == 1) {
     EVERY_N_MILLISECONDS(20) {
      FadeIn();
    }
  } else {
     EVERY_N_MILLISECONDS(20) {
      FadeOut();
    }
  }
}
void FadeIn() {

  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i].setRGB(g, r, b); // Set Color HERE!!!
    leds[i].fadeLightBy(255 - brightness);
  }
  FastLED.show();
  brightness = brightness + fadeAmount;
  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount ;
  }
}
void FadeOut() {
  if (brightness > 0) {
    brightness = brightness + fadeAmount;  
  }
  for (int i = 0; i < NUM_LEDS; i++ ) {
    leds[i].setRGB(g,r,b);  // Set Color HERE!!!
    leds[i].fadeToBlackBy(255 - brightness);
  }
  FastLED.show();
}

