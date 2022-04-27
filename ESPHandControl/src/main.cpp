#include <Arduino.h>
//#include <WiFi.h>
//#include <esp_now.h>
//#include <ESP32Servo.h>
#include "MotorControl.h"
#include "Wire.h"

int i = 0;

enum joint{
  PIP_1,
  MCP_1,
  PIP_2,
  MCP_2,
  PIP_3,
  MCP_3,
  PIP_4,
  MCP_4
};

MotorControl motor1 = MotorControl(8);

// const int maxServos[] = {180, 143, 180, 140, 180, 125, 180, 138};
// //Recommended 2,4,12-19,21-23,25-27,32-33
// const int servoPins[] = {15, 2, 4, 16, 18, 23, 19, 17};
// Servo servos[8];


//uint8_t broadcastAdress[] = {0x94,0xB9,0x7E,0xE4,0x84,0x34}; //MAC-adress till den svarta
//uint8_t broadcastAdress[] = {0x7C,0x9E,0xBD,0x60,0xD1,0x8C}; //MAC till den med kondensatorn
//uint8_t broadcastAdress[] = {0X7C,0X9E,0XBD,0X61,0X58,0XF4}; //MAC till den med vit tejp
uint8_t broadcastAdress[] = {0x94,0xB9,0x7E,0xE5,0x31,0xD8}; //MAC till Hand lolin

typedef struct struct_message{
  int sendID;
  float thumbIP;
  float thumbMCP;
  float finger1PIP;
  float finger1MCP;
  float finger2PIP;
  float finger2MCP;
  float finger3PIP;
  float finger3MCP;
  float finger4PIP;
  float finger4MCP;
  float thumbOpp;
  float test12;
  float test13;
  float test14;
  float test15;
}struct_message;

struct_message msg_to_send;
struct_message recv_data;
float joint_positions[8];

// // Callback when data is sent, triggered when something is sent
// void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//     Serial.print("\r\nLast Packet Send Status:\t");
//     Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
// }

// // Callback when data is received, triggered when something is recieved
// void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
//   memcpy(&recv_data, incomingData, sizeof(recv_data));
//   memcpy(&joint_positions, incomingData + 12, sizeof(joint_positions));
//   //Serial.print("Bytes received: ");
//   //Serial.println(len);
// }

// void getMACAdress(){
//   WiFi.mode(WIFI_MODE_STA);
//   Serial.println(WiFi.macAddress());
// }

// void init_wifi (){
//   WiFi.mode(WIFI_STA);
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }
 
//   esp_now_register_send_cb(OnDataSent);
 
//   esp_now_peer_info_t peerInfo;
//   memcpy(peerInfo.peer_addr, broadcastAdress, 6);
//   peerInfo.channel = 0;  
//   peerInfo.encrypt = false;
 
//   if (esp_now_add_peer(&peerInfo) != ESP_OK){
//     Serial.println("Failed to add peer");
//     return;
//   }
//   // Register for a callback function that will be called when data is received
//   esp_now_register_recv_cb(OnDataRecv);
// }

// void send (){
//   esp_now_send(0, (uint8_t *) &msg_to_send , sizeof(struct_message));
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  
  motor1.pos_raw_write(700);

  //init_wifi();
  //getMACAdress();

  // // init all servos
  // for (int i = 0; i < 8; i++)
  // {
  //   servos[i].setPeriodHertz(50); //50 Hz servo
  //   servos[i].attach(servoPins[i], 500, 2500); //Attach correct pin and set limits
  // }
}

void loop() {
  // for (int i = 0; i < 8; i++)
  // {
  //   int value = joint_positions[i];
  //   if(value < 0){value = 0; }
  //   if(value > 90){value = 90; }
    
  //   value = map(value, 0, 90, 0, maxServos[i]);
  //   servos[i].write(value);
  // }

  // delay(20);

  // read motor position
  Serial.print(motor1.pos_raw_read());
  
  // read motor current
  Serial.print("\t");
  Serial.print(motor1.PID_output_read());
  Serial.print("\t");
  
  // send motor position
  if (i < 500) {
    motor1.pos_raw_write(100);

    // print motor position
    Serial.print(100);

  } else if (i < 1000) {
    motor1.pos_raw_write(900);

    // print motor position
    Serial.print(900);
  } else {
    i = 0;
  }

  Serial.println();
  i++;

  delay(10);

}