#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Fonts/FreeSans9pt7b.h>

#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

#define TFT_CS 10
#define TFT_RST 7
#define TFT_DC 6

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define coinSlotPin 8
#define chargingPort 9 

bool insertedCoin = false;
int totalCoin = 0;
int coinSlotStatus;
int chargeTime = 10;

unsigned long previousMillis = 0;
const long interval = 1000;

// default values, there's no coin inserted
int hourUnit = 0;
int minUnit = 0;
int secUnit = 0;

int minOffSetValue = 0;
bool countdown = false;

void setup() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);
 
  pinMode(coinSlotPin, INPUT_PULLUP);
  pinMode(chargingPort, OUTPUT);
  
        tft.fillScreen(ST7735_BLACK);
      tft.setFont(&FreeSansBold9pt7b);
      tft.setCursor((tft.width() - 150) / 1, (tft.height() - 60) / 1.90); // Centered position
      tft.println("  Insert Coin to ");
      tft.setFont(&FreeSansBold9pt7b);
      tft.setCursor((tft.width() - 150) / 1, (tft.height() - 20) / 1.70); // Centered position
      tft.println(" Start Charging");
      tft.setFont(&FreeSans9pt7b);
      tft.setCursor((tft.width() - 160) / 1, (tft.height() - 1) / 1.30); // Centered position
      tft.println("1 Peso=10 Minutes");
  
}

void loop() {
  while (!insertedCoin) {
    coinSlotStatus = digitalRead(coinSlotPin);
    timer();

    if (coinSlotStatus == 0) {
      countdown = true;
      insertedCoin = true;
      totalCoin += 1;
      minUnit = (totalCoin * chargeTime) + minOffSetValue;

      if (minUnit >= 60) {
        hourUnit += 1;
        minUnit = minUnit - 60;

        if (minUnit > 0) {
          minOffSetValue = minUnit;
        }
        totalCoin = 0;
      }
    }
  }

  while (insertedCoin) {
    coinSlotStatus = digitalRead(coinSlotPin);

    if (coinSlotStatus == 1) {
      insertedCoin = false;
    }
  }
}

void timer() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (countdown) {
      printTime();
      countDown();
    }
  }
}

void printTime() {
  if(countdown){
    digitalWrite(chargingPort, HIGH);
    
  tft.fillScreen(ST7735_BLACK);
    tft.setFont(&FreeSansBold18pt7b);
  tft.setCursor((tft.width() - 145) / 1, (tft.height() - 20) / 1.45); // Centered position
  

  if (hourUnit < 10) {
    tft.print("0");
  }
  tft.print(hourUnit);
  tft.print(":");

  if (minUnit < 10) {
    tft.print("0");
  }
  tft.print(minUnit);
  tft.print(":");

  if (secUnit < 10) {
    tft.print("0");
  }
  tft.print(secUnit);
  }
}

void countDown() {
  if (secUnit > 0) {
    secUnit -= 1;
  } else {
    if (minUnit > 0) {
      minUnit -= 1;
      secUnit = 59;
    } else {
      totalCoin = 0;
      countdown = false;
      digitalWrite(chargingPort, LOW);
      tft.fillScreen(ST7735_BLACK);
      tft.setFont(&FreeSansBold9pt7b);
      tft.setCursor((tft.width() - 150) / 1, (tft.height() - 60) / 1.90); // Centered position
      tft.println("  Insert Coin to ");
      tft.setFont(&FreeSansBold9pt7b);
      tft.setCursor((tft.width() - 150) / 1, (tft.height() - 20) / 1.70); // Centered position
      tft.println(" Start Charging");
      tft.setFont(&FreeSans9pt7b);
      tft.setCursor((tft.width() - 160) / 1, (tft.height() - 1) / 1.30); // Centered position
      tft.println("1 Peso=10 Minutes");
    }
  }
}


