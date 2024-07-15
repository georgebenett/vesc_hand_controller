#include <Arduino.h>
#include "VescUart.h"
#include "ESP8266WiFi.h"
#include "espnow.h"
#include "HardwareSerial.h"

/** Initiate VescUart class */
VescUart UART;




//esp-now broadcast address
uint8_t broadcastAddress[] = {0xEC, 0xDA, 0x3B, 0xBF, 0x5D, 0xD0};

// Define variables to store incoming readings
int incomingThrottle;

//Define variables to be sent
int vescRPM;
float vescVoltage;
float vescCurrent;


// Variable to store if sending data was successful
String success;


typedef struct struct_rx_message {
    int throttle;
} struct_message;

typedef struct struct_tx_message {
    int rpm;
    float voltage;
    float current;
} struct_tx_message;


struct_rx_message myData;

struct_tx_message outcomingVescData;


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  //Serial.print("Last Packet Send Status: ");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (sendStatus ==0){
    //connected to the controller, should accept incoming throttle
    myData.throttle = incomingThrottle;
  }
  else{
    //not connected to the controller, safety stop (86 is the neutral value)
    myData.throttle = 127;
  }
}

// Callback when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  incomingThrottle = myData.throttle;
}


void setup(void) {

    /** Setup Serial port to display data */

  Serial.begin(115200);

  UART.setSerialPort(&Serial);

  WiFi.mode(WIFI_STA);
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_COMBO, 1, NULL, 0);

  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);

}


void loop() {

  UART.getVescValues();
  outcomingVescData.rpm = UART.data.rpm;
  outcomingVescData.voltage = UART.data.inpVoltage;
  outcomingVescData.current = UART.data.avgInputCurrent;

  esp_now_send(broadcastAddress, (uint8_t *) &outcomingVescData, sizeof(outcomingVescData));


  UART.nunchuck.valueY = myData.throttle;

  UART.setNunchuckValues();



}

