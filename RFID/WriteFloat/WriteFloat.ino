// import libraries
#include <SPI.h>
#include <MFRC522.h>

// configurable pinning
const uint8_t RST_PIN = 9; // reset pin
const uint8_t SS_PIN = 10; // serial data pin

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
      float temperature = sensorValue(); // read sensor value
      byte *tempBytes = (byte *)&temperature;
      byte dataBlock[BYTES_PER_BLOCK] = { 0 }; // initialize block buffer with zeros
      for (uint8_t i = 0; i < sizeof(float); i++) {
        dataBlock[i] = tempBytes[i]; // copy byte from float variable into buffer
      }
      status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(5, dataBlock, BYTES_PER_BLOCK);
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
      }
      else {
        Serial.println(F("Data written successfully"));
      }
    }
  }
}

float sensorValue() {
  return 15.09f;
}

