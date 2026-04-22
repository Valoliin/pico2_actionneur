#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <time.h>
#include <uxr/client/profile/transport/custom/custom_transport.h>

#define UART_ID uart1
#define BAUDRATE 115200
#define UART_TX_PIN 8
#define UART_RX_PIN 9

bool pico_serial_transport_open(struct uxrCustomTransport *transport)
{
    uart_init(UART_ID, BAUDRATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    return true;
}

bool pico_serial_transport_close(struct uxrCustomTransport *transport)
{
    return true;
}

size_t pico_serial_transport_write(
    struct uxrCustomTransport *transport,
    uint8_t *buf,
    size_t len,
    uint8_t *errcode)
{
    uart_write_blocking(UART_ID, buf, len);
    return len;
}

size_t pico_serial_transport_read(
    struct uxrCustomTransport *transport,
    uint8_t *buf,
    size_t len,
    int timeout,
    uint8_t *errcode)
{
    size_t count = 0;
    absolute_time_t end = make_timeout_time_ms(timeout);

    while (count < len && absolute_time_diff_us(get_absolute_time(), end) > 0)
    {
        if (uart_is_readable(UART_ID))
        {
            buf[count++] = uart_getc(UART_ID);
        }
    }

    return count;
}

void usleep(uint64_t us)
{
    sleep_us(us);
}

int clock_gettime(clockid_t unused, struct timespec *tp)
{
    uint64_t m = time_us_64();
    tp->tv_sec = m / 1000000;
    tp->tv_nsec = (m % 1000000) * 1000;
    return 0;
}