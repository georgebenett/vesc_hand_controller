#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);


void setup() {



  tft.init();

    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    sprite.createSprite(240,320);
    sprite.setSwapBytes(true);
    sprite.setTextDatum(4);



}

void loop() {
  sprite.fillSprite(TFT_BLACK);
  sprite.drawString("48",120,140, 8);
  sprite.drawString("TRIP",40,280, 4);
  sprite.pushSprite(0,0);

}