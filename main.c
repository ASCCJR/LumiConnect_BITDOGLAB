/**
 * @file main.c
 * @brief Ponto de entrada principal do projeto LumiConnect.
 *
 * Orquestra a inicialização do hardware, dos módulos de software (sensor e MQTT)
 * e executa o loop principal que lê os dados do sensor e os publica na rede.
 * Inclui lógica de reconexão automática ao broker MQTT.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/i2c.h" // Necessário para a inicialização do barramento I2C.

// Módulos customizados do projeto
#include "configura_geral.h"
#include "mqtt_lwip.h"
#include "bh1750.h"

/**
 * @brief Função principal do programa.
 */
int main() {
    // Inicializa todas as interfaces de I/O padrão (incluindo USB-CDC para o serial).
    stdio_init_all();

    // Espera ativa pela conexão do monitor serial via USB.
    // Garante que nenhuma mensagem de log inicial seja perdida.
    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("Projeto LumiConnect\n");

    // --- 1. Inicialização do Hardware e Conectividade ---
    printf("Inicializando hardware e conexões...\n");

    // Inicializa o chip CYW43439 para a conectividade Wi-Fi.
    if (cyw43_arch_init()) {
        printf("ERRO: Falha ao inicializar Wi-Fi\n");
        return -1;
    }
    cyw43_arch_enable_sta_mode(); // Habilita o modo "Station" (cliente).

    // Tenta conectar à rede Wi-Fi definida em configura_geral.h
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        printf("ERRO: Falha ao conectar ao Wi-Fi\n");
        return -1;
    }
    printf("Conectado ao Wi-Fi: %s\n", WIFI_SSID);

    // Inicializa o barramento I2C1 nos pinos definidos.
    // Esta inicialização é feita aqui para permitir o compartilhamento do barramento.
    i2c_init(i2c1, 100 * 1000);
    gpio_set_function(2, GPIO_FUNC_I2C); // SDA
    gpio_set_function(3, GPIO_FUNC_I2C); // SCL
    gpio_pull_up(2);
    gpio_pull_up(3);
    printf("Barramento I2C inicializado.\n");


    // --- 2. Inicialização dos Módulos de Software ---
    printf("Inicializando módulos...\n");

    bh1750_iniciar();
    printf("Sensor BH1750 pronto.\n");
    
    iniciar_mqtt_cliente();
    printf("Cliente MQTT iniciado. Aguardando conexão inicial...\n");
    
    // --- Laço de Espera pela Conexão MQTT Inicial ---
    // Dá um tempo de carência para a primeira conexão ser estabelecida antes de prosseguir.
    for (int i = 0; i < 20 && !cliente_mqtt_esta_conectado(); i++) {
        sleep_ms(500);
    }

    if (cliente_mqtt_esta_conectado()) {
        printf("Conexão MQTT estabelecida com sucesso!\n\n");
    } else {
        printf("[AVISO] Não foi possível conectar ao broker MQTT inicialmente.\n\n");
    }
    
    // Buffers para montar a string do tópico e do payload.
    char topico_completo[128];
    char payload_lux[20];

    // --- Loop Principal com Lógica de Reconexão ---
    while (1) {
        // Verifica se o cliente MQTT continua conectado.
        if (cliente_mqtt_esta_conectado()) {
            // Se conectado, realiza a leitura e a publicação.
            float lux = bh1750_ler_lux();
            printf("Luminosidade: %.2f Lux\n", lux);

            // Monta as strings de tópico e payload dinamicamente.
            snprintf(topico_completo, sizeof(topico_completo), "%s/%s", DEVICE_ID, TOPICO_PUBLICACAO_LUZ);
            snprintf(payload_lux, sizeof(payload_lux), "%.2f", lux);

            // Publica os dados via MQTT.
            publicar_mensagem_mqtt(topico_completo, payload_lux);
            
        } else {
            // Se desconectado, alerta e tenta restabelecer a conexão.
            printf("[AVISO] Cliente MQTT desconectado. Tentando reconectar...\n");
            iniciar_mqtt_cliente();
        }

        // Define o intervalo entre cada ciclo de leitura/publicação.
        sleep_ms(1000); 
    }

    return 0;
}