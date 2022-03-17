#include <FastLED.h>

#define LED_PIN     3
#define NUM_LEDS    60

int comm = 1;

CRGB leds[NUM_LEDS];
char *colourList[] = {
  "000000",
  "000000",
  "000000",
  "000000",
  "FF0000",
  "FF9900",
  "CCFF00",
  "33FF00",
  "00FF66",
  "00FFFF",
  "0066FF",
  "3300FF",
  "CC00FF",
  "FF0099"
};


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  //flashRed();
  //chargingBlue();
  //randomLightUp();

  int adcPin = A2;
 
  int potVal = analogRead(adcPin);
  int brightness = map(potVal, 0, 1023, 0, 255);
  FastLED.setBrightness(brightness);
  
  if (Serial.available()) {
    comm = Serial.read();
    Serial.write(comm);
  }

  if (comm == 1) {
    chargingBlue();
  } else if (comm == 2) {
    flashRed();
  } else {
    randomLightUp();
  }
}

void randomLightUp( void) {
  int n;
  unsigned long int val;
  
  for (n=0; n<20;n++) {
    val = strtoul(colourList[random(0,14)],NULL, 16);
    leds[random(1,NUM_LEDS)] = val;
  }
  FastLED.show();
  delay(20);
}
void chargingBlue (void) {
  int n;
  for (n=0; n<NUM_LEDS/2; n++) {
    leds[NUM_LEDS/2+n] = CRGB::Blue;
    leds[NUM_LEDS/2-n-1] = CRGB::Blue;
    FastLED.show();
    delay(50); 
  }
  FastLED.clear();
}

void flashRed(void) {
  int n;
  
  for (n=0; n<NUM_LEDS; n++) {
    leds[n] = CRGB::Red;
  }
  FastLED.show();
  delay(300);
  for (n=0; n<NUM_LEDS; n++) {
    leds[n] = CRGB::Black;
  }
  FastLED.show();
  delay(100);

  for (int i=0; i<3; i++) {
    for (n=0; n<NUM_LEDS; n++) {
      leds[n] = CRGB::Red;
    }
    FastLED.show();
    delay(100);
    for (n=0; n<NUM_LEDS; n++) {
      leds[n] = CRGB::Black;
    }
    FastLED.show();
    delay(100);
  }
}

int StrToHex(char str[])
{
  return (int) strtol(str, 0, 16);
}
