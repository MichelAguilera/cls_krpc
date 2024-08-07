#include "inputs.h"
#include "wifi.h"

char *state_labels[] = {"rcs", "sas"};
uint8_t toggle_states[2] = {LOW, LOW};
unsigned long last_debounce_time[2] = {0, 0}; 
unsigned long last_stage_debounce_time = 0;
const unsigned long debounce_delay = 500;
uint8_t rcs_pin = 2;
uint8_t sas_pin = 4;
uint8_t stage_pin = 7;

void read_input() {
  uint8_t states[2] = {LOW, LOW};
  states[0] = digitalRead(rcs_pin);
  states[1] = digitalRead(sas_pin);

  for (int i = 0; i < 2; i++) {
    if (states[i] == HIGH) {
      unsigned long now = millis();
      if (debounce(now, last_debounce_time[i]) == 0) {
        last_debounce_time[i] = now;
        if (toggle_states[i] == HIGH) {
          toggle_states[i] = LOW;
          char send_str[255];
          sprintf(send_str, "space_center,active_vessel,control,%s,boolean;False\n", state_labels[i]);
          send_packet(send_str);
        } else if (toggle_states[i] == LOW) {
          toggle_states[i] = HIGH;
          char send_str[100];
          sprintf(send_str, "space_center,active_vessel,control,%s,boolean;True\n", state_labels[i]);
          send_packet(send_str);
        }
      }
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
    return 1;
  } else {
    return 0;
  }
}