# External control of spaceflight simulator (KSP) using KRPC and Arduino

### Introduction

This document provides an overview of the project, designed to interface with Kerbal Space Program (KSP) using external controls via Arduino. The project consists of two main components: the Arduino code, which handles hardware interaction, and the Python code, which communicates with the game.

### Project Structure

```
cls_krpc/
├── kerbal_peripheral/
│   ├── inputs.h
│   ├── inputs.cpp
│   ├── kerbal_peripheral.ino (main)
│   ├── wifi.h
│   ├── wifi.cpp
├── kerbal_middle/
│   ├── main.py
│   ├── sender.py
│   ├── listener.py
│   ├── controller.py
│   ├── config_tool.py
│   ├── config.json
```
The project is split in 2 components: 

kerbal_peripheral - 
* Holds the code responsible for direct interaction with the user, receiving their input.

kerbal_middle - 
* Receives signals and interprets signals from the arduino, and transmits the correct commands to Kerbal Space Program through KRPC.

### Signal pipeline

Originating at the user, inputs received by the arduino in the form of button presses are transmitted to the machine running the kerbal_middle component using UDP packets sent over the network.

Once the UDP packets are received, kerbal_middle interprets those signals and transmits the correct commands to Kerbal Space Program.

Here is a YouTube video demonstrating this: https://youtu.be/5bcgRyC7olQ

### Code Explanation

##### kerbal_peripheral

``` 
kerbal_peripheral.ino
```

```
void setup()

* Runs one time at Arduino startup
* Sets up necessary WiFi connections and pinmode configurations for the buttons to work

Parameters: None
Returns: None


void loop()

* Continuous running loop, runs every tick of the Arduino clock.
* Runs continuous checks for button state changes

Parameters: None
Returns: None


void serial_init()

* Sets up serial communication between Arduino and development machine
* Only used for debugging

Parameters: None
Returns: None
```

``` 
inputs.cpp
```

```
Global variables:


char *state_labels[];

* Text label for each button
* Used to sign packet for proper interpretation by kerbal_middle


uint8_t toggle_states[];

* Holds current state of button press
* If state is LOW, then the button is not being pressed
* If state is HIGH, then the button is being pressed
* Used by functions to detect change in button state


unsigned long last_debounce_time[];

* Holds current time since last debounce signal
* Used by debounce function to determine whether to read change in state or not


unsigned long last_stage_debounce_time;

* Holds current time since last debounce signal of the staging button


const unsigned long debounce_delay;

* Debounce delay constant
* Determines how long the debounce should be


uint8_t rcs_pin;
uint8_t sas_pin;
uint8_t stage_pin;

* Physical pin numbers in Arduino pins
* Used in the setup function to determine which pins the Arduino will read from


void read_input()

* Runs continuously
* Reads button state
* Sends packets to kerbal_middle if valid change in state is detected

Parameters: None
Returns: None


uint8_t debounce(unsigned long now, unsigned long start_time)

* Runs debounce check given the parameter values
* Returns 0 when debounce fails (and allows the program to interpret signal as valid)
* Returns 1 when debounce succeeds (prevents program from interpreting singal as valid)

Parameters: 
* unsigned long now (current time in seconds since power on)
* unsigned long start_time (time since last change of state)

Returns:
* uint8_t (Used instead of a boolean to conserve memory, acts in the same way)
```

``` 
wifi.cpp
```

```
Global variables:

int status;

* WiFi connection status
* Used to track the current state of the WiFi connection


char ssid[];

* Network SSID (WiFi name)
* Stores the SSID for the WiFi network to connect to


char pass[];

* Network password
* Stores the password for the WiFi network


unsigned int localPort;

* Local port number
* Specifies the port number on which the Arduino listens for incoming UDP packets


char packetBuffer[255];

* Buffer for incoming packets
* Used to temporarily store incoming UDP packets


char ReplyBuffer[];

* Reply message
* Contains the message to be sent back as an acknowledgment


WiFiUDP Udp;

* WiFi UDP object
* Used to handle UDP communication over the network


void wifi_check()

* Checks the WiFi shield status and firmware version
* Ensures that the WiFi shield is present and that the firmware version is correct

Parameters: None
Returns: None


void wifi_connect()

* Connects to the specified WiFi network
* Attempts to connect to the network using the provided SSID and password
* Repeats the connection attempt until successful

Parameters: None
Returns: None


void send_packet(char msg[])

* Sends a UDP packet to a specified remote IP address and port
* Formats and sends the message using the WiFiUDP library functions

Parameters:

* char msg[] (The message to be sent)

Returns: None


void printWifiStatus()

* Prints the current WiFi status to the serial monitor
* Displays the SSID, local IP address, and signal strength (RSSI)

Parameters: None
Returns: None
```

##### kerbal_middle

```
main.py
```

```
def main(config)

* Initializes the `Controller` class with the provided configuration
* Starts the controller's event loop

Parameters: 
* config (dict): Configuration settings

Returns: None
```

```
sender.py
```

```
__init__(self, conn, signal_split_char)

* Initializes the `Sender` class
* Sets up the connection and signal split character

Parameters:
* conn (object): KRPC connection object
* signal_split_char (str): Character used to split signals

Returns: None


def send(self, message)

* Processes and sends commands to the KRPC interface

Parameters:
* message (str): Command message to be processed and sent

Returns: None


@staticmethod
def to_type(value, new_type)

* Converts a string value to a specified type

Parameters:
* value (str): The value to be converted
* new_type (str): The type to convert the value to

Returns: Converted value (float, int, bool, or None)
```

```
controller.py
```

```
__init__(self, config, start_disconnected)

* Initializes the `Controller` class
* Sets up the listener and sender
* Connects to the KRPC server if not in disconnected mode, otherwise sets up a dummy connection

Parameters: 
* config (dict): Configuration settings including network addresses and ports
* start_disconnected (bool): Flag to determine if the controller should start without connecting to KRPC. Used for debugging

Returns: None


async def run(self)

* Main loop for the controller
* Continuously listens for incoming signals and sends commands to KSP

Parameters: None
Returns: None
```

```
listener.py
```

```
__init__(self, host, port)

* Initializes the `Listener` class
* Sets up the UDP socket and binds it to the specified host and port

Parameters: 
* host (str): IP address to bind the socket to
* port (int): Port number to bind the socket to

Returns: None


def listen_step(self)

* Listens for incoming UDP packets and returns the decoded message

Parameters: None
Returns: str (The decoded message from the UDP packet)
```

```
config_tool.py
```

```
def get_config()

* Reads the configuration from the specified JSON file
* Prompts the user to view or create a new configuration

Parameters: None
Returns: dict (configuration settings)
```

### Conclusion

This project demonstrates an effective use of Arduino and Python to create a custom peripheral for Kerbal Space Program, showcasing skills in embedded systems, network communication, and modular programming. The structured approach ensures code maintainability and scalability for future enhancements.


### Contact

For any questions or support, please contact:
    michelaguilera01@gmail.com
