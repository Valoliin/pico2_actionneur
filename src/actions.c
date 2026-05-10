#include "actions.h"
#include "lx16.h"
#include "periph.h"
#include "pico/stdlib.h"

// --- Macros privées pour tes gestes --- DELAIS donnés aux gestes
const int pause_geste_debut = 1000; // Durée de pause avant d'exécuter un geste (en ms)
const int pause_geste = 10;         // Durée de pause entre les mouvements (en ms)
//const int time_geste = 1500;         // Durée par défaut pour les mouvements (en ms)
const int time_curseur = 600;
const int time_tourne = 700;
const int time_baisse = 700;
const int time_leve = 800;

static void geste_range_servo()
{
    sleep_ms(pause_geste_debut);                          // tous les servomoteurs mis en position de repos (rangés)
    lx16_move(ID_CURSEUR_D, REPOS_CURSEUR_D, time_curseur); // bras curseurs levés
    // lx16_read_pos(ID_CURSEUR_D);                          // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_CURSEUR_G, REPOS_CURSEUR_G, time_curseur);
    // lx16_read_pos(ID_CURSEUR_G); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DD, REPOS_LEVE_DD, time_leve); // bras ventouses levés
    // lx16_read_pos(ID_LEVE_DD);                        // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DC, REPOS_LEVE_DC, time_leve);
    // lx16_read_pos(ID_LEVE_DC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GC, REPOS_LEVE_GC, time_leve);
    // lx16_read_pos(ID_LEVE_GC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GG, REPOS_LEVE_GG, time_leve);
    // lx16_read_pos(ID_LEVE_GG); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_TOURNE_DD, REPOS_TOURNE_DD, time_tourne); // ventouses face au plateau
    // lx16_read_pos(ID_TOURNE_DD);                          // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_TOURNE_DC, REPOS_TOURNE_DC, time_tourne);
    // lx16_read_pos(ID_TOURNE_DC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_TOURNE_GC, REPOS_TOURNE_GC, time_tourne);
    // lx16_read_pos(ID_TOURNE_GC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_TOURNE_GG, REPOS_TOURNE_GG, time_tourne);
    // lx16_read_pos(ID_TOURNE_GG); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
}

static void geste_prendre_noisette()
{
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste
    // 1. Descendre le bras avec le servo
    lx16_move(ID_LEVE_DD, BAS_LEVE_DD, time_baisse);
    // lx16_read_pos(ID_LEVE_DD); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DC, BAS_LEVE_DC, time_baisse);
    // lx16_read_pos(ID_LEVE_DC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GC, BAS_LEVE_GC, time_baisse);
    // lx16_read_pos(ID_LEVE_GC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GG, BAS_LEVE_GG, time_baisse);
    // lx16_read_pos(ID_LEVE_GG); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(200);
    // 2. Allumer les pompes
    // pompes_set(true);  // met en aspiration toutes les pompes
    // vannes_set(false); // ferme les vannes de mise à l'air
    sleep_ms(5000); // Laisser le temps à la ventouse de "coller"
    // 3. Remonter les bras en position prévue pour les rotations
    lx16_move(ID_LEVE_DD, HAUT_LEVE_DD, time_leve);
    // lx16_read_pos(ID_LEVE_DD); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DC, HAUT_LEVE_DC, time_leve);
    // lx16_read_pos(ID_LEVE_DC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GC, HAUT_LEVE_GC, time_leve);
    // lx16_read_pos(ID_LEVE_GC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GG, HAUT_LEVE_GG, time_leve);
    // lx16_read_pos(ID_LEVE_GG); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(100);
}

static void geste_poser_noisette()
{
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste
    // 1. Descendre les 4 bras
    lx16_move(ID_LEVE_GG, BAS_LEVE_GG, time_baisse);
    // lx16_read_pos(ID_LEVE_GG); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GC, BAS_LEVE_GC, time_baisse);
    // lx16_read_pos(ID_LEVE_GC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DC, BAS_LEVE_DC, time_baisse);
    // lx16_read_pos(ID_LEVE_DC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DD, BAS_LEVE_DD, time_baisse);
    // lx16_read_pos(ID_LEVE_DD); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(900);
    // 2. Libérer les noisettes des ventouses
    //    pompes_set(false); // arrêter l'aspiration des pompes
    //    vannes_set(true);  // ouvrir les vannes pour la mise à l'air
    // 3. ranger les 4 bras de ventouses maintenant vides
    sleep_ms(5000);
    lx16_move(ID_LEVE_GG, REPOS_LEVE_GG, time_leve);
    // lx16_read_pos(ID_LEVE_GG); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_GC, REPOS_LEVE_GC, time_leve);
    // lx16_read_pos(ID_LEVE_GC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DC, REPOS_LEVE_DC, time_leve);
    // lx16_read_pos(ID_LEVE_DC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_LEVE_DD, REPOS_LEVE_DD, time_leve);
    // lx16_read_pos(ID_LEVE_DD); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(500);
}

static void geste_tourne_DD()
{
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste
    // fait tourner de 90° le porte noisette le plus à droite
    lx16_move(ID_TOURNE_DD, ACTIF_TOURNE_DD, time_tourne);
    // lx16_read_pos(ID_TOURNE_DD); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
}

static void geste_tourne_DC()
{
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste

    lx16_move(ID_TOURNE_DC, ACTIF_TOURNE_DC, time_tourne);
    // lx16_read_pos(ID_TOURNE_DC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
}

static void geste_tourne_GC()
{
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste
    lx16_move(ID_TOURNE_GC, ACTIF_TOURNE_GC, time_tourne);
    // lx16_read_pos(ID_TOURNE_GC); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
}

static void geste_tourne_GG()
{                                // fait tourner de 90° le porte noisette le plus à gauche
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste
    lx16_move(ID_TOURNE_GG, ACTIF_TOURNE_GG, time_tourne);
    // lx16_read_pos(ID_TOURNE_GG); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
}

static void geste_descend_curseurs()
{                                // les deux curseurs sont descendus ensemble
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste
    lx16_move(ID_CURSEUR_D, ACTIF_CURSEUR_D, time_curseur);
    // lx16_read_pos(ID_CURSEUR_D); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_CURSEUR_G, ACTIF_CURSEUR_G, time_curseur);
    // lx16_read_pos(ID_CURSEUR_G); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
}

static void geste_range_curseurs()
{                                // les deux curseurs sont remontés ensemble
    sleep_ms(pause_geste_debut); // pause avant d'exécuter le geste
    lx16_move(ID_CURSEUR_D, REPOS_CURSEUR_D, time_curseur);
    // lx16_read_pos(ID_CURSEUR_D); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
    lx16_move(ID_CURSEUR_G, REPOS_CURSEUR_G, time_curseur);
    // lx16_read_pos(ID_CURSEUR_G); // publication de la position du servo sur le bus micro-ROS
    sleep_ms(pause_geste);
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