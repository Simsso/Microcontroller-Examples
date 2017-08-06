// import libraries
#include <SPI.h>
#include <MFRC522.h>

const uint8_t RST_PIN = 9; // reset pin
const uint8_t SS_PIN = 10; // serial data pin

const uint8_t UID_LENGTH = 4, UID_COUNT = 2;

const uint8_t UNLOCK_PIN = 2;

const uint8_t RGB_LED_COUNT = 8;

const uint8_t PERMITTED_UIDS[UID_COUNT][UID_LENGTH] = {
  { 0xB5, 0xA3, 0x2B, 0x1B },
  { 0x3A, 0x06, 0xE8, 0xAB }
};

MFRC522 mfrc522(SS_PIN, RST_PIN); // create MFRC522 instance

void setup() {
  digitalWrite(UNLOCK_PIN, LOW);
  pinMode(UNLOCK_PIN, OUTPUT);
  
  Serial.begin(9600); // start serial connection
  SPI.begin(); // start SPI bus
  mfrc522.PCD_Init(); // init RFID module
}

void loop() {  
  // listen for new cards and read data
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // check uid size
    if (mfrc522.uid.size != UID_LENGTH) {
      Serial.println("UID length missmatch");
    }
    else if (isPermitted(mfrc522.uid.uidByte)) {
      Serial.println("Unlocked");
      digitalWrite(UNLOCK_PIN, HIGH);
      delay(500);
      digitalWrite(UNLOCK_PIN, LOW);
      delay(1500);
    }
    else {
      Serial.println("Locked");
      delay(1000);
    }
    
    mfrc522.PICC_HaltA();
  }
}

bool isPermitted(uint8_t *uid) {
  for (uint16_t uidIndex = 0; uidIndex < UID_COUNT; uidIndex++) {
    bool match = true;
    for (uint8_t byteIndex = 0; byteIndex < UID_LENGTH; byteIndex++) {
      if (PERMITTED_UIDS[uidIndex][byteIndex] != uid[byteIndex]) {
        match = false;
      }
    }
    if (match) return true;
  }
  return false;
}
