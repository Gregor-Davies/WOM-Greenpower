#include "timer.h"
#include <U8x8lib.h>
#include <LoRa.h>
#include "timerManager.h"

// this stuff is for the temp sensor
#include <OneWire.h>
#include <DallasTemperature.h>

#define ss 18
#define rst 14
#define dio0 26

// setting up the onewirebus on pin 23
const int oneWireBus = 23;

// setting up a one wire instance
OneWire oneWire(oneWireBus);

//pass one wire under the dallas temp lib
DallasTemperature sensors(&oneWire);

Timer timer;
//oled used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// speed sensor is on pin check out the heltec lora32 v2 pin out diagram
const int LM393 = 17;
int counter = 0;

// setting up the 2 timers.
Timer timer1;
Timer timer2;

void setup() {
  attachInterrupt(digitalPinToInterrupt(LM393), count, RISING);
  Serial.begin(115200);
  timer1.setInterval(1000);
  timer1.setCallback(KMH);
  timer2.setInterval(1000);
  timer2.setCallback(temp);
  TimerManager::instance().start();
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  LoRa.setPins(ss, rst, dio0);
  LoRa.begin(866E6);
  LoRa.setSyncWord(0xF3);
  sensors.begin();
}

void count() {
  counter++;
}

// gets the kmh
void KMH() {
  // this converts the number to rpm and into kmh, the 60 is for rpm and the first number is wheel diameter in cm k = d * r * 0.001885
  int speed = 2.57 * counter * 60 * 0.001885;
  u8x8.setCursor(0, 0);
  char buf[15];
  sprintf(buf, "SPEED: %d km/h", speed);
  u8x8.print(buf);
  LoRa.beginPacket();
  LoRa.print(buf);
  LoRa.endPacket();
  counter = 0;
  delay(3000);
  u8x8.clearDisplay();
}

// This gets temp1
void temp() {
  sensors.requestTemperatures();
  // use different index for the other sensor
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  delay(3000);
}

void loop() {
  TimerManager::instance().update();
}