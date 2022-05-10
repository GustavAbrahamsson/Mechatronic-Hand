#include <Arduino.h>
//#include <WiFi.h>
//#include <esp_now.h>
//#include <ESP32Servo.h>
#include "MotorControl.h"
#include "Wire.h"
#include "angles.h"

#define POT_IN 15
#define BTN 4

int letter = 0;
bool pressed = false;

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
  MotorControl(11, 280, 50, 0, 90), // Thumb IP
  MotorControl(10, 440, 125, 0, 90), // Thumb MCP
  MotorControl(9, 445, 120, 0, 90), // Thumb rotation
  MotorControl(8, 448, 240, 0, 45), // Thumb abduction

  MotorControl(16, 260, 530, 0, 90), //MCP 1
  MotorControl(12, 130, 390, 0, 90), // PIP 1
  MotorControl(18, 540, 615, -15, 15), // ABD 1

  MotorControl(17, 240, 540, 0, 90 ), //MCP 2
  MotorControl(13, 330, 580, 0, 90), // PIP 2
  MotorControl(19, 555, 600, -15, 15), // ABD 2

  MotorControl(20, 710, 400, 0, 90), //MCP 3
  MotorControl(14, 275, 500, 0, 90), // PIP 3
  MotorControl(22, 500, 460, -15, 15), // ABD 3

  MotorControl(21, 710, 450, 0, 90), //MCP 4
  MotorControl(15, 270, 490, 0, 90), // PIP 4
  MotorControl(23, 670, 570, -15, 15), // ABD 4
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

void setAllZero(){

  for(int i = 0; i < 16; i++){
    motors[i].angle_write(0);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();

  pinMode(POT_IN, INPUT);
  pinMode(BTN, INPUT_PULLDOWN);

  setAllZero();

  //init_wifi();
  //getMACAdress();

  // // init all servos
  // for (int i = 0; i < 8; i++)
  // {
  //   servos[i].setPeriodHertz(50); //50 Hz servo
  //   servos[i].attach(servoPins[i], 500, 2500); //Attach correct pin and set limits
  // }
}

// 330 = 0, 580 = 90
void loop() {
  
  Serial.println("next");
  while (!Serial.available()){}

  char l = Serial.read() - 'a';
  
  if (l < 0 || l > 27){
    Serial.println("invalid");
    return;
  }


  // Write letter
  for (int i = 0; i < 16; i++)
  {
    motors[i].angle_write(grips[l][i]);
  }
  
  delay(500);
  


  // while (digitalRead(BTN) == LOW)
  // {
  //   delay(20);
  // }

  // // send all motors to 0
  // for (int i = 0; i < 16; i++)
  // {
  //   motors[i].angle_write(0);

  //   Serial.print(" Motor: ");
  //   Serial.print(motors[i].addr);
  //   Serial.println(" Angle: 0");

  //   delay(1000);
  // }

  // while (digitalRead(BTN) == LOW)
  // {
  //   delay(20);
  // }

  // // Test all motors
  // for (int i = 0; i < 16; i++){
  //   MotorControl motor = motors[i];

  //   motor.angle_write(motor.minAngle);
  //   Serial.print(" Motor: ");
  //   Serial.print(motors[i].addr);
  //   Serial.print(" Angle: ");
  //   Serial.println(motor.minAngle);

  //   delay(1000);

  //   motor.angle_write(motor.maxAngle);
  //   Serial.print(" Motor: ");
  //   Serial.print(motors[i].addr);
  //   Serial.print(" Angle:");
  //   Serial.println(motor.maxAngle);

  //   delay(1000);
    
  //   motor.angle_write(motor.minAngle);
  //   Serial.print(" Motor: ");
  //   Serial.print(motors[i].addr);
  //   Serial.print(" Angle: ");
  //   Serial.println(motor.minAngle);

  //   delay(1000);
  // }

}