#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <TOTP.h>

// WiFi credentials
const char* ssid = "WIFI SSID";
const char* password = "WIFI PASSWORD";

// NTP server settings
const long utcOffsetInSeconds = 0;
const char* ntpServer = "192.168.0.1"; // your NTP server

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, utcOffsetInSeconds);

// Display setup
SSD1306Wire display(0x3c, SDA, SCL, GEOMETRY_128_32);

// OTP secret keys
byte secret1[] = { /* secret key */ };
byte secret2[] = { /* secret key */ };

TOTP totp1 = TOTP(secret1, sizeof(secret1));
TOTP totp2 = TOTP(secret2, sizeof(secret2));

void setup() {
    // Initialize display
    display.init();
    display.displayOn();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_16);
    display.clear();

    // Display WiFi connection message
    display.drawString(0, 0, "Connect WiFi...");
    display.display();

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    // Display NTP sync message
    display.drawString(0, 0, "Getting Time");
    display.display();

    // Initialize time client
    timeClient.begin();
    timeClient.forceUpdate();

    // Clear the display after setup
    display.clear();
}

void loop() {
    timeClient.update();
    displayOTP();
    delay(5000);
}

void displayOTP() {
    display.clear();

    // Calculate and display OTP
    String otpString1 = totp1.getCode(timeClient.getEpochTime());
    String otpString2 = totp2.getCode(timeClient.getEpochTime());
    display.drawString(12, 0, "OTP1");
    display.drawString(64, 0, otpString1);
    display.drawString(12, 16, "OTP2");
    display.drawString(64, 16, otpString2);

    display.display();
}
