#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_SSD1306.h> // Hardware-specific library for SSD1306
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

#include "throttle.h"
#include "battery.h"

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   32

#define OLED_RESET      -1

#define VBAT_TIMER_INTERVAL 1000
#define SOC_TIMER_INTERVAL  10000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*TO-DO: define int sizes*/
int throttle_readings[NUM_READINGS];  // Array to store throttle readings
int read_index = 0;                   // Index of the current reading
int total = 0;                        // Running total of the readings
int average_throttle = 0;             // Average throttle value
int raw_throttle_value = 0;           // Raw throttle value
int old_throttle = 0;                 // Old throttle value
int current_throttle_value = 0;       // Current throttle value

int raw_battery_voltage =  0;
int battery_voltage = 0;
int old_battery_voltage = 0;                  // Old battery voltage
int battery_charge = 0;
int old_battery_charge = 0;

//esp-now broadcast address
uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0x36, 0x41, 0xD8};

// Variable to store if sending data was successful
String success;

typedef struct struct_message {
    int throttle;
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 //Serial.print("\r\nLast Packet Send Status:\t");

  if (status == 0){
    //display.fillCircle(120, 0, 2, WHITE); //display connected
  }
  else{
    //display.fillCircle(120, 0, 2, BLACK); //display disconnected
  }
}

// create a timer with default settings
auto timer = timer_create_default();


bool timerCallBatteryCharge(void *) {
  /*printBatteryCharge();*/
  return true; // repeat? true
}



void setup(void) {
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.setRotation(2); //invert the display in 180 degrees

  //set the timers
  timer.every(SOC_TIMER_INTERVAL, timerCallBatteryCharge);


  // Initialize all readings to 0
  for (uint8_t i = 0; i < NUM_READINGS; i++) {
    throttle_readings[i] = 0;
  }
  // inicialize functions

  /*printBatteryCharge();*/

  WiFi.mode(WIFI_MODE_STA);

    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }


}


void loop() {
  printAverageThrottle();


  timer.tick(); /*This one calls the print battery charge function*/
  myData.throttle = average_throttle;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    //Serial.println("Sent with success");
  }
  else {
    //Serial.println("Error sending the data");
  }

}

