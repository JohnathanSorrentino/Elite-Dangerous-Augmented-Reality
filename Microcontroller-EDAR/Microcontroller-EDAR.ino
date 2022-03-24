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

char *colourListFull[] = {
  "FF0000",
  "FF1900",
  "FF3300",
  "FF4D00",
  "FF6600",
  "FF8000",
  "FF9900",
  "FFB300",
  "FFCC00",
  "FFE500",
  "FFFF00",
  "E6FF00",
  "CCFF00",
  "B3FF00",
  "99FF00",
  "80FF00",
  "66FF00",
  "4DFF00",
  "33FF00",
  "1AFF00",
  "00FF00",
  "00FF19",
  "00FF33",
  "00FF4C",
  "00FF66",
  "00FF80",
  "00FF99",
  "00FFB3",
  "00FFCC",
  "00FFE5",
  "00FFFF",
  "00E6FF",
  "00CCFF",
  "00B2FF",
  "0099FF",
  "0080FF",
  "0066FF",
  "004CFF",
  "0033FF",
  "0019FF",
  "0000FF",
  "1900FF",
  "3300FF",
  "4C00FF",
  "6600FF",
  "8000FF",
  "9900FF",
  "B300FF",
  "CC00FF",
  "E600FF",
  "FF00FF",
  "FF00E6",
  "FF00CC",
  "FF00B3",
  "FF0099",
  "FF0080",
  "FF0066",
  "FF004D",
  "FF0033",
  "FF001A"
};


void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //flashRed();
  //chargingBlue();
  //randomLightUp();
  int adcPin = A2;
  int potValue, brightness;
  unsigned long int x, val;
  
  potValue = analogRead(adcPin);
  brightness = map(potValue, 0, 1023, 0, 255);
  FastLED.setBrightness(brightness);
  
  if (Serial.available()) {
    x = Serial.parseInt();
    Serial.println(x+1);
  }
  switch(x)
    {
      case 0: setAllLeds(0xff0000); break;
      case 1: flashRed(); break;
      case 2: chargingBlue(); break;
      case 3: randomLightUp(); break;
      case 4: setAllLeds(0xffffff); break;
      default: rainbow(); break;//setAllLeds(0xff0089); break;
    }
}

void setAllLeds(unsigned long int colour) {
  int n;
  for (n=0; n< NUM_LEDS; n++){
    leds[n] = colour;
  }
  FastLED.show();
}

void rainbow( void ) {
  int n;
  unsigned long int val;
  
  for (n=0; n<NUM_LEDS; n++) {
    val = strtoul(colourListFull[n], NULL, 16);
    leds[n] = val;
  }
  FastLED.show();
  delay(20);
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
