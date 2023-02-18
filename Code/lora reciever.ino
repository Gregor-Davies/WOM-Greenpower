#include <U8x8lib.h>
#include <LoRa.h>
#include "timer.h"

#define ss 18
#define rst 14
#define dio0 26

Timer timer;
//oled init
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

const int LM393 = 17;
int counter = 0;

void setup() {
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(866E6);
  LoRa.setSyncWord(0xF3);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      u8x8.print(LoRaData);
      u8x8.setCursor(0, 1);
    }
    u8x8.print("RSSI: ");
    u8x8.print(LoRa.packetRssi());
    delay(3000);
    u8x8.clear();

  }
}
