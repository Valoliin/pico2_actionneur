# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico2 CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

# On garde TON nom de projet exact
project(PICO2_ACTIONNEUR C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# 1. DÉCLARATION DE L'EXÉCUTABLE (Avec les nouveaux chemins)
add_executable(PICO2_ACTIONNEUR
    src/main.c
    src/lx16.c
    src/actions.c
    src/periph.c
    src/pico_uart_transport.c
)

# 2. GÉNÉRATION PIO (Maintenant que la cible existe)
pico_generate_pio_header(PICO2_ACTIONNEUR ${CMAKE_CURRENT_LIST_DIR}/uart_tx.pio)
pico_generate_pio_header(PICO2_ACTIONNEUR ${CMAKE_CURRENT_LIST_DIR}/uart_rx.pio)

# 3. INCLUDES
target_include_directories(PICO2_ACTIONNEUR PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libmicroros/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# 4. LINKAGE
target_link_libraries(PICO2_ACTIONNEUR
    pico_stdlib
    hardware_uart
    hardware_pio
    hardware_clocks
    ${CMAKE_CURRENT_SOURCE_DIR}/libmicroros/libmicroros.a
)

# 5. DÉFINITIONS ET CONFIG
target_compile_definitions(PICO2_ACTIONNEUR PRIVATE
    PICO_RP2350=1
    PICO_DEFAULT_UART=1
    PICO_DEFAULT_UART_TX_PIN=8
    PICO_DEFAULT_UART_RX_PIN=9
)

pico_enable_stdio_usb(PICO2_ACTIONNEUR 0)
pico_enable_stdio_uart(PICO2_ACTIONNEUR 0)

pico_add_extra_outputs(PICO2_ACTIONNEUR)