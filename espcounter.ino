/*
40000000 / year
6.341958 / 5 sec
1.268391 / sec
*/

#define MPS  1.268391 // meters per seconds
#define LOOP 5        // loop every X seconds
#define BTN_IN  4     // pin D2 for button input
#define BTN_OUT 5     // pin D1 for button output

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include <time.h>

#define MAX7219_NUM_CHIPS   1  // 3 chips = 24 digits
#define MAX7219_CS_PIN     15  // pin the MAX7219's CS pin is connected to
#define MAX7219_BRIGHTNESS  3  // brightness level 0-15

#include <MAX7219_8_Digit_Driver.h>

MAX7219_8_Digit_Driver my_display(MAX7219_CS_PIN, MAX7219_NUM_CHIPS);

// Include parameters for WiFi and API
#include "mySettings.h"

// Let's Encrypt root cert
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

double prod = 0.0;

#if !defined(__time_t_defined)
typedef double time_t;
#endif

void setup() {
  Serial.begin(115200); delay(10);
  Serial.println();

  my_display.String_To_Buffer("   H1   ", MAX7219_BRIGHTNESS);

  // SSID and password are in mySettings.h - DO NOT CHECK IN!
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

  time_t ts_now = time(nullptr);

  while (ts_now < 8*3600*2) {
    delay(500);
    Serial.print(".");
    ts_now = time(nullptr);
  }
  Serial.println();

  struct tm time_now;
  gmtime_r(&ts_now, &time_now);
  Serial.printf("[TIME] now: %s", asctime(&time_now));

  time_t ts_start;
  struct tm time_start;

  time_start.tm_year = time_now.tm_year;
  time_start.tm_mon = 0;
  time_start.tm_mday = 1;
  time_start.tm_hour = 0;
  time_start.tm_min = 0;
  time_start.tm_sec = 0;
  time_start.tm_isdst = time_now.tm_isdst; // We already have the current time, get DST from that.
  ts_start = mktime(&time_start) + 3*3600; // Add some hours to get to GMT. See also timegm() and timezone foo.

  Serial.printf("[TIME] year start: %s", asctime(&time_start));

  time_t ts_diff;
  ts_diff = ts_now - ts_start;
  double ts_diffd = (double) ts_diff;
  Serial.printf("[TIME] diff in seconds: %.2f\n", ts_diffd);

  // ts_diff               :  seconds since Jan 1
  // MPS                   :  how many meters per second are done
  // prod = ts_diff * MPS  :  this years production at boot time
  prod = ts_diffd * MPS;
  Serial.printf("[PROD] at boot: %.2f\n", prod);

  // Reset pins at boot
  pinMode(BTN_OUT, OUTPUT);
  digitalWrite(BTN_OUT, LOW);
}

void loop() {
  double corr = 0.0;
  // Get correction difference from API
  if (check_btn()) {
    Serial.println("[DEBUG] Button pressed, get corrections.");
    my_display.String_To_Buffer(" UPDATE ", MAX7219_BRIGHTNESS);
    corr = get_value_from_api();
    Serial.printf("[DEBUG] Correct by: %.2f\n", corr);
  }

  prod = prod + (MPS * LOOP) + corr;

  // The decimal point does not count against the length of the character string
  //my_display.String_To_Buffer(String(counter++), MAX7219_BRIGHTNESS);
  my_display.String_To_Buffer(String(prod, 0), MAX7219_BRIGHTNESS);
  Serial.printf("[PROD] current: %.2f\n", prod);
  delay(LOOP*1000-50);
}
