#include <Arduino.h>
#include <TFT_eSPI.h>
#include <esp_now.h>
#include <WiFi.h>


#include "throttle.h"
#include "battery.h"
#include "display.h"

extern TFT_eSPI tft;


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

extern int speed;

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
    //tft.fillCircle(150, 150, 5, TFT_GREEN);
  }
  else{
    //not connected to the receiver
    //tft.fillCircle(150, 150, 5, TFT_RED);
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
  getBatteryCharge();
  return true; // repeat? true
}


void setup(void) {
  Serial.begin(115200);
  //set lcd properties

  displayInit();
  tft.drawString("0",120,150, 8);

  //set the timers
  timer.every(SOC_TIMER_INTERVAL, timerCallBatteryCharge);

  // Initialize all readings to 0
  for (uint8_t i = 0; i < NUM_READINGS; i++) {
    throttle_readings[i] = 0;
  }
  // inicialize functions
  getBatteryCharge();


  WiFi.mode(WIFI_MODE_STA);
  WiFi.setTxPower(WIFI_POWER_19_5dBm); // avg consumption 182mA at 5dBm

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
  getAverageThrottle();
  getSpeed(incomingRPM);
  getVescVoltage(incomingVoltage);
  myData.throttle = average_throttle;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));


  displaySpeed();

}

