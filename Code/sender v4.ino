#include <LoRa.h>

// display stuff
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// this stuff is for the temp sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// defining the lora pins
#define ss 18
#define rst 14
#define dio0 26

// setting up the onewirebus on pin 23
const int oneWireBus = 23;

// setting up a one wire instance
OneWire oneWire(oneWireBus);

//pass one wire under the dallas temp lib
DallasTemperature sensors(&oneWire);

// Addressing the 2 temp sensors
DeviceAddress temp1 = {0x28, 0x35, 0xF7, 0x2E, 0xA1, 0x21, 0x1, 0x8C };
DeviceAddress temp2 = {0x28, 0xC9, 0x3, 0x33, 0xA1, 0x21, 0x1, 0x9f };

// defining the oled used
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// speed sensor is on pin check out the heltec lora32 v2 pin out diagram
const int LM393 = 17;
int counter = 0;

// starting the oled
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  attachInterrupt(digitalPinToInterrupt(LM393), count, RISING);
  Serial.begin(115200);
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // setting the lora pins
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
  // Allowing the sensors time to do their stuff
  delay(500);
  char nonFloTemp[3];
  char nonFloBatteryTemp[3];
  char speedBuffer[15];
  char mTemp[25];
  char bTemp[25];
  char packetToSend[40];
  display.clearDisplay();
  // setting the colour to white and font size - this should always be white no matter what oled colour
  display.setTextColor(WHITE);
  display.setTextSize(1);
  // this converts the number to rpm and into kmh, the 60 is for rpm and the first number is wheel diameter in cm k = d * r * 0.001885
  int speed = 2.57 * counter * 60 * 0.001885;
  // getting the temp from the temperature sensors
  sensors.requestTemperatures();
  // getting the temp sensor at index 0 and converts the float to a string for sprintf
  float mTemperature = sensors.getTempC(temp1);
  dtostrf(mTemperature, 2, 0, nonFloTemp);
   // same thing but at index 1
  float bTemperature = sensors.getTempC(temp2);
  dtostrf(bTemperature, 2, 0, nonFloBatteryTemp);
  // combines the speed and motor
  sprintf(speedBuffer, "SPEED: %d km/h", speed);
  sprintf(mTemp, "MOTOR TEMP: %sC", nonFloTemp);
  sprintf(bTemp, "HEATSINK TEMP: %sC", nonFloBatteryTemp);
  // prints the speed, then sets the cursor one down and prints the temperature
  // sets cursor at 0
  display.setCursor(0,0);
  display.print(speedBuffer);
  display.setCursor(0,10);  
  display.print(mTemp);
  display.setCursor(0,20);
  display.print(bTemp);
  // Combiing the things to send as one packet
  snprintf(packetToSend, 40, "%s, %s, %s", speedBuffer, mTemp, bTemp);
  // begins the lora packet
  LoRa.beginPacket();
  LoRa.print(packetToSend);
  LoRa.endPacket();
  counter = 0;
}

void loop() {
  Sensors();
  display.display();
}