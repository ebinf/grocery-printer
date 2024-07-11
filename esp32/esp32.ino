#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <SoftwareSerial.h>

#define RX 14
#define TX 12
#define VO 13

SoftwareSerial bon(RX, TX);

const char* ssid     = "<SSID>";
const char* password = "<PSK>";
// DST Root CA X3
const char* rootCACertificate = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
"PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
"Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
"AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
"rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
"OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
"xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
"7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
"aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
"HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
"SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
"ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
"AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
"R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
"JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
"Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
"-----END CERTIFICATE-----";

void asciiPrint(const String& text) {
  String ascii = text;
  ascii.replace("Ä", "\xA0");
  ascii.replace("Ö", "\xA1");
  ascii.replace("Ü", "\xA2");
  ascii.replace("ß", "\xA3");
  ascii.replace("Ã", "\xAE");
  ascii.replace("ë", "\xAF");
  ascii.replace("é", "\xB0");
  ascii.replace("è", "\xB1");
  ascii.replace("ê", "\xB3");
  ascii.replace("ï", "\xB4");
  ascii.replace("í", "\xB5");
  ascii.replace("ì", "\xB6");
  ascii.replace("î", "\xB8");
  ascii.replace("ö", "\xB9");
  ascii.replace("ó", "\xBA");
  ascii.replace("ò", "\xBB");
  ascii.replace("ô", "\xBD");
  ascii.replace("ü", "\xBE");
  ascii.replace("ú", "\xBF");
  ascii.replace("ù", "\xC0");
  ascii.replace("û", "\xC2");
  ascii.replace("Ñ", "\xC5");
  ascii.replace("ñ", "\xC6");
  ascii.replace("Å", "\xCA");
  ascii.replace("ä", "\xCD");
  ascii.replace("á", "\xCD");
  ascii.replace("à", "\xCF");
  ascii.replace("â", "\xD1");
  bon.print(ascii);
}

void setup() {
  pinMode(VO, OUTPUT);
  digitalWrite(VO, HIGH);
  Serial.begin(19200);
  WiFi.begin(ssid, password);
  bon.begin(19200);
  
  // wait for WiFi connection
  Serial.print("Waiting for WiFi to connect");
  int counter = 20;
  while (WiFi.status() != WL_CONNECTED) {
    if (counter == 0) {
      ESP.restart();
    }
    counter--;
    delay(100);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  WiFiClientSecure *client = new WiFiClientSecure;
  if(client) {
    client -> setCACert(rootCACertificate);
    {
      HTTPClient https;
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(*client, "https://<URL>/api?pretty")) {
        Serial.print("[HTTPS] GET...\n");
        int httpCode = https.GET();
        if (httpCode > 0) {
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);
            asciiPrint(payload);
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
    }
    delete client;
  } else {
    Serial.println("Unable to create client");
  }
}

void loop() {}
