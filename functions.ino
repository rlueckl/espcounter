boolean check_btn() {
  // Set up pins
  pinMode(BTN_OUT, OUTPUT);
  pinMode(BTN_IN, INPUT);

  // Check for connection between pins
  delay(25);
  digitalWrite(BTN_OUT, HIGH);
  delay(25);
  int tmp = digitalRead(BTN_IN);
  // Reset
  digitalWrite(BTN_OUT, LOW);

  // Output
  return (tmp == 1);
}

int get_value_from_api() {
  int counter = 0;
  WiFiClientSecure client;

  // Wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    client.setTrustAnchors(&cert);

    HTTPClient https;

    Serial.print("[HTTPS] Begin\n");
    // API URL in settings.ino - DO NOT CHECK IN!
    if (https.begin(client, apiurl)) {  // HTTPS

      Serial.print("[HTTPS] GET ... ");
      // Start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("code: %d\n", httpCode);

        // File found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          counter = https.getString().toInt();
          Serial.printf("[HTTPS] GET value: %i\n", counter);
        }
      } else {
        Serial.printf("failed, error: %s\n", https.errorToString(httpCode).c_str());
        my_display.String_To_Buffer("6Et FA1L", MAX7219_BRIGHTNESS);
      }

      https.end();
    } else {
      Serial.printf("unable to connect\n");
    }
  }
  return counter;
}