#ifndef LX16_H
#define LX16_H

#include "pico/stdlib.h"
#include "hardware/clocks.h" // INDISPENSABLE pour set_sys_clock_khz
// Configuration des Pins (selon ton PCB corrigé par PIO)
#define PIN_TX 1
#define PIN_RX 0
#define SENS_PIN 2
#define BAUD 115200

// IDs Moteurs
#define SERVO_BRAS 1
#define SERVO_PINCE 2

void lx16_init();
void lx16_load(uint8_t id);
void lx16_move(uint8_t id, int16_t position, uint16_t time);

#endif