/**
 * @file mqtt_lwip.c
 * @brief Módulo encapsulado para o cliente MQTT usando a pilha lwIP.
 *
 * Este arquivo gerencia a conexão com o broker MQTT e fornece uma interface
 * simplificada para publicar mensagens. Ele é projetado para ser usado
 * de forma modular pelo restante do projeto LumiConnect.
 */

#include "configura_geral.h"
#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include <string.h>
#include <stdio.h>

// --- Variáveis Estáticas do Módulo ---

// Ponteiro para a instância do cliente MQTT da biblioteca lwIP.
static mqtt_client_t *client;
// Flag para controle de fluxo, evitando múltiplas publicações antes da confirmação (QoS 1).
static bool publicacao_em_andamento = false;


// --- Funções de Callback Internas ---

/**
 * @brief Callback executado quando o status da conexão com o broker muda.
 * @param client Ponteiro para o cliente MQTT.
 * @param arg Argumento opcional passado durante a conexão.
 * @param status Status da conexão (ex: MQTT_CONNECT_ACCEPTED).
 */
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("[MQTT] Conectado ao broker! (Modo Apenas Publicação)\n");
    } else {
        printf("[MQTT] Falha na conexão: %d\n", status);
    }
}

/**
 * @brief Callback executado após uma tentativa de publicação (QoS > 0).
 * @param arg Argumento opcional.
 * @param err Resultado da publicação (ERR_OK se bem-sucedida).
 */
static void mqtt_pub_request_cb(void *arg, err_t err) {
    // Libera a trava para permitir a próxima publicação.
    publicacao_em_andamento = false;
}


// --- Funções Públicas ---

/**
 * @brief Publica uma mensagem em um tópico MQTT.
 * A função verifica se o cliente está conectado e se não há outra publicação em andamento.
 * @param topico String do tópico onde a mensagem será publicada.
 * @param mensagem Conteúdo da mensagem a ser publicada.
 */
void publicar_mensagem_mqtt(const char *topico, const char *mensagem) {
    if (!client || !mqtt_client_is_connected(client) || publicacao_em_andamento) {
        return;
    }
    
    err_t err = mqtt_publish(client, topico, mensagem, strlen(mensagem), 1, 0, mqtt_pub_request_cb, NULL);
    
    if (err == ERR_OK) {
        // Ativa a trava para aguardar a confirmação do callback mqtt_pub_request_cb.
        publicacao_em_andamento = true;
    }
}

/**
 * @brief Inicializa o cliente MQTT e tenta se conectar ao broker.
 * As configurações de conexão são lidas do arquivo configura_geral.h.
 */
void iniciar_mqtt_cliente(void) {
    ip_addr_t broker_ip;
    ip4addr_aton(MQTT_BROKER_IP, &broker_ip);

    client = mqtt_client_new();
    
    char client_id[32];
    snprintf(client_id, sizeof(client_id), "%s_client", DEVICE_ID);
    
    struct mqtt_connect_client_info_t ci = { .client_id = client_id };
    mqtt_client_connect(client, &broker_ip, MQTT_BROKER_PORT, mqtt_connection_cb, 0, &ci);
}

/**
 * @brief Verifica de forma segura se o cliente MQTT está conectado.
 * @return true se o cliente foi inicializado e está conectado, false caso contrário.
 */
bool cliente_mqtt_esta_conectado(void) {
    // A verificação de 'client != NULL' é crucial para evitar um crash
    // caso esta função seja chamada antes de iniciar_mqtt_cliente().
    if (client == NULL) {
        return false;
    }
    return mqtt_client_is_connected(client);
}