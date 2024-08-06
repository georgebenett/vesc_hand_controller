#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>


#include "throttle.h"
#include "battery.h"
#include "display.h"

//esp-now broadcast address xiao esp32-c3
//uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0x36, 0x41, 0xD8};

//esp8266mod broadcast address
uint8_t broadcastAddress[] = {0xEC, 0xFA, 0xBC, 0x0E, 0x32, 0xF0};
// Variable to store if sending data was successful
String success;

// Create a struct_message to hold incoming sensor readings

// Define variables to store incoming readings
int incomingRPM;
float incomingVoltage;
float incomingCurrent;

int loopCount = 0;

bool connected = false;

typedef struct struct_tx_message {
    int throttle;
} struct_tx_message;

typedef struct struct_rx_message {
    int vescRpm;
    float vescVoltage;
    float vescCurrent;
} struct_rx_message;

struct_tx_message myData;

struct_rx_message incomingVesc;

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 //Serial.print("\r\nLast Packet Send Status:\t");

  if (status == 0){
    //connected to the receiver
    tft.fillCircle(210, 10, 5, ST77XX_GREEN);
    connected = true;
  }
  else{
    //not connected to the receiver
    tft.fillCircle(210, 10, 5, ST77XX_RED);
    connected = false;
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingVesc, incomingData, sizeof(incomingVesc));
  incomingRPM = incomingVesc.vescRpm;
  incomingVoltage = incomingVesc.vescVoltage;
  incomingCurrent = incomingVesc.vescCurrent;
}

// create a timer with default settings
auto timer = timer_create_default();


bool timerCallBatteryCharge(void *) {
  printBatteryCharge();
  return true; // repeat? true
}


void setup(void) {
  Serial.begin(115200);
  //set lcd properties
  tft.init(240, 280);           // Init ST7789 280x240
  tft.setRotation(2);           // Rotate screen 180 degrees
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);

  //set the timers
  timer.every(SOC_TIMER_INTERVAL, timerCallBatteryCharge);

  // Initialize all readings to 0
  for (uint8_t i = 0; i < NUM_READINGS; i++) {
    throttle_readings[i] = 0;
  }
  // inicialize functions
  printBatteryCharge();


  WiFi.mode(WIFI_MODE_STA);
  WiFi.setTxPower(WIFI_POWER_11dBm); // avg consumption 182mA at 5dBm

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

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}


void loop() {
  loopCount++;
  printAverageThrottle();
  printSpeed(incomingRPM);
  printVescVoltage(incomingVoltage);
  printSkateSoc();
  printTripDistance();

  timer.tick(); /*This one calls the print battery charge function*/

  myData.throttle = average_throttle;

  if(loopCount > 20){
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  loopCount = 50; //reset loopCount
  }

}
