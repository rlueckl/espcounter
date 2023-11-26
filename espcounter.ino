/*
40000000 / year
6.342 / 5 sec
1.268 / sec
*/

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define MAX7219_NUM_CHIPS   1  // 3 chips = 24 digits
#define MAX7219_CS_PIN     15  // pin the MAX7219's CS pin is connected to
#define MAX7219_BRIGHTNESS  3  // brightness level 0-15

#include <MAX7219_8_Digit_Driver.h>

MAX7219_8_Digit_Driver my_display(MAX7219_CS_PIN, MAX7219_NUM_CHIPS);

// Wifi
#include "mySettings.h"

// Let'sEncrypt root cert
const char IRG_Root_X1 [] PROGMEM = R"CERT(
-----BEGIN CERTIFICATE-----
MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB
AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC
ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL
wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D
LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK
4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5
bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y
sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ
Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4
FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc
SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql
PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND
TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw
SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1
c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx
+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB
ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu
b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E
U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu
MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC
5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW
9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG
WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O
he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC
Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5
-----END CERTIFICATE-----
)CERT";

X509List cert(IRG_Root_X1);

int counter = 0;

#include <time.h>

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();

  my_display.String_To_Buffer("   H1   ", MAX7219_BRIGHTNESS);

  // ssid & pass in settings.ino - DO NOT CHECK IN!
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to "); Serial.print(ssid); Serial.println(" ...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("Connected, IP: "); Serial.println(WiFi.localIP());

  my_display.String_To_Buffer(WiFi.localIP().toString(), MAX7219_BRIGHTNESS);
  delay(3000);

  configTime(3*3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");

  time_t now = time(nullptr);

  while (now < 8*3600*2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println();

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

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
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }

  struct tm t;
  time_t t_of_day;
  t.tm_year = timeinfo.tm_year;
  t.tm_mon = 0;
  t.tm_mday = 1;
  t.tm_hour = 0;
  t.tm_min = 0;
  t.tm_sec = 0;
  t.tm_isdst = timeinfo.tm_isdst; // We already have the current time, get DST from that.
  t_of_day = mktime(&t) + 3*3600; // Add some hours to get to GMT. See also timegm() and timezone foo.
  Serial.println(t_of_day);


  //Serial.println(timest_jan1);
}

void loop() {
  // The decimal point does not count against the length of the character string
  my_display.String_To_Buffer(String(counter++), MAX7219_BRIGHTNESS);
  delay(1000);
}
