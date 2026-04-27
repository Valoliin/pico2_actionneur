#ifndef PERIPH_H
#define PERIPH_H

#include "pico/stdlib.h"
#include <stdbool.h>

// signaux actionnant les pompes 
#define POMPES_A_IN1 10   //
#define POMPES_A_IN2 11

// signaux actionnant les électrovannes
#define VANNES_A_IN1 12   // iront sur les 4 électrovannes
#define VANNES_A_IN2 13

void periph_init();
void pompes_set(bool etat);  // si etat = vrai on aspire
void vannes_set(bool etat);  // si etat = vrai on ouvre et met à l'air

#endif