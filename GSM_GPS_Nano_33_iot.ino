// Use Serial1 for the GPS module (hardware serial)
#define GPS_SERIAL Serial1

// Use Serial2 for the GSM module (hardware serial)
#define GSM_SERIAL Serial1

void setup() {
  Serial.begin(9600);      // Start the serial communication with the computer
  GPS_SERIAL.begin(9600);  // Start the hardware serial communication with the GPS module
  GSM_SERIAL.begin(9600);  // Start the hardware serial communication with the GSM module

  // Initialize the GSM module
  GSM_SERIAL.println("AT");
  delay(1000);
  while (GSM_SERIAL.available()) {
    Serial.write(GSM_SERIAL.read());
  }
  // Set the GSM module to text mode
  GSM_SERIAL.println("AT+CMGF=1");
  GSM_SERIAL.println("ATD+254748613509;");  // Replace with the desired phone number
  delay(10000);
  Serial.println("Calling...");
}
void loop() {
  if (GPS_SERIAL.available()) {
    String sentence = GPS_SERIAL.readStringUntil('\n');  // Read a line from GPS module

    // Check if the sentence contains latitude and longitude
    if (sentence.startsWith("$GPGGA")) {
      // Split the sentence into individual parts
      char data[100];
      sentence.toCharArray(data, 100);

      // Extract latitude and longitude
      char *token = strtok(data, ",");
      for (int i = 0; i < 6; i++) {
        token = strtok(NULL, ",");
      }
      if (token != NULL) {
        float latitude = atof(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
          float longitude = atof(token);
          Serial.print("Latitude: ");
          Serial.print(latitude, 6);  // Print latitude with 6 decimal places
          Serial.print(", Longitude: ");
          Serial.println(longitude, 6);  // Print longitude with 6 decimal places

          // Check if latitude and longitude are valid (not zero or near zero)
          if (isValidLocation(latitude, longitude)) {
            // Make a call using the GSM module
            // Set the phone number you want to call
            GSM_SERIAL.println("ATD+254748613509;");  // Replace with the desired phone number
            delay(10000);
            Serial.println("Calling...");
          }
        }
      }
    }
  }
}
bool isValidLocation(float lat, float lon) {
  // Check if latitude and longitude are valid (not zero or near zero)
  // You can adjust the threshold as needed
  return (lat != 0.0 && lon != 0.0);
}
