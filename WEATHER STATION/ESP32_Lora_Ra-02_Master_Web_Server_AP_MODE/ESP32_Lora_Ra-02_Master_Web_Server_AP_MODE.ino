
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ESP32 Lora Ra-02 Master Web Server AP MODE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  --------------------------------------                                                                                                            //
//  A brief description of how this project works.                                                                                                    //
//                                                                                                                                                    //
//  There are 2 types/modes of messages sent by the Master, namely:                                                                                   //
//  1. "get_Data_Mode".                                                                                                                               //
//    > In this mode the Master sends a message containing the command for the Slaves to send the reading status of the DHT11 sensor, humidity value, //
//      temperature value, state of LED 1 and LED 2.                                                                                                  //
//    > In this mode the Master sends messages to Slave 1 and Slave 2 alternately every 1 second.                                                     //
//      This is done so that there is no collision between incoming messages from Slave 1 and Slave 2.                                                //
//    > The message sent contains:                                                                                                                    //
//      "destination_address | sender_address | message_length | mode | message_content".                                                             //
//      destination_address     = address of Slave 1 or Slave 2.                                                                                      //
//      sender_address          = Master address.                                                                                                     //
//      message_content_length  = total number of "characters" in the message sent (destination_address , sender_address and mode not included).      //
//                                In this mode this section is empty/NULL.                                                                            //
//      mode                    = contains values ​​1 and 2. Value 1 for "get_Data_Mode".                                                               //
//      message_content         = message content in the form of characters (String). In this mode this section is empty/NULL.                        //
//                                                                                                                                                    //
//      For example, send a message to Slave 1:                                                                                                       //
//      "0x05 | 0x04 | NULL | 1 | NULL"                                                                                                               //
//      0x05  = address of Slave 1.                                                                                                                   //
//      0x04  = Master address.                                                                                                                       //
//      1     = "get_Data_Mode". Notifies the intended Slave (for example Slave 1) to send a reply message containing the humidity value,             //
//              temperature value, state of LED 1 and LED 2.                                                                                          //
//    > After the message is received by the intended Slave (Slave 1 or Slave 2). Then the slave will send a reply message containing:                //
//      "destination_address | sender_address | message_content | message_content".                                                                   //
//      destination_address     = Master address.                                                                                                     //
//      sender_address          = address of Slave 1 or Slave 2.                                                                                      //
//      message_content_length  = total number of "characters" in the message sent (destination_address , sender_address and mode not included).      //
//      message_content         = message content in the form of characters (String).                                                                 //
//                                                                                                                                                    //
//      For example, Slave 1 sends a reply message to Master:                                                                                         //
//      "0x04 | 0x05 | 14 | s,80,30,50,0,1"                                                                                                           //
//      0x04            = Master address.                                                                                                             //
//      0x05            = address of Slave 1.                                                                                                         //
//      14              = total number of "characters" in the message sent. The content of the message sent is: "s,80,30.50,0,1" ,                    //
//                        the total number of characters is 14 characters.                                                                            //
//      s,80,30.50,0,1  = data.                                                                                                                       //
//                        s     = DHT11 sensor reading status, "s" if successful and "f" if failed.                                                   //
//                        80    = humidity value.                                                                                                     //
//                        30.50 = temperature value.                                                                                                  //
//                        0     = LED 1 state (0 if off, 1 if on).                                                                                    //
//                        1     = LED 2 state (0 if off, 1 if on).                                                                                    //
//                                                                                                                                                    //
//  2. "led_Control_Mode".                                                                                                                            //
//    > In this mode the Master sends a message containing the command to control LED 1 and LED 2 on the Slaves.                                      //
//    > In this mode the Master sends a message to Slave 1 or Slave 2 each time the Button to control the LED on the web server page (GUI)            //
//      is pressed/clicked.                                                                                                                           //
//    > The message sent contains:                                                                                                                    //
//    "destination_address | sender_address | message_length | mode | message_content".                                                               //
//    destination_address     = address of Slave 1 or Slave 2.                                                                                        //
//    sender_address          = Master address.                                                                                                       //
//    message_content_length  = total number of "characters" in the message sent (destination_address , sender_address and mode not included).        //
//    mode                    = contains values ​​1 and 2. Value 2 for "led_Control_Mode".                                                              //
//    message_content         = message content in the form of characters (String).                                                                   //
//                                                                                                                                                    //
//    For example, send a message to Slave 1:                                                                                                         //
//    "0x05 | 0x04 | 3 | 2 | 1,t"                                                                                                                     //
//    0x05  = address of Slave 1.                                                                                                                     //
//    0x04  = Master address.                                                                                                                         //
//    3     = total number of "characters" in the message sent. The content of the message sent is: "1,1" ,                                           //
//            the total number of characters is 3 characters.                                                                                         //
//    2     = "led_Control_Mode". Notifies the intended Slave (for example Slave 1) that a message was sent to control the LED.                       //
//    1,t   = data.                                                                                                                                   //
//            1 = LED number controlled (1 for "LED 1" and 2 for "LED 2").                                                                            //
//            t = Sets the LED state ("t" to turn on and "f" to turn off).                                                                            //
//  --------------------------------------                                                                                                            //
//                                                                                                                                                    //
//  --------------------------------------                                                                                                            //
//  Additional information :                                                                                                                          //
//                                                                                                                                                    //
//  If you are sure that you have connected the Lora Ra-02 correctly to the ESP32,                                                                    //
//  but you get the error message "LoRa init failed ! Check your connections", then try using an external power source for the Lora Ra-02.            //
//  In this project I used a 3.3V power supply from an ESP32 board to power the Lora Ra-02.                                                           //
//  Because maybe the quality of the ESP32 board module is different in the market and the power supply is not strong enough to power the Lora Ra-02. //
//  So you get the error message "LoRa init failed ! Check your connections".                                                                         //
//  --------------------------------------                                                                                                            //
//                                                                                                                                                    //
//  --------------------------------------                                                                                                            //
//  SORRY IF MY ENGLISH IS CONFUSING, I USE AN ONLINE TRANSLATOR.                                                                                     //
//  --------------------------------------                                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------- Include Library.
#include <SPI.h>
#include <LoRa.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
//---------------------------------------- 

#include "PageIndex.h" 

//---------------------------------------- LoRa Pin / GPIO configuration.
#define ss 5
#define rst 14
#define dio0 2
//----------------------------------------

//---------------------------------------- Access Point Declaration and Configuration.
const char* ssid = "WEATHER_STATION";  //--> access point name
const char* password = "password"; //--> access point password

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
//----------------------------------------

//---------------------------------------- Variable declaration to hold incoming and outgoing data.
String Incoming = "";
String Message = ""; 
//----------------------------------------

//---------------------------------------- LoRa data transmission configuration.
byte LocalAddress = 0x04;               //--> address of this device (Master Address).
byte Destination_ESP32_Slave_1 = 0x05;  //--> destination to send to Slave 1 (ESP32).
byte Destination_ESP32_Slave_2 = 0x06;  //--> destination to send to Slave 2 (ESP32).
const byte get_Data_Mode = 1;           //--> Mode to get the reading status of the DHT11 sensor, humidity value, temperature value, state of LED 1 and LED 2.
const byte led_Control_Mode = 2;        //--> Mode to control LED 1 and LED 2 on the targeted Slave.
//---------------------------------------- 

//---------------------------------------- Variable declaration for Millis/Timer.
unsigned long previousMillis_SendMSG_to_GetData = 0;
const long interval_SendMSG_to_GetData = 1000;

unsigned long previousMillis_RestartLORA = 0;
const long interval_RestartLORA = 1000;
//---------------------------------------- 

//---------------------------------------- Variables to accommodate the reading status of the DHT11 sensor, humidity value, temperature value, state of LED 1 and LED 2.
int Humd[2];
float Temp[2];
String LED_1_State_str = "";
String LED_2_State_str = "";
String receive_Status_Read_DHT11 = "";
bool LED_1_State_bool = false;
bool LED_2_State_bool = false;
//---------------------------------------- 

//---------------------------------------- The variables used to check the parameters passed in the URL.
// Look in the "PageIndex.h" file.
// xhr.open("GET", "set_LED?Slave_Num="+slave+"&LED_Num="+led_num+"&LED_Val="+value, true);
// For example :
// set_LED?Slave_Num=S1&LED_Num=1&LED_Val=1
// PARAM_INPUT_1 = S1
// PARAM_INPUT_2 = 1
// PARAM_INPUT_3 = 1
const char* PARAM_INPUT_1 = "Slave_Num";
const char* PARAM_INPUT_2 = "LED_Num";
const char* PARAM_INPUT_3 = "LED_Val";
//---------------------------------------- 

//---------------------------------------- Variable declaration to hold data from the web server to control the LED.
String Slave_Number = "";
String LED_Number = "";
String LED_Value = "";
//---------------------------------------- 

// Variable declaration to count slaves.
byte Slv = 0;

// Variable declaration to get the address of the slaves.
byte slave_Address;

// Declaration of variable as counter to restart Lora Ra-02.
byte count_to_Rst_LORA = 0;

// Variable declaration to notify that the process of receiving the message has finished.
bool finished_Receiving_Message = false;

// Variable declaration to notify that the process of sending the message has finished.
bool finished_Sending_Message = false;

// Variable declaration to start sending messages to Slaves to control the LEDs.
bool send_Control_LED = false;

// Initialize JSONVar
JSONVar JSON_All_Data_Received;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// salim added one
unsigned long lastPrint = 0;


//________________________________________________________________________________ Subroutines for sending data (LoRa Ra-02).
void sendMessage(String Outgoing, byte Destination, byte SendMode) { 
  finished_Sending_Message = false;

  Serial.println();
  Serial.println("Tr to  : 0x" + String(Destination, HEX));
  Serial.print("Mode   : ");
  if (SendMode == 1) Serial.println("Getting Data");
  if (SendMode == 2) Serial.println("Controlling LED.");
  Serial.println("Message: " + Outgoing);
  
  LoRa.beginPacket();             //--> start packet
  LoRa.write(Destination);        //--> add destination address
  LoRa.write(LocalAddress);       //--> add sender address
  LoRa.write(Outgoing.length());  //--> add payload length
  LoRa.write(SendMode);           //--> 
  LoRa.print(Outgoing);           //--> add payload
  LoRa.endPacket();               //--> finish packet and send it
  
  finished_Sending_Message = true;
}
//________________________________________________________________________________ 

//________________________________________________________________________________ Subroutines for receiving data (LoRa Ra-02).
void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  finished_Receiving_Message = false;

  //---------------------------------------- read packet header bytes:
  int recipient = LoRa.read();        //--> recipient address
  byte sender = LoRa.read();          //--> sender address
  byte incomingLength = LoRa.read();  //--> incoming msg length
  //---------------------------------------- 

  // Clears Incoming variable data.
  Incoming = "";

  //---------------------------------------- Get all incoming data / message.
  while (LoRa.available()) {
    Incoming += (char)LoRa.read();
  }
  //---------------------------------------- 

  // Resets the value of the count_to_Rst_LORA variable if a message is received.
  count_to_Rst_LORA = 0;

  //---------------------------------------- Check length for error.
  if (incomingLength != Incoming.length()) {
    Serial.println();
    Serial.println("er"); //--> "er" = error: message length does not match length.
    //Serial.println("error: message length does not match length");
    finished_Receiving_Message = true;
    return; //--> skip rest of function
  }
  //---------------------------------------- 

  //---------------------------------------- Checks whether the incoming data or message for this device.
  if (recipient != LocalAddress) {
    Serial.println();
    Serial.println("!");  //--> "!" = This message is not for me.
    //Serial.println("This message is not for me.");
    finished_Receiving_Message = true;
    return; //--> skip rest of function
  }
  //---------------------------------------- 

  //----------------------------------------  if message is for this device, or broadcast, print details:
  Serial.println();
  Serial.println("Rc from: 0x" + String(sender, HEX));
  Serial.println("Message: " + Incoming);
  //---------------------------------------- 

  // Get the address of the senders or slaves.
  slave_Address = sender;

  // Calls the Processing_incoming_data() subroutine.
  Processing_incoming_data();

  finished_Receiving_Message = true;
}
//________________________________________________________________________________ 

//________________________________________________________________________________ Subroutines to process data from incoming messages.
void Processing_incoming_data() {
  
//  Examples of the contents of messages received from slaves are as follows: "s,80,30.50,1,0" , 
//  to separate them based on the comma character, the "GetValue" subroutine is used and the order is as follows:
//  GetValue(Incoming, ',', 0) = s
//  GetValue(Incoming, ',', 1) = 80
//  GetValue(Incoming, ',', 2) = 30.50
//  GetValue(Incoming, ',', 3) = 1
//  GetValue(Incoming, ',', 4) = 0

  //---------------------------------------- Conditions for processing data or messages from Slave 1 (ESP32 Slave 1).
  if (slave_Address == Destination_ESP32_Slave_1) {
    receive_Status_Read_DHT11 = GetValue(Incoming, ',', 0);
    if (receive_Status_Read_DHT11 == "f") receive_Status_Read_DHT11 = "FAILED";
    if (receive_Status_Read_DHT11 == "s") receive_Status_Read_DHT11 = "SUCCEED";
    Humd[0] = GetValue(Incoming, ',', 1).toInt();
    Temp[0] = GetValue(Incoming, ',', 2).toFloat();
    LED_1_State_str = GetValue(Incoming, ',', 3);
    LED_2_State_str = GetValue(Incoming, ',', 4);
    if (LED_1_State_str == "1" || LED_1_State_str == "0") {
      LED_1_State_bool = LED_1_State_str.toInt();
    }
    if (LED_2_State_str == "1" || LED_2_State_str == "0") {
      LED_2_State_bool = LED_2_State_str.toInt();
    }

    // Calls the Send_Data_to_WS() subroutine.
    Send_Data_to_WS("S1", 1);
  }
  //---------------------------------------- 

  //---------------------------------------- Conditions for processing data or messages from Slave 2 (ESP32 Slave 2).
  if (slave_Address == Destination_ESP32_Slave_2) {
    receive_Status_Read_DHT11 = GetValue(Incoming, ',', 0);
    if (receive_Status_Read_DHT11 == "f") receive_Status_Read_DHT11 = "FAILED";
    if (receive_Status_Read_DHT11 == "s") receive_Status_Read_DHT11 = "SUCCEED";
    Humd[1] = GetValue(Incoming, ',', 1).toInt();
    Temp[1] = GetValue(Incoming, ',', 2).toFloat();
    LED_1_State_str = GetValue(Incoming, ',', 3);
    LED_2_State_str = GetValue(Incoming, ',', 4);
    if (LED_1_State_str == "1" || LED_1_State_str == "0") {
      LED_1_State_bool = LED_1_State_str.toInt();
    }
    if (LED_2_State_str == "1" || LED_2_State_str == "0") {
      LED_2_State_bool = LED_2_State_str.toInt();
    }

    // Calls the Send_Data_to_WS() subroutine.
    Send_Data_to_WS("S2", 2);
  }
  //---------------------------------------- 
}
//________________________________________________________________________________ 

//________________________________________________________________________________ Subroutine to send data received from Slaves to the web server to be displayed.
void Send_Data_to_WS(char ID_Slave[5], byte Slave) {
  //:::::::::::::::::: Enter the received data into JSONVar(JSON_All_Data_Received).
  JSON_All_Data_Received["ID_Slave"] = ID_Slave;
  JSON_All_Data_Received["StatusReadDHT11"] = receive_Status_Read_DHT11;
  JSON_All_Data_Received["Humd"] = Humd[Slave-1];
  JSON_All_Data_Received["Temp"] = Temp[Slave-1];
  JSON_All_Data_Received["LED1"] = LED_1_State_bool;
  JSON_All_Data_Received["LED2"] = LED_2_State_bool; 
  //:::::::::::::::::: 
  
  //:::::::::::::::::: Create a JSON String to hold all data received from the sender.
  String jsonString_Send_All_Data_received = JSON.stringify(JSON_All_Data_Received);
  //:::::::::::::::::: 
  
  //:::::::::::::::::: Sends all data received from the sender to the browser as an event ('allDataJSON').
  events.send(jsonString_Send_All_Data_received.c_str(), "allDataJSON", millis());
  //::::::::::::::::::  
}
//________________________________________________________________________________ 

//________________________________________________________________________________ String function to process the data received
// I got this from : https://www.electroniclinic.com/reyax-lora-based-multiple-sensors-monitoring-using-arduino/
String GetValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
//________________________________________________________________________________ 

//________________________________________________________________________________ Subroutine to reset Lora Ra-02.
void Rst_LORA() {
  LoRa.setPins(ss, rst, dio0);

  Serial.println();
  Serial.println("Restart LoRa...");
  Serial.println("Start LoRa init...");
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 or 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");

  // Reset the value of the count_to_Rst_LORA variable.
  count_to_Rst_LORA = 0;
}
//________________________________________________________________________________ 

//________________________________________________________________________________ VOID SETUP
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  //---------------------------------------- Clears the values of the Temp and Humd array variables for the first time.
  for (byte i = 0; i < 2; i++) {
    Humd[i] = 0;
    Temp[i] = 0.00;
  }
  //---------------------------------------- 

 //---------------------------------------- Set Wifi to AP mode
  Serial.println();
  Serial.println("-------------");
  Serial.println("WIFI mode : AP");
  WiFi.mode(WIFI_AP);
  Serial.println("-------------");
  //---------------------------------------- 

  delay(100);
//---------------------------------------- Setting up ESP32 to be an Access Point.
  Serial.println();
  Serial.println("-------------");
  Serial.println("Setting up ESP32 to be an Access Point.");
  WiFi.softAP(ssid, password); //--> Creating Access Points
  delay(1000);
  Serial.println("Setting up ESP32 softAPConfig.");
  WiFi.softAPConfig(local_ip, gateway, subnet);
  Serial.println("-------------");
  //----------------------------------------

  delay(500);

  //---------------------------------------- Handle Web Server
  Serial.println();
  Serial.println("Setting Up the Main Page on the Server.");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", MAIN_page);
  });
  //---------------------------------------- 

  //---------------------------------------- Handle Web Server Events
  Serial.println();
  Serial.println("Setting up event sources on the Server.");
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 10 second
    client->send("hello!", NULL, millis(), 10000);
  });
  //---------------------------------------- 

  //---------------------------------------- Send a GET request to <ESP_IP>/set_LED?Slave_Num=<inputMessage1>&LED_Num=<inputMessage2>&LED_Val=<inputMessage3>
  server.on("/set_LED", HTTP_GET, [] (AsyncWebServerRequest *request) {
    //:::::::::::::::::: 
    // GET input value on <ESP_IP>/set_LED?Slave_Num=<inputMessage1>&LED_Num=<inputMessage2>&LED_Val=<inputMessage3>
    // PARAM_INPUT_1 = inputMessage1
    // PARAM_INPUT_2 = inputMessage2
    // PARAM_INPUT_3 = inputMessage3
    // Slave_Number = PARAM_INPUT_1
    // LED_Number = PARAM_INPUT_2
    // LED_Value = PARAM_INPUT_3
    //:::::::::::::::::: 
    
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2) && request->hasParam(PARAM_INPUT_3)) {
      Slave_Number = request->getParam(PARAM_INPUT_1)->value();
      LED_Number = request->getParam(PARAM_INPUT_2)->value();
      LED_Value = request->getParam(PARAM_INPUT_3)->value();

      String Rslt = "Slave : " + Slave_Number + " || LED : " + LED_Number + " || Set to : " + LED_Value;
      Serial.println();
      Serial.println(Rslt);
      send_Control_LED = true;
    }
    else {
      send_Control_LED = false;
      Slave_Number = "No message sent";
      LED_Number = "No message sent";
      LED_Value = "No message sent";
      String Rslt = "Slave : " + Slave_Number + " || LED : " + LED_Number + " || Set to : " + LED_Value;
      Serial.println();
      Serial.println(Rslt);
    }
    request->send(200, "text/plain", "OK");
  });
  //---------------------------------------- 

  //---------------------------------------- Adding event sources on the Server.
  Serial.println();
  Serial.println("Adding event sources on the Server.");
  server.addHandler(&events);
  //---------------------------------------- 

  //---------------------------------------- Starting the Server.
  Serial.println();
  Serial.println("Starting the Server.");
  server.begin();
  //---------------------------------------- 

  // Calls the Rst_LORA() subroutine.
  Rst_LORA();

   Serial.println();
  Serial.println("------------");
  Serial.print("SSID name : ");
  Serial.println(ssid);
  Serial.print("IP address : ");
  Serial.println(WiFi.softAPIP());
  Serial.println();
  Serial.println("Connect your computer or mobile Wifi to the SSID above.");
  Serial.println("Visit the IP Address above in your browser to open the main page.");
  Serial.println("------------");
  Serial.println();
}
//________________________________________________________________________________ 

//________________________________________________________________________________ VOID LOOP
void loop() {
  // put your main code here, to run repeatedly:

  //---------------------------------------- Millis/Timer to send messages to slaves every 1 second (see interval_SendMSG_to_GetData variable).
  //  Messages are sent every one second is alternately.
  //  > Master sends message to Slave 1, delay 1 second.
  //  > Master sends message to Slave 2, delay 1 second.
  

//salim added---
  // Print ESP32 IP every 10 seconds
  if (millis() - lastPrint > 10000) { // every 10 seconds
    Serial.print("Current ESP32 IP: ");
    Serial.println(WiFi.localIP());
    lastPrint = millis();
  }

  
//salim added---

  unsigned long currentMillis_SendMSG_to_GetData = millis();
  
  if (currentMillis_SendMSG_to_GetData - previousMillis_SendMSG_to_GetData >= interval_SendMSG_to_GetData) {
    previousMillis_SendMSG_to_GetData = currentMillis_SendMSG_to_GetData;

    Slv++;
    if (Slv > 2) Slv = 1;
    
    //:::::::::::::::::: Condition for sending message / command data to Slave 1 (ESP32 Slave 1).
    if (Slv == 1) {
      Humd[0] = 0;
      Temp[0] = 0.00;
      sendMessage("", Destination_ESP32_Slave_1, get_Data_Mode);
    }
    //:::::::::::::::::: 
    
    //:::::::::::::::::: Condition for sending message / command data to Slave 2 (ESP32 Slave 2).
    if (Slv == 2) {
      Humd[1] = 0;
      Temp[1] = 0.00;
      sendMessage("", Destination_ESP32_Slave_2, get_Data_Mode);
    }
    //:::::::::::::::::: 
  }
  //---------------------------------------- 

  //---------------------------------------- 
  if (finished_Sending_Message == true && finished_Receiving_Message == true) {
    if (send_Control_LED == true) {
      delay(250);
      send_Control_LED = false;
      if (Slave_Number == "S1") {
        Message = "";
        Message = LED_Number + "," + LED_Value;
        sendMessage(Message, Destination_ESP32_Slave_1, led_Control_Mode);
      }
      if (Slave_Number == "S2") {
        Message = "";
        Message = LED_Number + "," + LED_Value;
        sendMessage(Message, Destination_ESP32_Slave_2, led_Control_Mode);
      }
      delay(250);
    }
  }
  //---------------------------------------- 

  //---------------------------------------- Millis/Timer to reset Lora Ra-02.
  //  - Lora Ra-02 reset is required for long term use.
  //  - That means the Lora Ra-02 is on and working for a long time.
  //  - From my experience when using Lora Ra-02 for a long time, there are times when Lora Ra-02 seems to "freeze" or an error, 
  //    so it can't send and receive messages. It doesn't happen often, but it does happen sometimes. 
  //    So I added a method to reset Lora Ra-02 to solve that problem. As a result, the method was successful in solving the problem.
  //  - This method of resetting the Lora Ra-02 works by checking whether there are incoming messages, 
  //    if no messages are received for approximately 30 seconds, then the Lora Ra-02 is considered to be experiencing "freezing" or error, so a reset is carried out.

  unsigned long currentMillis_RestartLORA = millis();
  
  if (currentMillis_RestartLORA - previousMillis_RestartLORA >= interval_RestartLORA) {
    previousMillis_RestartLORA = currentMillis_RestartLORA;

    count_to_Rst_LORA++;
    if (count_to_Rst_LORA > 30) {
      LoRa.end();
      Rst_LORA();
    }
  }
  //----------------------------------------

  //----------------------------------------parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
  //----------------------------------------
}
//________________________________________________________________________________ 
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
