// by Ray Burnette 20161013 compiled on Linux 16.3 using Arduino 1.6.12
// ripped off by Jeffrey Wubbenhorst
// ESP8266 Timer Example
// SwitchDoc Labs  October 2015
// derived from:
// http://www.switchdoc.com/2015/10/iot-esp8266-timer-tutorial-arduino-ide/

#include <stdio.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>
#define PIN 14
#define PI 3.14159265
uint16_t i = 0;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);
double x = 0;

#include <ESP8266WiFi.h>
#include "./functions.h"

#define disable 0
#define enable  1
// uint8_t channel = 1;
unsigned int channel = 1;


extern "C" {
#include "user_interface.h"
}

os_timer_t myTimer;

bool tickOccured;

// start of timerCallback
void timerCallback(void *pArg) {

  // add LED stuff
  // tickOccured = true;
  float val = (exp(sin(x / 2 * PI)) - 0.36787944) * 108.0;
  for (i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, val));
  }
  strip.show();
  x = x + 0.05; // runs through a cycle every 2 seconds 

} // End of timerCallback

void user_init(void) {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 50, true); // timer runs every 50ms
} // End of user_init



void setup() {
  // get neopixels going
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(57600);
  Serial.printf("\n\nSDK version:%s\n\r", system_get_sdk_version());
  Serial.println(F("ESP8266 mini-sniff by Ray Burnette http://www.hackster.io/rayburne/projects"));
  Serial.println(F("Type:   /-------MAC------/-----WiFi Access Point SSID-----/  /----MAC---/  Chnl  RSSI"));

  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb);   // Set up promiscuous callback
  wifi_promiscuous_enable(enable);
   user_init();
}

void loop() {
  channel = 1;
  wifi_set_channel(channel);
  while (true) {
    nothing_new++;                          // Array is not finite, check bounds and adjust if required
    if (nothing_new > 200) {
      nothing_new = 0;
      channel++;
      if (channel == 15) break;             // Only scan channels 1 to 14
      wifi_set_channel(channel);
    }
    delay(1);  // critical processing timeslice for NONOS SDK! No delay(0) yield()
    // Press keyboard ENTER in console with NL active to repaint the screen
    if ((Serial.available() > 0) && (Serial.read() == '\n')) {
      Serial.println("\n-------------------------------------------------------------------------------------\n");
      for (int u = 0; u < clients_known_count; u++) print_client(clients_known[u]);
      for (int u = 0; u < aps_known_count; u++) print_beacon(aps_known[u]);
      Serial.println("\n-------------------------------------------------------------------------------------\n");
    }
  }
}

