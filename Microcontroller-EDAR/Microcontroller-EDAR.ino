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
  Serial.setTimeout(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte serialByte;
  int adcPin = A2;
  int potValue, brightness, i, n;
  unsigned long int pythonMsg;
  unsigned long int pythonMsgTemp = 0;
  unsigned long int val;
  
  potValue = analogRead(adcPin);
  brightness = map(potValue, 0, 1023, 0, 255);
  FastLED.setBrightness(brightness);
  
  i = 0;
  while (Serial.available() > 0) {
    serialByte = Serial.read();
    Serial.println(serialByte);
    pythonMsgTemp |= (long) serialByte << (i*8);
    i++;
    if (i == 4) {
      pythonMsg = pythonMsgTemp;
      Serial.println(pythonMsg);
    }
  }
  
  // FSD Jump
  if ((pythonMsg >> 30) & 1) {
    randomLightUp();
  }
  // In Danger, Being Interdicted, Overheating
  else if (((pythonMsg >> 22) & 1) | ((pythonMsg >> 23) & 1) | ((pythonMsg >> 20) & 1)) {
    flashRed();
  }
  // FSD Charging
  else if ((pythonMsg >> 17) & 1) {
    chargingBlue();
  } 
  // Landing Gear Down
  else if((pythonMsg >> 2) & 1) {
    docked();
  } else {
    // Shields Up
    if ((pythonMsg >> 3) & 1) {
      for (n=20; n<40; n++) {
        leds[n] = 0x0066FF;
      }
    } else {
      for (n=20; n<40; n++) {
        leds[n] = 0xff0000;
      }
    }
    // HUD in Analysis Mode
    if ((pythonMsg >> 27) & 1) {
      for (n=8; n<20; n++) {
        leds[n] = 0x0019FF;
      }
      for (n=40; n<52; n++) {
        leds[n] = 0x0019FF;
      }
    } else {
      for (n=8; n<20; n++) {
        leds[n] = 0xff1900;
      }
      for (n=40; n<52; n++) {
        leds[n] = 0xff1900;
      }
    }
    // Hardpoints Deployed
    if ((pythonMsg >> 6) & 1) {
      for (n=0; n<8; n++) {
        leds[n] = 0xff0000;
      }
    } else {
      for (n=0; n<8; n++) {
        leds[n] = 0x000000;
      }
    }
    // Flight Assist Off
    /*if ((pythonMsg >> 5) & 1) {
      leds[0] = 0x00ff00;
    } else {
      leds[0] = 0xff0000;
    }*/
    // Mass Locked
    if ((pythonMsg >> 16) & 1){
      for (n=52; n<60; n++) {
        leds[n] = 0xff0000;
      }
    } else {
      for (n=52; n<60; n++) {
        leds[n] = 0x000000;
      }
    }
    FastLED.show();
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
  FastLED.clear();
  
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
   
  setAllLeds(0x000000);

  for (int i=0; i<3; i++) {
    setAllLeds(0xff0000);
    delay(100);
    setAllLeds(0x000000);
    delay(100);
  }

  setAllLeds(0xff0000);
  delay(500);
  setAllLeds(0x000000);
  delay(100);
}

void docked( void ) {
  static int ping = 0;
  int i;
  
  FastLED.clear();
  if (ping == 0) {
    for (i=0; i<NUM_LEDS/2; i++) {
      leds[i*2] = 0xffffff;
    }
    ping = 1;
  } else {
    for (i=0; i<NUM_LEDS/2; i++) {
      leds[i*2+1] = 0xffffff;
    }
    ping = 0;
  }
  FastLED.show();
  delay(1000);  
}

int StrToHex(char str[])
{
  return (int) strtol(str, 0, 16);
}
