#ifndef ACTIONS_H
#define ACTIONS_H

#include "lx16.h"

// Liste des IDs d'actions (pour ton switch case)
#define CMD_REPOS 50
#define CMD_PRENDRE 51
#define CMD_POSER 52
#define CMD_TOURNER_DD 53
#define CMD_TOURNER_DC 54
#define CMD_TOURNER_GC 55
#define CMD_TOURNER_GG 56
#define CMD_RANGE_CURSEURS 57
#define CMD_BAISSE_CURSEURS 58
#define CMD_SALUER 59

// Prototypes
void action_executer(int action_id);

#endif