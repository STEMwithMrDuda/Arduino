#include <Keyboard.h>

String text = "testing, testing, testing, testing, testing, testing, testing, testing, testing, testing, testing, testing, testing, testing, testing, testing, ";  // Define your text

void setup() {
  Serial.begin(9600);  // Start serial communication
  Keyboard.begin();
  Serial.println("Waiting for input... Type 'send' to trigger typing.");
}

void loop() {
  if (Serial.available()) {  // Check if data is received
    String command = Serial.readStringUntil('\n');  // Read input from serial
    command.trim();  // Remove any extra spaces or newlines
    
    if (command == "send") {  // Check if user typed "send"
      delay(1000);
      Keyboard.print(text);  // Type the stored text
      Serial.println("Text sent!");
    } else {
      Serial.println("Unknown command. Type 'send' to trigger.");
       }
  }
}
