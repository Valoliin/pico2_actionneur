#include "actions.h"
#include "lx16.h"
#include "periph.h"
#include "pico/stdlib.h"

// --- Macros privées pour tes gestes ---

static void geste_range_servo()
{           // tous les servomoteurs mis en position de repos (rangés)
    lx16_move(ID_CURSEUR_D, REPOS_CURSEUR_D, 500);      // bras curseurs levés
    sleep_ms(2);
    lx16_move(ID_CURSEUR_G, REPOS_CURSEUR_G, 500);
    sleep_ms(2);
    lx16_move(ID_LEVE_DD, REPOS_LEVE_DD, 700);      // bras ventouses levés
    sleep_ms(2);
    lx16_move(ID_LEVE_DC, REPOS_LEVE_DC, 700);
    sleep_ms(2);
    lx16_move(ID_LEVE_GC, REPOS_LEVE_GC, 700);
    sleep_ms(2);
    lx16_move(ID_LEVE_GG, REPOS_LEVE_GG, 700);
    sleep_ms(2);
    lx16_move(ID_TOURNE_DD, REPOS_TOURNE_DD, 500);  // ventouses face au plateau
    sleep_ms(2);
    lx16_move(ID_TOURNE_DD, REPOS_TOURNE_DC, 500);
    sleep_ms(2);
    lx16_move(ID_TOURNE_DD, REPOS_TOURNE_GC, 500);
    sleep_ms(2);
    lx16_move(ID_TOURNE_DD, REPOS_TOURNE_GG, 500);
    sleep_ms(2);
}

static void geste_prendre_noisette()
{
    // 1. Descendre le bras avec le servo
    lx16_move(ID_LEVE_DD, BAS_LEVE_DD, 500);
    sleep_ms(2);
    lx16_move(ID_LEVE_DC, BAS_LEVE_DC, 500);
    sleep_ms(2);
    lx16_move(ID_LEVE_GC, BAS_LEVE_GC, 500);
    sleep_ms(2);
    lx16_move(ID_LEVE_GG, BAS_LEVE_GG, 500);
    sleep_ms(200);
    // 2. Allumer les pompes
    pompes_set(true);    // met en aspiration toutes les pompes
    vannes_set(false);   // ferme les vannes de mise à l'air
    sleep_ms(500);      // Laisser le temps à la ventouse de "coller"
    // 3. Remonter les bras en position prévue pour les rotations
    lx16_move(ID_LEVE_DD, HAUT_LEVE_DD, 500);
    sleep_ms(2);
    lx16_move(ID_LEVE_DC, HAUT_LEVE_DC, 500);
    sleep_ms(2);
    lx16_move(ID_LEVE_GC, HAUT_LEVE_GC, 500);
    sleep_ms(2);
    lx16_move(ID_LEVE_GG, HAUT_LEVE_GG, 500);
    sleep_ms(100);
}

static void geste_poser_noisette()
{   // 1. Descendre les 4 bras
    lx16_move(ID_LEVE_GG, BAS_LEVE_GG, 700); 
    sleep_ms(2);
    lx16_move(ID_LEVE_GC, BAS_LEVE_GC, 700); 
    sleep_ms(2);
    lx16_move(ID_LEVE_DC, BAS_LEVE_DC, 700); 
    sleep_ms(2);
    lx16_move(ID_LEVE_DD, BAS_LEVE_DD, 700); 
    sleep_ms(900);
    // 2. Libérer les noisettes des ventouses
    pompes_set(false);    // arrêter l'aspiration des pompes
    vannes_set(true);     // ouvrir les vannes pour la mise à l'air
    // 3. ranger les 4 bras de ventouses maintenant vides
    lx16_move(ID_LEVE_GG, REPOS_LEVE_GG, 700); 
    sleep_ms(2);
    lx16_move(ID_LEVE_GC, REPOS_LEVE_GC, 700); 
    sleep_ms(2);
    lx16_move(ID_LEVE_DC, REPOS_LEVE_DC, 700); 
    sleep_ms(2);
    lx16_move(ID_LEVE_DD, REPOS_LEVE_DD, 700);
    sleep_ms(500);
}

static void geste_tourne_DD()
{       // fait tourner de 90° le porte noisette le plus à droite
    lx16_move(ID_TOURNE_DD, ACTIF_TOURNE_DD, 400);
    sleep_ms(2);
}

static void geste_tourne_DC()
{
    lx16_move(ID_TOURNE_DD, ACTIF_TOURNE_DC, 400);
    sleep_ms(2);
}

static void geste_tourne_GC()
{
    lx16_move(ID_TOURNE_DD, ACTIF_TOURNE_GC, 400);
    sleep_ms(2);
}

static void geste_tourne_GG()
{       // fait tourner de 90° le porte noisette le plus à gauche
    lx16_move(ID_TOURNE_DD, ACTIF_TOURNE_GG, 400);
    sleep_ms(2);
}

static void geste_descend_curseurs()
{           // les deux curseurs sont descendus ensemble
    lx16_move(ID_CURSEUR_D, ACTIF_CURSEUR_D, 300);
    sleep_ms(2);
    lx16_move(ID_CURSEUR_G, ACTIF_CURSEUR_G, 300);
    sleep_ms(2);
}

static void geste_range_curseurs()
{            // les deux curseurs sont remontés ensemble
    lx16_move(ID_CURSEUR_D, REPOS_CURSEUR_D, 300);
    sleep_ms(2);
    lx16_move(ID_CURSEUR_G, REPOS_CURSEUR_G, 300);
    sleep_ms(2);
}

// --- Le "Cerveau" des actions ---

void action_executer(int action_id)
{
    switch (action_id)
    {
        case CMD_REPOS:
            geste_range_servo();
            break;
        case CMD_PRENDRE:
            geste_prendre_noisette();
            break;
        case CMD_POSER:
            geste_poser_noisette();
            break;
        case CMD_BAISSE_CURSEURS:
            geste_descend_curseurs();
            break;
        case CMD_RANGE_CURSEURS:
            geste_range_curseurs();
            break;
        case CMD_TOURNER_DD:
            geste_tourne_DD();
            break;
        case CMD_TOURNER_DC:
            geste_tourne_DC();
            break;
        case CMD_TOURNER_GC:
            geste_tourne_GC();
            break;
        case CMD_TOURNER_GG:
            geste_tourne_GG();
            break;
        default:
            break;
    }
}