/**
 * @file mqtt_lwip.h
 * @brief Define a interface pública (API) para o módulo cliente MQTT.
 *
 * Este arquivo de cabeçalho declara as funções que podem ser chamadas
 * por outras partes do sistema para interagir com o cliente MQTT,
 * abstraindo a complexidade da implementação interna.
 */

#ifndef MQTT_LWIP_H
#define MQTT_LWIP_H

#include <stdbool.h> // Incluído para o tipo 'bool'.

/**
 * @brief Inicializa o cliente MQTT e dispara a tentativa de conexão com o broker.
 * Esta função é assíncrona; ela retorna imediatamente enquanto a conexão
 * é estabelecida em segundo plano.
 */
void iniciar_mqtt_cliente(void);

/**
 * @brief Publica uma mensagem em um tópico MQTT específico.
 * @param topico A string do tópico (ex: "dados/sensor1").
 * @param mensagem O conteúdo (payload) da mensagem a ser enviada.
 */
void publicar_mensagem_mqtt(const char *topico, const char *mensagem);

/**
 * @brief Verifica o status atual da conexão com o broker MQTT.
 * @return true se o cliente está conectado, false caso contrário.
 */
bool cliente_mqtt_esta_conectado(void);

#endif // MQTT_LWIP_H