#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include <WiFiEspServer.h>
#include <WiFiEspUdp.h>

// Serial monitor must be set to 115200 baud rate, and "Both NL & CR" 

#include <Wire.h>
#include "SparkFun_VL53L1X_Arduino_Library.h"
VL53L1X distanceSensor;
#ifndef HAVE_HWSERIAL1
#include <SoftwareSerial.h>

SoftwareSerial Serial1(6, 7); // Create WiFi module object on GPIO pin 6 (RX) and 7 (TX)
#endif


// ***************************************************
// VARIABLES (CHECK IF THESE ARE CORRECT)
// ***************************************************
// Declare and initialise global arrays for WiFi settings
IPAddress receiverIP(192,168,43,224); // IP where data is sent
unsigned int arduinoPort = 8081; // Arduino's port to connect to socket 8081
unsigned int receiverPort = 8080; // Port where data is sent
char ssid[] = "arduino server";
char password[] = "12345678";
//____________________________________________________

char incomingPacket[8];  // buffer for incoming packets
int status = WL_IDLE_STATUS; // Declare and initialise variable for radio status 
WiFiEspUDP Udp; // Initiate instance of UDP

// ***************************************************
void setup(void) {
  pinMode(4, OUTPUT); // LED
  digitalWrite(4, HIGH); // Green light indicate that setup is finnished
  
  // Setup WiFi connection
  Serial.begin(115200);// Initialize serial for debugging
  Serial1.begin(9600);// Initialize serial for ESP (WiFi) module
  WiFi.init(&Serial1);// Initialize ESP module

  // Check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) { // Don't continue
    Serial.println("WiFi shield not present");
    while (true);
  }

  //WiFi.config(sIP);
  
  // Attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi ");
 //   Serial.println(ssid);
    status = WiFi.begin(ssid, password); // Connect to WPA/WPA2 network
  }

  Serial.println("You're connected to the network");
  printWifiStatus(); // prints SSID and IP
  
  Udp.begin(arduinoPort); // Starts WiFiUDP socket from this port

  // Setup distance sensor
  Wire.begin(); // initiate wire library for use of I2C bus

  if (distanceSensor.begin() == false) { Serial.println("Distance sensor offline!"); }
  else { Serial.println("Distance sensor online"); }

  distanceSensor.setDistanceMode(2); // 0=short, 1=medium, 2=long
  Serial.print("Distance mode: ");
  if(distanceSensor.getDistanceMode() == 0) { Serial.println("short"); }
  if(distanceSensor.getDistanceMode() == 1) { Serial.println("medium"); }
  if(distanceSensor.getDistanceMode() == 2) { Serial.println("long"); }

  digitalWrite(4, LOW); // LED turns off when setup is complete
  
}
//____________________________________________________

void loop(void) {

  

  // ***************************************************
  // DATA AQUISITION
  // ***************************************************

  char data[] = "00000"; // variable to store distance
  char finalData[10];
  
  //Poll for completion of measurement. Takes 40-50ms.
  while (distanceSensor.newDataReady() == false)
    delay(5);


  String distance = String(distanceSensor.getDistance()); // Get the result of the measurement from the sensor (mm). Originaly type 'int'
  distance.toCharArray(data, 5); // converting 'distance' to char array, storing result in 'data'
  byte rangeStatus = distanceSensor.getRangeStatus(); // status of the distance reading

  sprintf(finalData,"%id%s",rangeStatus,data); // creating string to store in 'finalData': xxxdyyyy (x=range status, y=distance)

 
   //____________________________________________________


  // ***************************************************
  // SENDING DATA BY WIFI
  // ***************************************************
  int packetSize = Udp.parsePacket(); // Checks for available socket

  Udp.beginPacket(receiverIP, receiverPort);
  Udp.write(finalData); // data to send as UDP (must be char array?)
  Udp.endPacket();

/*    
    // For debuging:
     Serial.print("arduinoPort = ");
    Serial.println(arduinoPort);
    Serial.print("recvPort = ");
    Serial.println(recvPort);
//    Serial.print("receiverIP = ");
//    Serial.println(receiverIP);
    Serial.print("recvIP = ");
    Serial.println(recvIP);
    Serial.print("data = ");
    Serial.println(data); 
    } */
  //____________________________________________________


    
  //Serial.println();
   // delay(500);
}






void printWifiStatus() {
  // Print the SSID of the network
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print the IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
/*
  // Print signal strength
  Serial.print("Signal strength: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dB");
*/
}
