#include <Arduino.h>
//#include <WiFi.h>
//#include <esp_now.h>
//#include <ESP32Servo.h>
#include "MotorControl.h"
#include "Wire.h"

#define POT_IN A0

enum joint{
  IP_TMB,
  MCP_TMB,
  CMC_TMB,
  ABD_TMB,
  MCP_1,
  PIP_1,
  ABD_1,
  MCP_2,
  PIP_2,
  ABD_2,
  MCP_3,
  PIP_3,
  ABD_3,
  MCP_4,
  PIP_4,
  ABD_4,
};

//List of 16 motor objects
MotorControl motors[16] = {
  MotorControl(11, 100, 300, 0, 90), // Thumb IP
  MotorControl(10, 100, 300, 0, 90), // Thumb MCP
  MotorControl(9, 100, 300, 0, 90), // Thumb rotation
  MotorControl(8, 100, 200, 0, 45), // Thumb abduction

  MotorControl(16, , , ,), //MCP 1
  MotorControl(12, 500, 200, 0, 90), // PIP 1
  MotorControl(, , , , ), // ABD 1

  MotorControl(17, , , ), //MCP 2
  MotorControl(13, 500, 200, 0, 90), // PIP 2
  MotorControl(, , , , ), // ABD 2

  MotorControl(20, 750, 500, 0, 90), //MCP 3
  MotorControl(14, 500, 200, 0, 90), // PIP 3
  MotorControl(22, 450, 550, -15, 15), // ABD 3

  MotorControl(21, 750, 500, 0, 90), //MCP 4
  MotorControl(15, 500, 200, 0, 90), // PIP 4
  MotorControl(23, 450, 550, -15, 15), // ABD 4
};

// const int maxServos[] = {180, 143, 180, 140, 180, 125, 180, 138};
// //Recommended 2,4,12-19,21-23,25-27,32-33
// const int servoPins[] = {15, 2, 4, 16, 18, 23, 19, 17};
// Servo servos[8];


//uint8_t broadcastAdress[] = {0x94,0xB9,0x7E,0xE4,0x84,0x34}; //MAC-adress till den svarta
//uint8_t broadcastAdress[] = {0x7C,0x9E,0xBD,0x60,0xD1,0x8C}; //MAC till den med kondensatorn
//uint8_t broadcastAdress[] = {0X7C,0X9E,0XBD,0X61,0X58,0XF4}; //MAC till den med vit tejp
uint8_t broadcastAdress[] = {0x94,0xB9,0x7E,0xE5,0x31,0xD8}; //MAC till Hand lolin

// typedef struct struct_message{
//   int sendID;
//   float thumbIP;
//   float thumbMCP;
//   float finger1PIP;
//   float finger1MCP;
//   float finger2PIP;
//   float finger2MCP;
//   float finger3PIP;
//   float finger3MCP;
//   float finger4PIP;
//   float finger4MCP;
//   float thumbOpp;
//   float test12;
//   float test13;
//   float test14;
//   float test15;
// }struct_message;

// struct_message msg_to_send;
// struct_message recv_data;
// float joint_positions[8];

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

  pinMode(POT_IN, INPUT);

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
  // read A0
  int pot_value = analogRead(POT_IN)+100;

  //send 100 to all motors
  for (int i = 0; i < 16; i++){
    motors[i].pos_raw_write(pot_value);
  }

  // read motor position
  Serial.print(motors[7].pos_raw_read());
  Serial.print("\t");

  // read motor current
  Serial.print("\t");
  Serial.print(motors[7].current_read());
  Serial.print("\t");
  

  // // if (i < 500) {
  // //   motor1.pos_raw_write(100);

  // //   // print motor position
  // //   Serial.print(100);

  // // } else if (i < 1000) {
  // //   motor1.pos_raw_write(900);

  // //   // print motor position
  // //   Serial.print(900);
  // // } else {
  // //   i = 0;
  // // }

  Serial.println();

  delay(10);

}