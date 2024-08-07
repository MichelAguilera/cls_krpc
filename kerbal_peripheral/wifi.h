#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>
#include <WiFiUdp.h>

extern unsigned int localPort;
extern char packetBuffer[];
extern char ReplyBuffer[];
extern WiFiUDP Udp;

void wifi_check();
void wifi_connect();
void send_packet(char msg[]);
void printWifiStatus();

#endif