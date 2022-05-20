#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "MotorControl.h"
#include "Wire.h"
#include "angles.h"
#include "Math.h"

#define POT_IN 15
#define BTN 4

int letter = 0;
bool pressed = false;

uint32_t currentTime = 0;
uint32_t lastTime = 0;

uint32_t phaseShift = 3000;
float waveTime = 500.0;

uint8_t indexAngle  = 0;
uint8_t middleAngle = 0;
uint8_t ringAngle   = 0;
uint8_t littleAngle = 0;

uint8_t sequenceIndex = 0;

uint8_t CMCangle = 0;

int mode = 1;

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
  MotorControl(19, 530, 620, -15, 15), // ABD 2

  MotorControl(20, 710, 400, 0, 90), //MCP 3
  MotorControl(14, 275, 500, 0, 90), // PIP 3
  MotorControl(22, 520, 440, -15, 15), // ABD 3

  MotorControl(21, 710, 450, 0, 90), //MCP 4
  MotorControl(15, 270, 490, 0, 90), // PIP 4
  MotorControl(23, 670, 570, -15, 15), // ABD 4
};

//uint8_t broadcastAdress[] = {0x94,0xB9,0x7E,0xE4,0x84,0x34}; //MAC-adress till den svarta
//uint8_t broadcastAdress[] = {0x7C,0x9E,0xBD,0x60,0xD1,0x8C}; //MAC till den med kondensatorn
uint8_t broadcastAdress[] = {0X7C,0X9E,0XBD,0X61,0X58,0XF4}; //MAC till den med vit tejp
//uint8_t broadcastAdress[] = {0X7C,0X9E,0XBD,0X61,0X58,0XF4}; //MAC till Hand lolin

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
  float finger1Pot;
  float finger2Po;
  float finger3Po;
  float finger4Po;
  float opposition;
  float pot1;
  float pot2;
  float pot3;
  float pot4;
 }struct_message;

struct_message msg_to_send;
struct_message recv_data;
float joint_positions[8];

// // Callback when data is sent, triggered when something is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
     Serial.print("\r\nLast Packet Send Status:\t");
     Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
 }

// // Callback when data is received, triggered when something is recieved
 void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
   memcpy(&recv_data, incomingData, sizeof(recv_data));
   memcpy(&joint_positions, incomingData + 12, sizeof(joint_positions));

   //Serial.print("Bytes received: ");
   //Serial.println(len);

   motors[IP_TMB].angle_write(recv_data.thumbIP);
   motors[MCP_TMB].angle_write(recv_data.thumbMCP);
   motors[CMC_TMB].angle_write(recv_data.opposition);
   motors[PIP_1].angle_write(recv_data.finger1PIP);
   motors[MCP_1].angle_write(recv_data.finger1MCP);
   motors[PIP_2].angle_write(recv_data.finger2PIP);
   motors[MCP_2].angle_write(recv_data.finger2MCP);
   motors[PIP_3].angle_write(recv_data.finger3PIP);
   motors[MCP_3].angle_write(recv_data.finger3MCP);
   motors[PIP_4].angle_write(recv_data.finger4PIP);
   motors[MCP_4].angle_write(recv_data.finger4MCP);
  /*
   motors[ABD_1].angle_write(recv_data.finger1Pot);
   motors[ABD_2].angle_write(recv_data.finger2Po);
   motors[ABD_3].angle_write(recv_data.finger3Po);
   motors[ABD_4].angle_write(recv_data.finger4Po);
   */
   motors[ABD_1].angle_write(0);
   motors[ABD_2].angle_write(0);
   motors[ABD_3].angle_write(0);
   motors[ABD_4].angle_write(0);
 }

 void getMACAdress(){
   WiFi.mode(WIFI_MODE_STA);
   Serial.println(WiFi.macAddress());
 }

 void init_wifi (){
   WiFi.mode(WIFI_STA);
   if (esp_now_init() != ESP_OK) {
     Serial.println("Error initializing ESP-NOW");
     return;
   }
 
   esp_now_register_send_cb(OnDataSent);
 
   esp_now_peer_info_t peerInfo;
   memcpy(peerInfo.peer_addr, broadcastAdress, 6);
   peerInfo.channel = 0;  
   peerInfo.encrypt = false;
 
   if (esp_now_add_peer(&peerInfo) != ESP_OK){
     Serial.println("Failed to add peer");
     return;
   }
   // Register for a callback function that will be called when data is received
   esp_now_register_recv_cb(OnDataRecv);
 }

 void send (){
   msg_to_send.sendID = 1;
   esp_now_send(0, (uint8_t *) &msg_to_send , sizeof(struct_message));
 }

void setAllZero(){

  for(int i = 0; i < 16; i++){
    motors[i].angle_write(0);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();

  init_wifi();
  getMACAdress();

  pinMode(POT_IN, INPUT);
  pinMode(BTN, INPUT_PULLDOWN);

  setAllZero();

}

void loop() {
  send();

/*
  while(1); // Testing with receiving data from the sensor glove

  currentTime = millis();
  char l = '0';  
  if(Serial.available()){
    l = Serial.read();
  }

  if (l == '1'){
    mode = 1;
    Serial.println("Mode 1: letters");
    delay(500);
  }else if (l == '2'){
    mode = 2;
    Serial.println("Mode 2: grasps");
    delay(500);
  }else if (l == '3'){
    mode = 3;
    Serial.println("Mode 3: demo");
    delay(500);
  }else if (mode == 1){
    // mode 1 = letters

    l = l-'a';

    if (l < 0 || l > 27){
      return;
    }

    // Write letter
    for (int i = 0; i < 16; i++)
    {
      motors[i].angle_write(letters[l][i]);
    }
  delay(500);

  }else if(mode == 2){
    // mode 2 = grasps

    l = l-'a';

    if (l < 0 || l > 16){
      return;
    }

    // Write grasp
    for (int i = 0; i < 16; i++)
    {
      motors[i].angle_write(grips[l][i]);
    }
  delay(500);
  }else if(mode == 3){
    // mode 3 = demo
    currentTime = currentTime % 45000;

    if(currentTime < 10000){
      indexAngle = 45 + 45*sin(currentTime/waveTime);
      motors[MCP_1].angle_write(indexAngle);
      motors[PIP_1].angle_write(indexAngle);

      middleAngle = 45 + 45*sin((currentTime + 2 * phaseShift)/waveTime);
      motors[MCP_2].angle_write(middleAngle);
      motors[PIP_2].angle_write(middleAngle);
      
      ringAngle = 45 + 45*sin((currentTime + 3 * phaseShift)/waveTime);
      motors[MCP_3].angle_write(ringAngle);
      motors[PIP_3].angle_write(ringAngle);
      
      littleAngle = 45 + 45*sin((currentTime + 4 * phaseShift)/waveTime);
      motors[MCP_4].angle_write(littleAngle);
      motors[PIP_4].angle_write(littleAngle);

      CMCangle = 10 + 10*sin((currentTime-waveTime)/waveTime);
      motors[CMC_TMB].angle_write(CMCangle);
      motors[IP_TMB].angle_write(2*CMCangle);
      motors[MCP_TMB].angle_write(2*CMCangle);
    
    }else if(currentTime < 11000){
      setAllZero();

    }else if(currentTime < 20000){
      motors[MCP_1].angle_write(0);
      motors[PIP_1].angle_write(0);
      motors[MCP_2].angle_write(0);
      motors[PIP_2].angle_write(0);
      motors[MCP_3].angle_write(0);
      motors[PIP_3].angle_write(0);
      motors[MCP_4].angle_write(0);
      motors[PIP_4].angle_write(0);
      motors[MCP_TMB].angle_write(0);
      motors[IP_TMB].angle_write(0);
      motors[CMC_TMB].angle_write(0);
      motors[ABD_TMB].angle_write(0);

      if(currentTime % 4000 < 1000){
        motors[ABD_1].angle_write(-15);
        motors[ABD_2].angle_write(-15);
        motors[ABD_3].angle_write(-15);
        motors[ABD_4].angle_write(-15);
      }else if(currentTime % 4000 < 2000){
        motors[ABD_1].angle_write(15);
        motors[ABD_2].angle_write(-15);
        motors[ABD_3].angle_write(-15);
        motors[ABD_4].angle_write(15);
      }else if(currentTime % 4000 < 3000){
        motors[ABD_1].angle_write(-15);
        motors[ABD_2].angle_write(-15);
        motors[ABD_3].angle_write(-15);
        motors[ABD_4].angle_write(-15);
      }else{
        motors[ABD_1].angle_write(10);
        motors[ABD_2].angle_write(15);
        motors[ABD_3].angle_write(15);
        motors[ABD_4].angle_write(0);
      }
    }else if(currentTime < 21000){
      setAllZero();
    }else if(currentTime < 35000){
      motors[ABD_1].angle_write(10);
      motors[ABD_4].angle_write(10);
      if(currentTime - lastTime > 750){
        lastTime = currentTime;
        sequenceIndex++;
        sequenceIndex %= 15;
        motors[sequenceIndex].angle_write(0);
        if(!((sequenceIndex + 1) % 3 == 0) || sequenceIndex == 2 ){
          motors[sequenceIndex + 1].angle_write(motors[sequenceIndex + 1].maxAngle);
        }
      }
    }else if(currentTime < 36000){
      setAllZero();
    }else{
      for(int i = 0; i < 16; i++){
        motors[i].angle_write(yeah[i]);
      }
    }
    delay(50);
  }
  */
}