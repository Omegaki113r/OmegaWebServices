/**
 * @file MQTT.cpp
 * @author Omegaki113r
 * @date Sunday, 9th February 2025 7:08:11 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: MQTT.cpp
 * File Created: Sunday, 9th February 2025 7:08:11 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Monday, 10th February 2025 3:48:29 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <cstring>

#include "OmegaWebServices/MQTT.hpp"

#include <sdkconfig.h>
#if CONFIG_OMEGA_WEB_SERVICES_DEBUG
#define LOGD(format, ...) OMEGA_LOGD(format, ##__VA_ARGS__)
#define HEX_LOGD(buffer, length) OMEGA_HEX_LOGD(buffer, length)
#else
#define LOGD(format, ...)
#define HEX_LOGD(buffer, length)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_INFO
#define LOGI(format, ...) OMEGA_LOGI(format, ##__VA_ARGS__)
#define HEX_LOGI(buffer, length) OMEGA_HEX_LOGI(buffer, length)
#else
#define LOGI(format, ...)
#define HEX_LOGI(buffer, length)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_WARN
#define LOGW(format, ...) OMEGA_LOGW(format, ##__VA_ARGS__)
#define HEX_LOGW(buffer, length) OMEGA_HEX_LOGW(buffer, length)
#else
#define LOGW(format, ...)
#define HEX_LOGW(buffer, length)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_ERROR
#define LOGE(format, ...) OMEGA_LOGE(format, ##__VA_ARGS__)
#define HEX_LOGE(buffer, length) OMEGA_HEX_LOGE(buffer, length)
#else
#define LOGE(format, ...)
#define HEX_LOGE(buffer, length)
#endif

namespace Omega
{
    namespace WebServices
    {
        namespace MQTT
        {
            class _Client
            {
            public:
                esp_mqtt_client_handle_t m_handle;
                Client *m_client;
            };

            OmegaStatus Client::connect()
            {
                const auto uri = std::get_if<BrokerURI>(&m_connection_info);
                const auto host = std::get_if<BrokerInfo>(&m_connection_info);
                if (nullptr == uri && nullptr == host)
                {
                    LOGE("Invalid parameters");
                    return eFAILED;
                }
                if (nullptr != uri)
                {
                    LOGD("Configuring with URI with transport: %d", static_cast<u8>(uri->m_transport));
                    const esp_mqtt_client_config_t::broker_t::address_t address{.uri = uri->m_uri};
                    const esp_mqtt_client_config_t::broker_t broker{address};
                    if (m_authentication.has_value())
                    {
                        LOGD("Auth: [%s],[%s]", m_authentication.value().username, m_authentication.value().password);
                        const esp_mqtt_client_config_t::credentials_t::authentication_t authentication{m_authentication.value().password};
                        const esp_mqtt_client_config_t::credentials_t credential{.username = m_authentication.value().username, .authentication = authentication};
                        const esp_mqtt_client_config_t mqtt_config = {.broker = broker, .credentials = credential};
                        m_handle = esp_mqtt_client_init(&mqtt_config);
                        if (nullptr == m_handle)
                        {
                            LOGE("esp_mqtt_client_init failed");
                            return eFAILED;
                        }
                    }
                    else
                    {
                        const esp_mqtt_client_config_t mqtt_config = {broker};
                        m_handle = esp_mqtt_client_init(&mqtt_config);
                        if (nullptr == m_handle)
                        {
                            LOGE("esp_mqtt_client_init failed");
                            return eFAILED;
                        }
                    }
                }
                if (nullptr != host)
                {
                    LOGD("Configuring with Host %s:%d", host->m_host, host->m_port);
                    const esp_mqtt_client_config_t::broker_t::address_t address{.hostname = host->m_host, .transport = static_cast<esp_mqtt_transport_t>(host->m_transport), .port = host->m_port};
                    const esp_mqtt_client_config_t::broker_t broker{address};
                    if (m_authentication.has_value())
                    {
                        LOGD("Auth: [%s],[%s]", m_authentication.value().username, m_authentication.value().password);
                        const esp_mqtt_client_config_t::credentials_t::authentication_t authentication{m_authentication.value().password};
                        const esp_mqtt_client_config_t::credentials_t credential{.username = m_authentication.value().username, .authentication = authentication};
                        const esp_mqtt_client_config_t mqtt_config = {.broker = broker, .credentials = credential};
                        m_handle = esp_mqtt_client_init(&mqtt_config);
                        if (nullptr == m_handle)
                        {
                            LOGE("esp_mqtt_client_init failed");
                            return eFAILED;
                        }
                    }
                    else
                    {
                        const esp_mqtt_client_config_t mqtt_config = {broker};
                        m_handle = esp_mqtt_client_init(&mqtt_config);
                        if (nullptr == m_handle)
                        {
                            LOGE("esp_mqtt_client_init failed");
                            return eFAILED;
                        }
                    }
                }

                const auto event_handler = [](void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
                {
                    Client *client = (Client *)event_handler_arg;
                    switch (event_id)
                    {
                    case MQTT_EVENT_ERROR:
                    {
                        LOGD("MQTT_EVENT_ERROR");
                        break;
                    }
                    case MQTT_EVENT_CONNECTED:
                    {
                        LOGD("MQTT_EVENT_CONNECTED");
                        client->m_state = State::eCONNECTED;
                        if (client->m_on_connected)
                            client->m_on_connected();
                        break;
                    }
                    case MQTT_EVENT_DISCONNECTED:
                    {
                        LOGD("MQTT_EVENT_DISCONNECTED");
                        client->m_state = State::eDISCONNECTED;
                        if (client->m_on_disconnected)
                            client->m_on_disconnected();
                        break;
                    }
                    case MQTT_EVENT_PUBLISHED:
                    {
                        LOGD("MQTT_EVENT_PUBLISHED");
                        break;
                    }
                    case MQTT_EVENT_DATA:
                    {
                        LOGD("MQTT_EVENT_DATA");
                        esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
                        const auto data = reinterpret_cast<u8 *>(event->data);
                        const auto data_length = event->data_len;
                        if (client->m_on_data)
                            client->m_on_data(data, data_length);
                        break;
                    }
                    case MQTT_EVENT_BEFORE_CONNECT:
                    {
                        LOGD("MQTT_EVENT_BEFORE_CONNECT");
                        break;
                    }
                    default:
                    {
                        LOGE("Unhandled Event: %ld", event_id);
                        break;
                    }
                    }
                };
                if (const auto err = esp_mqtt_client_register_event(m_handle, MQTT_EVENT_ANY, event_handler, this); ESP_OK != err)
                {
                    LOGE("esp_mqtt_client_register_event failed");
                    return eFAILED;
                }
                if (const auto err = esp_mqtt_client_start(m_handle); ESP_OK != err)
                {
                    LOGE("esp_mqtt_client_start failed");
                    return eFAILED;
                }
                return eSUCCESS;
            }

            State Client::is_connected() const noexcept { return m_state; }

            void Client::publish(const char *topic, const char *data, size_t data_length, u8 qos, bool retain)
            {
                if (m_state != State::eCONNECTED)
                {
                    LOGW("Not connected");
                    return;
                }
                const auto message_id = esp_mqtt_client_publish(m_handle, topic, data, data_length, qos, retain);
                // LOGD("MessageID: %d", message_id);
            }

            Client::~Client() {}
        } // namespace MQTT
    } // namespace WebServices
} // namespace Omega
