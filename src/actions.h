#ifndef ACTIONS_H
#define ACTIONS_H

#include "lx16.h"

// Liste des IDs d'actions (pour ton switch case)
#define CMD_REPOS 0
#define CMD_PRENDRE 1
#define CMD_POSER 2
#define CMD_SALUER 3

// Prototypes
void action_executer(int action_id);

#endif