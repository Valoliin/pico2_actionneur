#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// Tes fichiers drivers et actions
#include "lx16.h"
#include "actions.h"
#include "periph.h"

// Micro-ROS
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>
#include <rmw_microros/rmw_microros.h>
#include "pico_uart_transports.h"

const uint LED_PIN = 25;

// Objets micro-ROS
rcl_publisher_t publisher;
rcl_subscription_t subscriber;
std_msgs__msg__Int32 msg_pub;
std_msgs__msg__Int32 msg_sub;

// Callback pour les commandes reçues (ex: depuis une tablette ou un PC)
void subscription_callback(const void *msin)
{
    const std_msgs__msg__Int32 *msg = (const std_msgs__msg__Int32 *)msin;

    // On appelle ton switch case dans actions.c
    // Note : pendant l'action, l'executor est bloqué.
    action_executer(msg->data);
}

void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
    (void)last_call_time;
    if (timer != NULL)
    {
        rcl_publish(&publisher, &msg_pub, NULL);
        msg_pub.data++;
    }
}

int main()
{
    // 0. Configuration de l'horloge pour Pico 2 (RP2350)
    set_sys_clock_khz(150000, true);
    stdio_init_all();

    // 1. Initialisation de ton matériel (LX16 et Pompes)
    lx16_init();
    periph_init(); // Initialise tes pins DRV8833 pour les pompes

    // On active les moteurs (LOAD)
    lx16_load(SERVO_BRAS);
    lx16_load(SERVO_PINCE);

    // 2. Configuration micro-ROS (UART1 pour l'agent)
    uart_init(uart1, 115200);
    gpio_set_function(8, GPIO_FUNC_UART);
    gpio_set_function(9, GPIO_FUNC_UART);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    rmw_uros_set_custom_transport(
        true, NULL,
        pico_serial_transport_open,
        pico_serial_transport_close,
        pico_serial_transport_write,
        pico_serial_transport_read);

    // Attente Agent
    while (rmw_uros_ping_agent(100, 1) != RCL_RET_OK)
    {
        gpio_put(LED_PIN, !gpio_get(LED_PIN));
        sleep_ms(250);
    }

    // 3. Init ROS 2
    rcl_allocator_t allocator = rcl_get_default_allocator();
    rclc_support_t support;
    rcl_node_t node;

    rclc_support_init(&support, 0, NULL, &allocator);
    rclc_node_init_default(&node, "pico_junior", "", &support);

    // Publisher (Status)
    rclc_publisher_init_default(
        &publisher, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "pico_status");

    // Subscriber (Commandes d'actions)
    rclc_subscription_init_default(
        &subscriber, &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
        "pico_cmd");

    rcl_timer_t timer;
    rclc_timer_init_default(&timer, &support, RCL_MS_TO_NS(1000), timer_callback);

    // 4. Executor (Attention : 2 handles car 1 timer + 1 subscriber)
    rclc_executor_t executor;
    rclc_executor_init(&executor, &support.context, 2, &allocator);
    rclc_executor_add_timer(&executor, &timer);
    rclc_executor_add_subscription(&executor, &subscriber, &msg_sub, &subscription_callback, ON_NEW_DATA);

    gpio_put(LED_PIN, 1);
    msg_pub.data = 0;

    while (true)
    {
        rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
    }
}