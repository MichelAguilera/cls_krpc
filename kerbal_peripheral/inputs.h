#ifndef INPUTS_H
#define INPUTS_H

#include <SPI.h>

extern char *state_labels[];
extern uint8_t toggle_states[];
extern unsigned long last_debounce_time[];
extern unsigned long last_stage_debounce_time;
extern const unsigned long debounce_delay;
extern uint8_t rcs_pin;
extern uint8_t sas_pin;
extern uint8_t stage_pin;

void read_input();
uint8_t debounce(unsigned long now, unsigned long start_time);

#endif