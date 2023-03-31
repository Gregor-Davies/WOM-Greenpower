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

// setting up the timer.
Timer timer1;

void setup() {
  attachInterrupt(digitalPinToInterrupt(LM393), count, RISING);
  Serial.begin(115200);
  timer1.setInterval(1000);
  timer1.setCallback(Sensors);
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

// gets the sensors
void Sensors() {
  char nonFloTemp[3];
  char nonFloHeatsinkTemp[3];
  char speedBuffer[15];
  char mTemp[25];
  char hTemp[25];
  char packetToSend[40];
  // this converts the number to rpm and into kmh, the 60 is for rpm and the first number is wheel diameter in cm k = d * r * 0.001885
  int speed = 2.57 * counter * 60 * 0.001885;
  // getting the temp from the temperature sensors
  sensors.requestTemperatures();
  // getting the temp sensor at index 0 and converts the float to a string for sprintf
  float mTemperature = sensors.getTempCByIndex(0);
  dtostrf(mTemperature, 2, 0, nonFloTemp);
  // same thing but at index 1
  float hTemperature sensors.getTempCByIndex(1);
  dtostrf(hTemperature, 2, 0, nonFloHeatsinkTemp);
  // sets cursor at 0
  u8x8.setCursor(0, 0);
  // combines the speed and motor
  sprintf(speedBuffer, "SPEED: %d km/h", speed);
  sprintf(mTemp, "MOTOR TEMP: %sC", nonFloTemp);
  sprintf(hTemp, "HEATSINK TEMP: %sC", nonFloHeatsinkTemp);
  // prints the speed, then sets the cursor one down and prints the temperature
  u8x8.print(speedBuffer);
  u8x8.setCursor(0,1);  
  u8x8.print(mTemp);
  u8x8.setCursor(0,2);
  u8x8.print(hTemp);
  // Combiing the things to send as onbe packet
  snprintf(packetToSend, 40, "%s, %s", speedBuffer, mTemp);
  // begins the lora packet
  LoRa.beginPacket();
  LoRa.print(packetToSend);
  LoRa.endPacket();
  counter = 0;
  delay(3000);
  u8x8.clearDisplay();
}

void loop() {
  TimerManager::instance().update();
}