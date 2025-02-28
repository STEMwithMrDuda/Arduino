#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9  // Reset pin for RFID module
#define SS_PIN          10 // Slave select pin for RFID module
#define BUZZER_PIN      6  // Pin for piezo buzzer

MFRC522 mfrc522(SS_PIN, RST_PIN);

int incorrectScanCount = 0;  // Counter for incorrect scans
const int maxIncorrectScans = 3;  // Max number of incorrect scans before alarm
bool alarmActive = false;  // Track if the alarm is active
unsigned long alarmStartMillis = 0;  // Time when alarm was triggered

//*****************************************************************************************//
void setup() {
  Serial.begin(9600);          // Initialize serial communication
  SPI.begin();                 // Start SPI communication
  mfrc522.PCD_Init();          // Initialize RFID module
  pinMode(BUZZER_PIN, OUTPUT); // Set buzzer pin as output
  Serial.println(F("Scan your RFID card:"));
}

//*****************************************************************************************//
void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // Exit loop if no new card is detected
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // Exit loop if card read fails
  }

  Serial.println(F("**Card Detected:**"));
  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); // Print card details
  
  // Read the first and last names from the card
  String firstName = readNameFromCard(4); // Block 4 for first name
  String lastName = readNameFromCard(1);  // Block 1 for last name

  Serial.print(firstName);
  Serial.print(" ");
  Serial.println(lastName);

  // Check if the scanned card belongs to "Green Goblin"
  bool isGreenGoblin = (firstName == "Green" && lastName == "Goblin");

  // If "Green Goblin" is scanned, reset the alarm immediately and play a happy tone
  if (isGreenGoblin) {
    playHappyTone();  // Play a happy tone
    resetAlarm();     // Reset the alarm
  } else {
    playSadTone();    // Play a sad tone
    incorrectScanCount++;  // Increment incorrect scan count
  }

  // Trigger the alarm if there have been too many incorrect scans
  if (incorrectScanCount > maxIncorrectScans && !alarmActive) {
    Serial.println(F("Too many incorrect scans! Alarm triggered."));
    soundAlarm();
    alarmActive = true;  // Set alarm as active
    alarmStartMillis = millis();  // Record the start time of the alarm
  }

  // Automatically turn off the alarm after 30 seconds if no "Green Goblin" card is scanned
  if (alarmActive && millis() - alarmStartMillis >= 30000) {
    Serial.println(F("Alarm timed out after 30 seconds."));
    resetAlarm();
  }

  Serial.println(F("\n**End Reading**\n"));
  delay(100);  // Short delay before the next scan
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

//*****************************************************************************************//
String readNameFromCard(byte block) {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // Set authentication key

  byte buffer[18];
  byte len = 18;
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }

  status = mfrc522.MIFARE_Read(block, buffer, &len);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return "";
  }

  // Extract name from buffer and return as string
  String name = "";
  for (uint8_t i = 0; i < 16; i++) {
    if (buffer[i] != 32) {  // Ignore padding spaces (32 is ASCII for space)
      name += (char)buffer[i];
    }
  }
  name.trim();  // Remove any trailing spaces
  return name;
}

//*****************************************************************************************//
void playHappyTone() {
  tone(BUZZER_PIN, 1000, 200);  // Play a 1000 Hz tone for 200ms
  delay(250);
  tone(BUZZER_PIN, 1200, 200);  // Play a 1200 Hz tone for 200ms
  delay(250);
}

void playSadTone() {
  tone(BUZZER_PIN, 400, 300);  // Play a 400 Hz tone for 300ms
  delay(350);
  tone(BUZZER_PIN, 300, 300);  // Play a 300 Hz tone for 300ms
  delay(350);
}

//*****************************************************************************************//
void soundAlarm() {
  // Play a loud alarm if too many incorrect scans
  for (int i = 0; i < 5; i++) {
    tone(BUZZER_PIN, 1500); // Play a 1500 Hz tone
    delay(500);             // Wait for 500ms
    noTone(BUZZER_PIN);     // Stop the tone
    delay(500);             // Wait for 500ms
  }
}

//*****************************************************************************************//
void resetAlarm() {
  alarmActive = false;  // Deactivate the alarm immediately
  incorrectScanCount = 0;  // Reset the incorrect scan counter
  noTone(BUZZER_PIN);  // Stop the buzzer immediately
  Serial.println(F("Alarm reset!"));
}
