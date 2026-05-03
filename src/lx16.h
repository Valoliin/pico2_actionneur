#ifndef LX16_H
#define LX16_H

#include "pico/stdlib.h"
#include "hardware/clocks.h" // INDISPENSABLE pour set_sys_clock_khz
// Configuration des Pins (selon ton PCB corrigé par PIO)
#define PIN_TX 1
#define PIN_RX 0
#define SENS_PIN 2
#define BAUD 115200

// IDs Moteurs    Programmés
#define ID_LEVE_GG 5
#define ID_LEVE_GC 7
#define ID_LEVE_DC 9
#define ID_LEVE_DD 11
#define ID_TOURNE_GG 6
#define ID_TOURNE_GC 10
#define ID_TOURNE_DC 8
#define ID_TOURNE_DD 4
#define ID_CURSEUR_D 2    
#define ID_CURSEUR_G 3

// POSITIONS BASSES DES SERVOS LEVEURS DES BRAS  CALIBRE = !
#define BAS_LEVE_GG 800   // !
#define BAS_LEVE_GC 800   // !
#define BAS_LEVE_DC 800   // !
#define BAS_LEVE_DD 800   // !

// POSITIONS DE REPOS, HAUTE DES SERVOS LEVEURS DE BRAS  CALIBRE = !
#define REPOS_LEVE_GG 160   // !
#define REPOS_LEVE_GC 160   // !
#define REPOS_LEVE_DC 160   // !
#define REPOS_LEVE_DD 160   // !

// POSITION LEVEE PROPICE A LA ROTATION DES SERVOS LEVEURS DE BRAS  CALIBRE = !
#define HAUT_LEVE_GG 400    // !
#define HAUT_LEVE_GC 200    // !
#define HAUT_LEVE_DC 400    // !
#define HAUT_LEVE_DD 180    // !

// POSITIONS DE REPOS DES SERVO TOURNEURS DE NOISETTES   CALIBRE = !
#define REPOS_TOURNE_DD 455   // !   
#define REPOS_TOURNE_DC 440   // !
#define REPOS_TOURNE_GC 490   // !
#define REPOS_TOURNE_GG 500   // !

// POSITIONS TORUNEES DES SERVOS TOURNEURS DE NOISETTES    CALIBRE = !
#define ACTIF_TOURNE_DD 860   // !
#define ACTIF_TOURNE_DC 870   // !
#define ACTIF_TOURNE_GC 880   // !
#define ACTIF_TOURNE_GG 880   // !

// DIFFERENTES POSITIONS DES SERVOS MANIPULANT LES CURSEURS   A CALIBRER  !!
#define REPOS_CURSEUR_D 400     // non paramétré car pas disponible ...
#define ACTIF_CURSEUR_D 600     // non paramétré car pas disponible ...
#define REPOS_CURSEUR_G 900   
#define ACTIF_CURSEUR_G 350

void lx16_init();
void lx16_load(uint8_t id);
void lx16_move(uint8_t id, int16_t position, uint16_t time);

#endif