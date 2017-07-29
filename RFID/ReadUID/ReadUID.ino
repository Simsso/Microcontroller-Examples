// import libraries
#include <SPI.h>
#include <MFRC522.h>

// configurable pinning
const uint8_t RST_PIN = 9; // reset pin
const uint8_t SS_PIN = 10; // serial data pin

MFRC522 mfrc522(SS_PIN, RST_PIN); // create MFRC522 instance

void setup() {
  Serial.begin(9600); // start serial connection
  SPI.begin(); // start SPI bus
  mfrc522.PCD_Init(); // init RFID module
}

void loop() {
  // listen for new cards and read data
  mfrc522.PICC_IsNewCardPresent();
  mfrc522.PICC_ReadCardSerial();

  // read uid size
  for (int i = 0; i < mfrc522.uid.size; i++) {
    // print i-th ID byte in hexadecimal format
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  delay(100);
}
