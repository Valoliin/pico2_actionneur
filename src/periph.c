#include "periph.h"      // Pour les defines POMPE_A_IN1 etc.
#include "pico/stdlib.h" // Pour gpio_init, GPIO_OUT, etc.
#include <stdbool.h>     // Pour true/false et le type bool

void periph_init()
{
    gpio_init(POMPES);
    gpio_set_dir(POMPES, GPIO_OUT);
    gpio_init(VANNES);
    gpio_set_dir(VANNES, GPIO_OUT);
    pompes_set (false);   // Éteint par défaut
    vannes_set (false);   // Obturé, passif par défaut
}

void pompes_set(bool etat)
{                   // A VERIFIER !
    if (etat)
    {
        gpio_put(POMPES, 1);
    }
    else
    {
        gpio_put(POMPES, 0);
    }
}

void vannes_set(bool etat)
{                   // A VERIFIER !
    if (etat)
    {
        gpio_put(VANNES, 1);
    }
    else
    {
        gpio_put(VANNES, 0);
    }
}