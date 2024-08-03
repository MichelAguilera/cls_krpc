#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

int status = WL_IDLE_STATUS;
char ssid[] = "M-tech";   // network name
char pass[] = "Jodon59Barbudo!";    // network password

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char ReplyBuffer[] = "acknowledged";       // a string to send back

char *state_labels[] = {"rcs", "sas"};
uint8_t toggle_states[2] = {LOW, LOW};
unsigned long last_debounce_time[2] = {0, 0}; 
unsigned long last_stage_debounce_time = 0;
const unsigned long debounce_delay = 500;
uint8_t rcs_pin = 2;
uint8_t sas_pin = 4;
uint8_t stage_pin = 7;

WiFiUDP Udp;


void monitor_incoming_packets() {
  // IPAddress remoteIP(172,20,10,9);
  // uint16_t remotePort = 12347; 

  // Serial.write("Sending packet to 172.20.10.9:12347...\n");

  // if (Udp.beginPacket(remoteIP, remotePort)) {
  //   Udp.write("Hello world!\n");
  //   if (Udp.endPacket() == 1) {
  //     Serial.println("Packet sent successfully.");
  //   } else {
  //     Serial.println("Error sending packet.");
  //   }
  // } else {
  //   Serial.println("Failed to start packet.");
  // }

  // if there's data available, read a packet

  // int packetSize = Udp.parsePacket();

  // if (packetSize) {

  //   Serial.print("Received packet of size ");

  //   Serial.println(packetSize);

  //   Serial.print("From ");

  //   IPAddress remoteIp = Udp.remoteIP();

  //   Serial.print(remoteIp);

  //   Serial.print(", port ");

  //   Serial.println(Udp.remotePort());

  //   // read the packet into packetBufffer

  //   int len = Udp.read(packetBuffer, 255);

  //   if (len > 0) {

  //     packetBuffer[len] = 0;

  //   }

  //   Serial.println("Contents:");

  //   Serial.println(packetBuffer);

  //   // send a reply, to the IP address and port that sent us the packet we received

  //   Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());

  //   Udp.write(ReplyBuffer);

  //   Udp.endPacket();

  // }
}


void serial_init() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void wifi_check() {
  // check for the presence of the shield:

  if (WiFi.status() == WL_NO_SHIELD) {

    Serial.println("WiFi shield not present");

    // don't continue:

    while (true);

  }

  String fv = WiFi.firmwareVersion();

  if (fv != "1.1.0") {

    Serial.println("Please upgrade the firmware");

  }
}

void wifi_connect() {
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {

    Serial.print("Attempting to connect to SSID: ");

    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

    status = WiFi.begin(ssid, pass);
    Serial.print("STATUS:");
    Serial.println(status);
    // wait 10 seconds for connection:

    delay(5000);
  }

  Serial.println("Connected to wifi");
}

void read_input() {
  // Read current state
  uint8_t states[2] = {LOW, LOW};
  states[0] = digitalRead(rcs_pin);
  states[1] = digitalRead(sas_pin);

  for (int i = 0; i < 2; i++) {
    if (states[i] == HIGH) {
      // digitalWrite(LED_BUILTIN, HIGH);
      unsigned long now = millis();
      if (debounce(now, last_debounce_time[i]) == 0) {
        // Serial.println("Debounced false");
        last_debounce_time[i] = now;
        if (toggle_states[i] == HIGH) {
          toggle_states[i] = LOW;
          char send_str[255];
          sprintf(send_str, "space_center,active_vessel,control,%s,boolean;False\n", state_labels[i]);
          send_packet(send_str);
        }
        else if (toggle_states[i] == LOW) {
          toggle_states[i] = HIGH;
          char send_str[100];
          sprintf(send_str, "space_center,active_vessel,control,%s,boolean;True\n", state_labels[i]);
          send_packet(send_str);
        }
      }
      // digitalWrite(LED_BUILTIN, LOW);
    }
  }

  uint8_t stage_state = digitalRead(stage_pin);
  if (stage_state == HIGH) {
    if (debounce(millis(), last_stage_debounce_time) == 0) {
      last_stage_debounce_time = millis();
      send_packet("space_center,active_vessel,control,activate_next_stage,action;\n");
    }
  }
}

uint8_t debounce(unsigned long now, unsigned long start_time) {
  if (now - start_time < 500) {
    // Serial.print("Debounce not finished yet; ");
    // Serial.println(now - start_time);
    return 1;
  } else {
    return 0;
  }
}

void send_packet(char msg[]) {
  IPAddress remoteIP(10,0,0,34);
  uint16_t remotePort = 12347; 

  Serial.print("Sending packet to 10.0.0.70:12347... ");

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

  // print the SSID of the network you're attached to:

  Serial.print("SSID: ");

  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:

  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");

  Serial.println(ip);

  // print the received signal strength:

  long rssi = WiFi.RSSI();

  Serial.print("signal strength (RSSI):");

  Serial.print(rssi);

  Serial.println(" dBm");
}

void setup() {
  // Hardware setup functions
  serial_init();
  wifi_check();
  wifi_connect();
  printWifiStatus();

  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:
  Udp.begin(localPort);

  // Pin setup
  pinMode(rcs_pin, INPUT);
  pinMode(sas_pin, INPUT);
  pinMode(stage_pin, INPUT);

  // Internal LED debugging
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  read_input();
  monitor_incoming_packets();
}
