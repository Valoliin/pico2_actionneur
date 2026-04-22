#include "lx16.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "uart_tx.pio.h"

static PIO _pio = pio0;
static uint _sm = 0;

void lx16_init()
{
    gpio_init(SENS_PIN);
    gpio_set_dir(SENS_PIN, GPIO_OUT);
    gpio_put(SENS_PIN, 0); // Mode lecture par défaut

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
}

static uint8_t calc_checksum(uint8_t *buf)
{
    uint16_t temp = 0;
    for (uint8_t i = 2; i < buf[3] + 2; i++)
        temp += buf[i];
    return (uint8_t)(~temp);
}

static void send_packet(uint8_t *buf, uint8_t len)
{
    gpio_put(SENS_PIN, 1); // EMET
    sleep_us(5);
    for (int i = 0; i < len; i++)
        pio_sm_put_blocking(_pio, _sm, buf[i]);
    while (!pio_sm_is_tx_fifo_empty(_pio, _sm))
        ;
    sleep_us(100);
    gpio_put(SENS_PIN, 0); // LIT
}

void lx16_move(uint8_t id, int16_t position, uint16_t time)
{
    uint8_t buf[10] = {0x55, 0x55, id, 7, 1, position & 0xFF, position >> 8, time & 0xFF, time >> 8, 0};
    buf[9] = calc_checksum(buf);
    send_packet(buf, 10);
}

void lx16_load(uint8_t id)
{
    uint8_t buf[7] = {0x55, 0x55, id, 4, 31, 1, 0};
    buf[6] = calc_checksum(buf);
    send_packet(buf, 7);
}