/**
 * @file configura_geral.h
 * @brief Centraliza todas as configurações e credenciais do projeto LumiConnect.
 *
 * @warning ESTE ARQUIVO CONTÉM INFORMAÇÕES SENSÍVEIS.
 * Não compartilhe este arquivo com credenciais reais preenchidas.
 * Substitua as senhas por placeholders antes de versionar em sistemas como o Git.
 */

#ifndef CONFIGURA_GERAL_H
#define CONFIGURA_GERAL_H

// --- Configurações da Rede Wi-Fi ---
// ATENÇÃO: Substitua pelos dados da sua rede Wi-Fi.
#define WIFI_SSID "XXX"
#define WIFI_PASSWORD "XXX"

// --- Configurações do Broker MQTT ---
// Endereço IP e porta do servidor (broker) MQTT na sua rede local.
#define MQTT_BROKER_IP   "192.168.0.18" 
#define MQTT_BROKER_PORT 1883

// --- Identificação do Dispositivo ---
// ID único para este dispositivo. Usado para compor os tópicos MQTT.
#define DEVICE_ID "LumiConnect"

// --- Definições de Tópicos MQTT ---
// Tópico base onde os dados de luminosidade serão publicados.
#define TOPICO_PUBLICACAO_LUZ      "dados/luminosidade"

#endif // CONFIGURA_GERAL_H