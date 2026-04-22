#ifndef PERIPH_H
#define PERIPH_H

#include "pico/stdlib.h"
#include <stdbool.h>

#define POMPE_A_IN1 10
#define POMPE_A_IN2 11

void periph_init();
void pompe_set(bool etat);

#endif