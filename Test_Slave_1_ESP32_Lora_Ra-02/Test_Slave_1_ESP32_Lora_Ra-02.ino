
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test Slave 1 ESP32 Lora Ra-02
//---------------------------------------- Include Library.
#include <SPI.h>
#include <LoRa.h>
#include "DHT.h"
//---------------------------------------- 

//---------------------------------------- Defines the DHT11 Pin and the DHT type.
#define DHTPIN      15
#define DHTTYPE     DHT11
//---------------------------------------- 

//---------------------------------------- Defines LED Pins.
#define LED_1_Pin   27
#define LED_2_Pin   25
//---------------------------------------- 

//---------------------------------------- LoRa Pin / GPIO configuration.
#define ss 5
#define rst 14
#define dio0 2
//----------------------------------------

// Initializes the DHT sensor (DHT11).
DHT dht11(DHTPIN, DHTTYPE);

//---------------------------------------- Variable declaration to hold incoming and outgoing data.
String Incoming = "";
String Message = "";             
//----------------------------------------

//---------------------------------------- LoRa data transmission configuration.
////////////////////////////////////////////////////////////////////////////
// PLEASE UNCOMMENT AND SELECT ONE OF THE "LocalAddress" VARIABLES BELOW. //
////////////////////////////////////////////////////////////////////////////

byte LocalAddress = 0x02;       //--> address of this device (Slave 1).
//byte LocalAddress = 0x03;       //--> address of this device (Slave 2).

byte Destination_Master = 0x01; //--> destination to send to Master (ESP32).
//----------------------------------------

//---------------------------------------- Variable declarations for temperature and humidity values.
int h = 0;
float t = 0.0;
byte LED_1_State;
byte LED_2_State;
//---------------------------------------- 

//---------------------------------------- Millis / Timer to update temperature and humidity values from DHT11 sensor.
unsigned long previousMillis_UpdateDHT11 = 0;
const long interval_UpdateDHT11 = 2000;
//---------------------------------------- 

//________________________________________________________________________________ Subroutines for sending data (LoRa Ra-02).
void sendMessage(String Outgoing, byte Destination) {
  LoRa.beginPacket();             //--> start packet
  LoRa.write(Destination);        //--> add destination address
  LoRa.write(LocalAddress);       //--> add sender address
  LoRa.write(Outgoing.length());  //--> add payload length
  LoRa.print(Outgoing);           //--> add payload
  LoRa.endPacket();               //--> finish packet and send it
}
//________________________________________________________________________________ 

//________________________________________________________________________________ Subroutines for receiving data (LoRa Ra-02).
void onReceive(int packetSize) {
  if (packetSize == 0) return;  //--> if there's no packet, return

  //---------------------------------------- read packet header bytes:
  int recipient = LoRa.read();        //--> recipient address
  byte sender = LoRa.read();          //--> sender address
  byte incomingLength = LoRa.read();  //--> incoming msg length
  //---------------------------------------- 

  // Clears Incoming variable data.
  Incoming = "";

  //---------------------------------------- Get all incoming data.
  while (LoRa.available()) {
    Incoming += (char)LoRa.read();
  }
  //---------------------------------------- 

  //---------------------------------------- Check length for error.
  if (incomingLength != Incoming.length()) {
    Serial.println();
    Serial.println("error: message length does not match length");
    return; //--> skip rest of function
  }
  //---------------------------------------- 

  //---------------------------------------- Checks whether the incoming data or message for this device.
  if (recipient != LocalAddress) {
    Serial.println();
    Serial.println("This message is not for me.");
    return; //--> skip rest of function
  }
  //---------------------------------------- 

  //---------------------------------------- if message is for this device, or broadcast, print details:
  Serial.println();
  Serial.println("Received from: 0x" + String(sender, HEX));
  //Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + Incoming);
  //Serial.println("RSSI: " + String(LoRa.packetRssi()));
  //Serial.println("Snr: " + String(LoRa.packetSnr()));
  //---------------------------------------- 

  // Calls the Processing_incoming_data() subroutine.
  Processing_incoming_data();
}
//________________________________________________________________________________ 

//________________________________________________________________________________ Subroutines to process data from incoming messages, then send messages to the Master.
void Processing_incoming_data() {
  //---------------------------------------- Conditions for sending messages to Master.
/////////////////////////////////////////////////////////////////////////////////////////
// PLEASE UNCOMMENT THE LINE OF CODE BELOW IF THIS CODE OR THIS DEVICE IS FOR SLAVE 1. //
/////////////////////////////////////////////////////////////////////////////////////////

 if (Incoming == "SDS1") {
   digitalWrite(LED_1_Pin, !digitalRead(LED_1_Pin));
   digitalWrite(LED_2_Pin, !digitalRead(LED_2_Pin));
   
   LED_1_State = digitalRead(LED_1_Pin);
   LED_2_State = digitalRead(LED_2_Pin);

   Message = "";
   Message = "SL1," + String(h) + "," + String(t) + "," + String(LED_1_State) + "," + String(LED_2_State);

   Serial.println();
   Serial.println("Send message to Master");
   Serial.print("Message: ");
   Serial.println(Message);
   
   sendMessage(Message, Destination_Master);
 }
  //---------------------------------------- 

  //---------------------------------------- Conditions for sending messages to Master.
/////////////////////////////////////////////////////////////////////////////////////////
// PLEASE UNCOMMENT THE LINE OF CODE BELOW IF THIS CODE OR THIS DEVICE IS FOR SLAVE 2. //
/////////////////////////////////////////////////////////////////////////////////////////

//  if (Incoming == "SDS2") {
//    digitalWrite(LED_1_Pin, !digitalRead(LED_1_Pin));
//    digitalWrite(LED_2_Pin, !digitalRead(LED_2_Pin));
//    
//    LED_1_State = digitalRead(LED_1_Pin);
//    LED_2_State = digitalRead(LED_2_Pin);
//
//    Message = "";
//    Message = "SL2," + String(h) + "," + String(t) + "," + String(LED_1_State) + "," + String(LED_2_State);
//
//    Serial.println();
//    Serial.println("Send message to Master");
//    Serial.print("Message: ");
//    Serial.println(Message);
//    
//    sendMessage(Message, Destination_Master);
//  }
  //---------------------------------------- 
}
//________________________________________________________________________________ 

//________________________________________________________________________________ VOID SETUP
void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  pinMode(LED_1_Pin, OUTPUT);
  pinMode(LED_2_Pin, OUTPUT);

  delay(100);

  digitalWrite(LED_1_Pin, HIGH);
  digitalWrite(LED_2_Pin, LOW);

  dht11.begin();

  //---------------------------------------- Settings and start Lora Ra-02.
  LoRa.setPins(ss, rst, dio0);
  
  Serial.println();
  Serial.println("Start LoRa init...");
  if (!LoRa.begin(433E6)) {             // initialize ratio at 915 or 433 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }
  Serial.println("LoRa init succeeded.");
  //---------------------------------------- 
}
//________________________________________________________________________________ 

//________________________________________________________________________________ VOID LOOP
void loop() {
  // put your main code here, to run repeatedly:

  //---------------------------------------- Millis / Timer to update the temperature and humidity values ​​from the DHT11 sensor every 2 seconds (see the variable interval_UpdateDHT11).
  unsigned long currentMillis_UpdateDHT11 = millis();
  
  if (currentMillis_UpdateDHT11 - previousMillis_UpdateDHT11 >= interval_UpdateDHT11) {
    previousMillis_UpdateDHT11 = currentMillis_UpdateDHT11;

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    h = dht11.readHumidity();
    // Read temperature as Celsius (the default)
    t = dht11.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float f = dht.readTemperature(true);
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      h = 0;
      t = 0.0;
    }
  }
  //---------------------------------------- 
  
  //---------------------------------------- parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
  //----------------------------------------
}
//________________________________________________________________________________ 
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

