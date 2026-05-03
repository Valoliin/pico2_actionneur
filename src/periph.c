#include "periph.h"      // Pour les defines POMPE_A_IN1 etc.
#include "pico/stdlib.h" // Pour gpio_init, GPIO_OUT, etc.
#include <stdbool.h>     // Pour true/false et le type bool

void periph_init()
{
    gpio_init(POMPES_A_IN1);
    gpio_set_dir(POMPES_A_IN1, GPIO_OUT);
    gpio_init(POMPES_A_IN2);
    gpio_set_dir(POMPES_A_IN2, GPIO_OUT);
    gpio_init(VANNES_A_IN1);
    gpio_set_dir(VANNES_A_IN1, GPIO_OUT);
    gpio_init(VANNES_A_IN2);
    gpio_set_dir(VANNES_A_IN2, GPIO_OUT);
    pompes_set(false);   // Éteint par défaut
    vannes_set (false);  // Obturé par défaut
}

void pompes_set(bool etat)
{                   // A VERIFIER !
    if (etat)
    {
        gpio_put(POMPES_A_IN1, 1);
        gpio_put(POMPES_A_IN2, 0);
    }
    else
    {
        gpio_put(POMPES_A_IN1, 0);
        gpio_put(POMPES_A_IN2, 0);
    }
}

void vannes_set(bool etat)
{                   // A VERIFIER !
    if (etat)
    {
        gpio_put(VANNES_A_IN1, 1);
        gpio_put(VANNES_A_IN2, 0);
    }
    else
    {
        gpio_put(VANNES_A_IN1, 0);
        gpio_put(VANNES_A_IN2, 0);
    }
}