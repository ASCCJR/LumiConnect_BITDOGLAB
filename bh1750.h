/**
 * @file bh1750.h
 * @brief Define a interface pública (API) para o módulo do sensor de luz BH1750.
 *
 * Este arquivo de cabeçalho declara as funções que podem ser chamadas
 * por outras partes do sistema para interagir com o sensor de luminosidade.
 */

#ifndef BH1750_H
#define BH1750_H

/**
 * @brief Inicializa o sensor BH1750, enviando o comando para ligá-lo.
 * @note A inicialização do barramento I2C deve ser feita externamente.
 */
void bh1750_iniciar(void);

/**
 * @brief Solicita uma nova medição ao sensor e retorna o valor em Lux.
 * @return float - O valor da luminosidade em Lux, ou -1.0f em caso de erro de comunicação.
 */
float bh1750_ler_lux(void);

#endif // BH1750_H