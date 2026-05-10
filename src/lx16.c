#include "lx16.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "uart_tx.pio.h"

static PIO _pio = pio0;
static uint _sm = 0;
static uint _sm_rx = 1;

// Programme PIO RX 8N1 manuel (évite de modifier le CMakeLists pour un fichier .pio)
static const uint16_t rx_ins[] = {
    0x2020, // 0: wait 0 pin 0     (Attend le bit de start)
    0xea27, // 1: set x, 7 [10]    (Initialise le compteur à 8 bits et attend le milieu du bit)
    0x4001, // 2: in pins, 1       (Lit le bit)
    0x0642, // 3: jmp x--, 2 [6]   (Boucle pour lire les 8 bits)
    0x8020  // 4: push             (Envoie l'octet lu dans la FIFO)
};
static const struct pio_program rx_prog = {
    .instructions = rx_ins,
    .length = 5,
    .origin = -1};

void lx16_init()
{                                     // initialisation du bus des servomoteurs LX16A
    gpio_init(SENS_PIN);              // on utilise un signal de sens deu dialogue
    gpio_set_dir(SENS_PIN, GPIO_OUT); // c'est une sortie
    gpio_put(SENS_PIN, 0);            // Mode lecture par défaut

    uint offset = pio_add_program(_pio, &uart_tx_program);
    pio_sm_config c = uart_tx_program_get_default_config(offset);
    sm_config_set_out_pins(&c, PIN_TX, 1);
    sm_config_set_sideset_pins(&c, PIN_TX);
    pio_gpio_init(_pio, PIN_TX);
    pio_sm_set_consecutive_pindirs(_pio, _sm, PIN_TX, 1, true);

    float div = (float)clock_get_hz(clk_sys) / (BAUD * 8);
    sm_config_set_clkdiv(&c, div);
    pio_sm_init(_pio, _sm, offset, &c);
    pio_sm_set_enabled(_pio, _sm, true);

    // Configuration du RX (PIO injecté pour la lecture)
    uint offset_rx = pio_add_program(_pio, &rx_prog);
    pio_sm_config c_rx = pio_get_default_sm_config();
    sm_config_set_wrap(&c_rx, offset_rx + 0, offset_rx + 4);
    sm_config_set_in_pins(&c_rx, PIN_RX);
    sm_config_set_in_shift(&c_rx, true, false, 32); // Shift à droite
    sm_config_set_clkdiv(&c_rx, div);

    pio_gpio_init(_pio, PIN_RX);
    pio_sm_set_consecutive_pindirs(_pio, _sm_rx, PIN_RX, 1, false); // Input
    gpio_pull_up(PIN_RX);                                           // Pull-up par sécurité
    pio_sm_init(_pio, _sm_rx, offset_rx, &c_rx);
    pio_sm_set_enabled(_pio, _sm_rx, true);
}

static uint8_t calc_checksum(uint8_t *buf)
{ // calcule le caractère de détection d'erreur de cette trame
    uint16_t temp = 0;
    for (uint8_t i = 2; i < buf[3] + 2; i++)
        temp += buf[i];
    return (uint8_t)(~temp);
}

static void send_packet(uint8_t *buf, uint8_t len)
{                          // envoi d'une trame au servomoteur
    gpio_put(SENS_PIN, 1); // EMET
    sleep_us(5);
    for (int i = 0; i < len; i++)
        pio_sm_put_blocking(_pio, _sm, buf[i]);
    while (!pio_sm_is_tx_fifo_empty(_pio, _sm))
        ;
    sleep_us(500);
    gpio_put(SENS_PIN, 0); // LIT
}

void lx16_move(uint8_t id, int16_t position, uint16_t time)
{ // met l'orintation du servomoteur de cette adresse à la position et dans le temps donnés
    uint8_t buf[10] = {0x55, 0x55, id, 7, 1, position & 0xFF, position >> 8, time & 0xFF, time >> 8, 0};
    buf[9] = calc_checksum(buf);
    send_packet(buf, 10);
    sleep_ms(10); // attendre la fin du mouvement avant de continuer
}

void lx16_load(uint8_t id)
{ // met en service le servomoteur de cette adresse
    uint8_t buf[7] = {0x55, 0x55, id, 4, 31, 1, 0};
    buf[6] = calc_checksum(buf);
    send_packet(buf, 7);
    sleep_ms(50); // attendre la fin du chargement avant de continuer
}

int16_t lx16_read_pos(uint8_t id)
{ // Demande et lit la position actuelle du servomoteur (retourne -1 si erreur)
    // 1. Vider le FIFO de réception des données résiduelles
    while (!pio_sm_is_rx_fifo_empty(_pio, _sm_rx))
    {
        (void)_pio->rxf[_sm_rx];
    }

    // 2. Envoyer la commande de lecture (cmd 21 = SERVO_POS_READ)
    uint8_t buf[6] = {0x55, 0x55, id, 3, 21, 0};
    buf[5] = calc_checksum(buf);
    send_packet(buf, 6);

    // 3. Attendre la réponse : 0x55, 0x55, id, 5, 21, pos_l, pos_h, checksum
    uint8_t rx_buf[8];
    int count = 0;
    absolute_time_t timeout_time = make_timeout_time_ms(20); // 20 ms de timeout

    while (count < 8 && absolute_time_diff_us(get_absolute_time(), timeout_time) > 0)
    {
        if (!pio_sm_is_rx_fifo_empty(_pio, _sm_rx))
        {
            // Le PIO a "shifté" les données dans les bits de poids fort
            io_rw_8 *rxfifo_shift = (io_rw_8 *)&_pio->rxf[_sm_rx] + 3;
            rx_buf[count++] = *rxfifo_shift;
        }
    }

    // 4. Vérifier la validité de la trame (Checksum et Entêtes)
    int16_t position = -1;
    if (count == 8 && rx_buf[0] == 0x55 && rx_buf[1] == 0x55 && rx_buf[2] == id && rx_buf[4] == 21)
    {
        if (calc_checksum(rx_buf) == rx_buf[7])
        {
            // Retourner la position (combinaison des deux octets de position)
            position = (int16_t)(rx_buf[5] | (rx_buf[6] << 8));
        }
    }

    // On publie le résultat sur le bus micro-ROS
    publish_servo_pos(id, position);

    return position; // -1 indique une erreur (TimeOut, checksum incorrect, etc.)
}