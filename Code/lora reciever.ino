// lora libraries
#include <LoRa.h>

// lora pins
#define ss 18
#define rst 14
#define dio0 26

// display stuff
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// defining the oled used
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// starting the oled
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  LoRa.setPins(ss, rst, dio0);
  Serial.begin(115200);
  LoRa.begin(866E6);
  LoRa.setSyncWord(0xF3);
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
}

void getLora() {
  delay(500);
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
      // read the string from lora
    String receivedString = LoRa.readString();

    char charArray[receivedString.length() + 1]; // creates a char array the length of the packet length but +1
    receivedString.toCharArray(charArray, receivedString.length() + 1); // creates the char array

    // split the charArray
    char* ptr = strtok(charArray, ","); // parses the char array by the "," delimiter which is set on the transmitter side
    String speed(ptr);
    ptr = strtok(NULL, ",");
    String mTemp(ptr);
    ptr = strtok(NULL, ",");
    String bTemp(ptr);

    display.clearDisplay();
    display.setTextColor(WHITE); // this should always be white
    display.setTextSize(1);

    // Displays the split strings
    display.setCursor(0,0);
    display.print(speed);
    display.setCursor(0, 10);
    display.print(mTemp);
    display.setCursor(0,20);
    display.print(bTemp);

    }
  }
}

void loop() {
  getLora();
  display.display();
}