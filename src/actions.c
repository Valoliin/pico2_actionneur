#include "actions.h"
#include "lx16.h"
#include "periph.h"
#include "pico/stdlib.h"

// --- Macros privées pour tes gestes ---

static void geste_prendre_noisette()
{
    // 1. Descendre le bras avec le servo
    lx16_move(SERVO_BRAS, 200, 1000);
    sleep_ms(1100);

    // 2. Allumer la pompe (DRV8833 en tout ou rien)
    pompe_set(true);
    sleep_ms(500); // Laisser le temps à la ventouse de "coller"

    // 3. Remonter le bras
    lx16_move(SERVO_PINCE, 0, 500);
    lx16_move(SERVO_BRAS, 800, 1000);
}

static void geste_poser_noisette()
{
    lx16_move(SERVO_BRAS, 200, 1000); // Descendre
    sleep_ms(1100);
    lx16_move(SERVO_PINCE, 800, 500); // Ouvrir
    sleep_ms(600);
    lx16_move(SERVO_BRAS, 800, 1000); // Remonter
}

// --- Le "Cerveau" des actions ---

void action_executer(int action_id)
{
    switch (action_id)
    {
    case CMD_PRENDRE:
        geste_prendre_noisette();
        break;
    case CMD_POSER:
        geste_poser_noisette();
        break;
    case CMD_REPOS:
        lx16_move(0xFE, 500, 1000); // 0xFE est l'ID universel (Broadcast)
        break;
    default:
        break;
    }
}