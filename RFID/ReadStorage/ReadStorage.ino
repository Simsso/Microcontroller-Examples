// import libraries
#include <SPI.h>
#include <MFRC522.h>

// configurable pinning
const uint8_t RST_PIN = 9; // reset pin
const uint8_t SS_PIN = 10; // serial data pin

const uint8_t SECTOR_COUNT = 16; // usually either 16 or 32
const uint8_t BYTES_PER_BLOCK = 16; 

MFRC522 mfrc522(SS_PIN, RST_PIN); // create MFRC522 instance

MFRC522::MIFARE_Key key; // access key

void setup() {
  Serial.begin(9600); // start serial connection
  SPI.begin(); // start SPI bus
  mfrc522.PCD_Init(); // init RFID module

  // initialize with default key value
  for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // new card present

    // authenticate
    byte trailerBlock = 7; // stores the key
    MFRC522::StatusCode status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status == MFRC522::STATUS_OK) {
      
      // iterate over sectory
      for (uint8_t i = 0; i < SECTOR_COUNT; i++) {
        // print i-th sector content
        mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, i);
      }


      // read block content
      uint16_t blockAddress = 4; // value in [0,63]
      uint8_t bufferSize = BYTES_PER_BLOCK + 2;
      uint8_t buffer[bufferSize];
      status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddress, buffer, &bufferSize);
      if (status == MFRC522::STATUS_OK) {
        // read success
        Serial.println(F("Block content"));
        // iterate over block bytes
        for (uint8_t i = 0; i < BYTES_PER_BLOCK; i++) {
          Serial.println(buffer[i], HEX);
        }
        Serial.println();
      }
      else {
        Serial.println(F("Block read failed"));
        Serial.println(mfrc522.GetStatusCodeName(status));
      }
    }
  }
}
