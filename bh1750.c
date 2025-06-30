/**
 * @file bh1750.c
 * @brief Driver para o sensor de luminosidade I2C BH1750.
 *
 * Este arquivo implementa as funções para inicializar e ler dados
 * do sensor de luz ambiente BH1750, usado no projeto LumiConnect.
 */

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "bh1750.h"

// --- Parâmetros de Hardware e Protocolo ---

// Define a instância e os pinos do I2C usados na placa BitDogLab para este sensor.
#define I2C_PORT i2c1
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

// Endereço I2C padrão do sensor BH1750.
#define BH1750_ADDR 0x23

// Comandos de operação do sensor BH1750.
// Usados como variáveis constantes para permitir o uso do operador '&'.
const uint8_t BH1750_CMD_POWER_ON = 0x01; // Comando para ligar o sensor.
const uint8_t BH1750_CMD_HIRES1 = 0x20;   // Comando para medição em alta resolução (1 lux).


/**
 * @brief Envia os comandos iniciais para ligar e configurar o sensor BH1750.
 * @note A inicialização do barramento I2C deve ser feita separadamente (ex: no main.c),
 * pois o barramento pode ser compartilhado com outros dispositivos.
 */
void bh1750_iniciar(void) {
    // Envia o comando para "acordar" o sensor.
    i2c_write_blocking(I2C_PORT, BH1750_ADDR, &BH1750_CMD_POWER_ON, 1, false);
    sleep_ms(10);
}

/**
 * @brief Realiza uma leitura completa do sensor e converte o valor para Lux.
 * * @return float - O valor da luminosidade em Lux. Retorna -1.0f em caso de falha na leitura I2C.
 */
float bh1750_ler_lux(void) {
    uint8_t raw_data[2];

    // Envia o comando para iniciar uma nova medição.
    i2c_write_blocking(I2C_PORT, BH1750_ADDR, &BH1750_CMD_HIRES1, 1, false);
    
    // Aguarda o tempo necessário para a conversão interna do sensor.
    sleep_ms(180); 
    
    // Lê os 2 bytes do resultado da medição.
    int bytes_read = i2c_read_blocking(I2C_PORT, BH1750_ADDR, raw_data, 2, false);
    if (bytes_read < 2) {
        return -1.0f; // Retorna valor de erro se a leitura falhar.
    }
    
    // Combina os dois bytes em um único valor inteiro de 16 bits.
    uint16_t raw_value = (raw_data[0] << 8) | raw_data[1];
    
    // Converte o valor bruto para Lux usando o fator de conversão do datasheet.
    return (float)raw_value / 1.2f;
}