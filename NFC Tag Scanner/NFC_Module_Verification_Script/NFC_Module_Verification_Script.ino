#include <SPI.h>
#include <Adafruit_PN532.h>

#define PN532_SS   10
#define PN532_IRQ  2
#define PN532_RESET 3

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("    PN532 NFC SPI MODE");
  Serial.println("========================================\n");
  
  Serial.println("Initializing SPI...");
  SPI.begin();
  
  Serial.println("Initializing PN532...");
  nfc.begin();
  
  delay(500);
  
  Serial.println("Checking firmware...");
  uint32_t versiondata = nfc.getFirmwareVersion();
  
  if (!versiondata) {
    Serial.println("❌ PN532 NOT FOUND on SPI!");
    Serial.println("\nCheck wiring:");
    Serial.println("  MOSI -> D11");
    Serial.println("  MISO -> D12");
    Serial.println("  SCK  -> D13");
    Serial.println("  SS   -> D10");
    Serial.println("  VCC  -> 5V");
    Serial.println("  GND  -> GND");
    Serial.println("\nJumpers: 1=OFF, 2=ON (for SPI mode)\n");
    while(1);
  }
  
  Serial.println("✅ PN532 FOUND!");
  Serial.print("Firmware: ");
  Serial.print((versiondata >> 24) & 0xFF, HEX);
  Serial.print(".");
  Serial.println((versiondata >> 16) & 0xFF, HEX);
  
  nfc.SAMConfig();
  
  Serial.println("\n========================================");
  Serial.println("READY! Place NFC card on antenna.");
  Serial.println("========================================\n");
}

void loop() {
  uint8_t uid[7];
  uint8_t uidLength;
  
  Serial.print("Scanning... ");
  
  bool success = nfc.readPassiveTargetID(
    PN532_MIFARE_ISO14443A,
    uid,
    &uidLength,
    1000
  );
  
  if (success) {
    Serial.println("✅ CARD FOUND!");
    
    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      if (uid[i] < 0x10) Serial.print("0");
      Serial.print(uid[i], HEX);
      Serial.print(" ");
    }
    
    Serial.print("\nLength: ");
    Serial.println(uidLength);
    Serial.println();
    
    delay(2000);
  } else {
    Serial.println("nothing detected");
  }
}