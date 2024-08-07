#include "wifi.h"
#include "inputs.h"

// Used for debugging, works as a terminal output
void serial_init() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void setup() {
  serial_init();
  wifi_check();
  wifi_connect();
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  Udp.begin(localPort);

  pinMode(rcs_pin, INPUT);
  pinMode(sas_pin, INPUT);
  pinMode(stage_pin, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  read_input();
  // monitor_incoming_packets();
}