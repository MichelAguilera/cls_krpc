#include "wifi.h"

// ! Change network ssid and password
int status = WL_IDLE_STATUS;
char ssid[] = "SSID";   // network name
char pass[] = "PASS";    // network password

unsigned int localPort = 2390;      // local port to listen on
char packetBuffer[255];
char ReplyBuffer[] = "acknowledged";
WiFiUDP Udp;

void wifi_check() {
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }
}

void wifi_connect() {
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    Serial.print("STATUS:");
    Serial.println(status);
    delay(5000);
  }

  Serial.println("Connected to wifi");
}

// ! Change IP and port
void send_packet(char msg[]) {
  IPAddress remoteIP(0,0,0,0);
  uint16_t remotePort = 00000; 

  Serial.print("Sending packet to IP:PORT... ");
  if (Udp.beginPacket(remoteIP, remotePort)) {
    Udp.write(msg);
    if (Udp.endPacket() == 1) {
      Serial.println("Packet sent successfully.");
    } else {
      Serial.println("Error sending packet.");
    }
  } else {
    Serial.println("Failed to start packet.");
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}