#include <Arduino.h>
#include <SPI.h>
#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <VescUart.h>



#define SERVO_PIN D1
#define RX D7
#define TX D6

Servo myServo;

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

esp_now_peer_info_t peerInfo;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("Last Packet Send Status: ");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    //connected to the controller, should accept incoming throttle
    myData.throttle = incomingThrottle;
  }
  else{
    //not connected to the controller, safety stop (86 is the neutral value)
    myData.throttle = 86;
  }
}

// Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  incomingThrottle = myData.throttle;
}


void setup(void) {

    /** Setup Serial port to display data */
  Serial.begin(115200);

  /** Setup UART port (Serial1 on Atmega32u4) */
  Serial1.begin(115200, SERIAL_8N1, RX, TX);

  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);

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
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}


void loop() {

  UART.getVescValues();
  outcomingVescData.rpm = UART.data.rpm;
  outcomingVescData.voltage = UART.data.inpVoltage;
  outcomingVescData.current = UART.data.avgInputCurrent;

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outcomingVescData, sizeof(outcomingVescData));

  Serial.println(myData.throttle);
  myServo.write(myData.throttle);


}

