#include "periph.h"      // Pour les defines POMPE_A_IN1 etc.
#include "pico/stdlib.h" // Pour gpio_init, GPIO_OUT, etc.
#include <stdbool.h>     // Pour true/false et le type bool

void periph_init()
{
    gpio_init(POMPE_A_IN1);
    gpio_set_dir(POMPE_A_IN1, GPIO_OUT);
    gpio_init(POMPE_A_IN2);
    gpio_set_dir(POMPE_A_IN2, GPIO_OUT);

    pompe_set(false); // Éteint par défaut
}

void pompe_set(bool etat)
{
    if (etat)
    {
        gpio_put(POMPE_A_IN1, 1);
        gpio_put(POMPE_A_IN2, 0);
    }
    else
    {
        gpio_put(POMPE_A_IN1, 0);
        gpio_put(POMPE_A_IN2, 0);
    }
}