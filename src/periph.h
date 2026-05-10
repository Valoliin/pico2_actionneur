#ifndef PERIPH_H
#define PERIPH_H

/*********************************************************
 * Périphériques du robot APEROBOT du CROF pour CDR 2026
 * On utilise ici deux modules d type MC1508 ou DRV8833
 * pour répartir les consommations des 4 pompes (0.5A/pompe sous 6V)
 * et des 4 électrovannes de remise à l'air des ventouses
 * (0.27A sous 6V). 
 * Mais on alimente le tout sous 9V 
 * et les électrovannes seulement pendant 0,5s
 **********************************************************/
#include "pico/stdlib.h"
#include <stdbool.h>

// signaux actionnant les pompes 
#define POMPES 17   // un seul signal IN1 qui commande les deux voies du DRV8833

// signaux actionnant les électrovannes
#define VANNES 16   // un seul signal IN1qui commande la voie du DRV8833 vers les 4 électrovannes

void periph_init();
void pompes_set(bool etat);  // si etat = vrai on aspire
void vannes_set(bool etat);  // si etat = vrai on ouvre et met à l'air

#endif