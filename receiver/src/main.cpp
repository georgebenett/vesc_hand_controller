#include <Arduino.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>


#define SERVO_PIN D1

Servo myServo;

//esp-now broadcast address
uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0xBF, 0x5D, 0xD0};

// Define variables to store incoming readings
int incomingThrottle;

// Variable to store if sending data was successful
String success;


typedef struct struct_message {
    int throttle;
} struct_message;

// Create a struct_message to hold incoming sensor readings
struct_message myData;

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("Last Packet Send Status: ");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    myData.throttle = incomingThrottle;
  }
  else{
    myData.throttle = 0;
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  incomingThrottle = myData.throttle;
}


void setup(void) {
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);

  WiFi.mode(WIFI_MODE_STA);

    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

    // Once ESPNow is Init, we will register for Send CB to
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
  esp_now_register_recv_cb(OnDataRecv);
}


void loop() {
    // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  int new_throttle = map(myData.throttle, -255, 255, 0, 180);
  myServo.write(new_throttle);

  Serial.print(myData.throttle);
  Serial.print("  ");
  Serial.println(new_throttle);
}

