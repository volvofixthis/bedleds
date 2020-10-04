#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <Servo.h>
#include <WiFiUdp.h>
#include "FastLED.h"

#ifndef STASSID
#define STASSID "iammi"
#define STAPSK "reqpythonplz"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

const int led = D0;

WiFiUDP UDPTestServer;
unsigned int UDPPort = 2807;

#define LED_DT    D2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define LED_COUNT 60
CRGB leds[LED_COUNT];
#define BRIGHTNESS          96

struct LedRGB {
    byte R;
    byte G;
    byte B;
};

LedRGB cur_led;

byte cmd;

unsigned long last_high_cmd;

void one_color_all(int cred, int cgrn, int cblu) {
    for(int i = 0 ; i < LED_COUNT; i++ ) {
        leds[i].setRGB( cred, cgrn, cblu);
    }
    FastLED.show();
}

void color_all_from_udp() {
    for(int i = 0; i < LED_COUNT; i++) {
        UDPTestServer.read((char *)(&cur_led), 3);
        leds[i].setRGB(cur_led.R, cur_led.G, cur_led.B);
    }
    FastLED.show();
}

void setup(void)
{
    pinMode(led, OUTPUT);
    digitalWrite(led, HIGH);
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("bedleds2"))
    {
        Serial.println("MDNS responder started");
    }

    UDPTestServer.begin(UDPPort); 

    FastLED.addLeds<WS2812B, LED_DT, COLOR_ORDER>(leds, LED_COUNT);
    one_color_all(0, 0, 255);
    last_high_cmd = millis();
}

void handleUDPServer() {
    int cb = UDPTestServer.parsePacket();
    if (cb) {
        UDPTestServer.read(&cmd, 1);
        if(cmd == 'c') {
            one_color_all(229, 9, 249);
        }
        if(cmd == 'd') {
            one_color_all(0, 0, 0);
        }
        if((cmd == 's') && (cb == 1 + LED_COUNT * 3) && (millis() - last_high_cmd > 10000)) {
            color_all_from_udp();
        }
        if(cmd == 'h' && cb == 1 + LED_COUNT * 3) {
            last_high_cmd = millis();
            color_all_from_udp();
        }
        // Serial.println(myData);
    }
}

void loop(void)
{
    MDNS.update();
    handleUDPServer();
}
