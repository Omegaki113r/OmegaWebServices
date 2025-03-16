/**
 * @file MQTT.hpp
 * @author Omegaki113r
 * @date Sunday, 9th February 2025 7:00:28 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: MQTT.hpp
 * File Created: Sunday, 9th February 2025 7:00:28 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Sunday, 16th March 2025 9:59:39 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#pragma once

#include <cstring>
#include <functional>
#include <optional>
#include <variant>

// #include <mqtt_client.h>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"

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
            enum class State
            {
                eDISCONNECTED,
                eCONNECTED,
            };

            enum class Transport
            {
                eTCP = 0x01,
                eSSL = 0x02,
                eWS = 0x03,
                eWSS = 0x04,
            };

            struct BrokerURI
            {
                char m_uri[500];
                Transport m_transport;

                BrokerURI(const char *uri, Transport transport = Transport::eTCP)
                {
                    if (nullptr == uri || 0 == std::strlen(uri))
                    {
                        LOGE("Invalid URI parameter");
                        return;
                    }
                    strncpy(m_uri, uri, OMEGA_MIN(sizeof(m_uri), std::strlen(uri)));
                    m_uri[std::strlen(uri)] = '\0';

                    m_transport = transport;
                }
            };
            struct BrokerInfo
            {
                char m_host[500];
                u16 m_port;
                Transport m_transport;

                BrokerInfo(const char *host, u16 port, Transport transport = Transport::eTCP)
                {
                    if (nullptr == host || 0 == std::strlen(host))
                    {
                        LOGE("Invalid Host parameter");
                        return;
                    }
                    strncpy(m_host, host, OMEGA_MIN(sizeof(m_host), std::strlen(host)));
                    m_host[std::strlen(host)] = '\0';
                    m_port = port;
                    m_transport = transport;
                }
            };

            template <typename T>
            class Client
            {
            private:
                State m_state = State::eDISCONNECTED;
                std::variant<std::monostate, BrokerURI, BrokerInfo> m_connection_info;
                Authentication m_authentication;
                char m_client_id[100]{0};
                // esp_mqtt_client_handle_t m_handle;
                std::function<void(void)> m_on_connected;
                std::function<void(const u8 *, size_t)> m_on_data;
                std::function<void(void)> m_on_disconnected;
                T m_hardware_base;

            public:
                Client(T hardware_base) : m_hardware_base(hardware_base) {}
                Client(T hardware_base, const char *uri) : m_hardware_base(hardware_base), m_connection_info(BrokerURI{uri}) {}
                Client(T hardware_base, const char *host, u16 port) : m_hardware_base(hardware_base), m_connection_info(BrokerInfo{host, port}) {}
                Client &url(const char *in_uri) noexcept
                {
                    m_connection_info = BrokerURI{in_uri};
                    return *this;
                }
                Client &host(const char *in_host, u16 in_port) noexcept
                {
                    m_connection_info = BrokerInfo{in_host, in_port};
                    return *this;
                }
                Client &authentication(const Authentication &in_authentication) noexcept
                {
                    m_authentication = in_authentication;
                    return *this;
                }
                Client &authentication(const char *in_username, const char *in_password) noexcept
                {
                    if (nullptr == in_username || 0 == std::strlen(in_username))
                    {
                        LOGE("Invalid username");
                        return *this;
                    }
                    if (nullptr == in_password || 0 == std::strlen(in_password))
                    {
                        LOGE("Invalid password");
                        return *this;
                    }
                    m_authentication = {in_username, in_password};
                    return *this;
                }
                Client &client_id(const char *client_id) noexcept
                {
                    const auto client_id_length = std::strlen(client_id);
                    if (sizeof(m_client_id) > client_id_length)
                    {
                        UNUSED(std::memcpy(m_client_id, client_id, client_id_length));
                    }
                    return *this;
                }
                Client &on_connected(std::function<void(void)> connected_callback) noexcept
                {
                    m_on_connected = connected_callback;
                    return *this;
                }
                Client &on_data(std::function<void(const u8 *, size_t)> data_callback) noexcept
                {
                    m_on_data = data_callback;
                    return *this;
                }
                Client &on_disconnected(std::function<void(void)> disconnected_callback) noexcept
                {
                    m_on_disconnected = disconnected_callback;
                    return *this;
                }

                void set_url(const char *url) noexcept { m_connection_info = BrokerURI(url); }
                void set_host(const char *host, u16 port) noexcept { m_connection_info = BrokerInfo(host, port); }
                void set_authentication(const Authentication &authentication) noexcept { m_authentication = authentication; }
                void set_client_id(const char *client_id) noexcept
                {
                    if (nullptr == client_id || 0 == std::strlen(client_id))
                    {
                        LOGE("Invalid client_id parameter");
                        return;
                    }
                    const auto client_id_size = std::strlen(client_id);
                    strncpy(m_client_id, client_id, OMEGA_MIN(sizeof(m_client_id), client_id_size));
                    m_client_id[client_id_size] = '\0';
                }

                void add_on_connected_handler(std::function<void(void)> connected_callback) noexcept { m_on_connected = connected_callback; }
                void add_on_data_handler(std::function<void(const u8 *, size_t)> data_callback) noexcept { m_on_data = data_callback; }
                void add_on_disconnected_handler(std::function<void(void)> disconnected_callback) noexcept { m_on_disconnected = disconnected_callback; }
                OmegaStatus connect() noexcept
                {
                    if (std::holds_alternative<std::monostate>(m_connection_info))
                    {
                        LOGE("connection parameters are uninitialized");
                        return eFAILED;
                    }
                    if (std::holds_alternative<BrokerURI>(m_connection_info))
                    {
                        const auto connection_info = std::get<BrokerURI>(m_connection_info);
                        return m_hardware_base.connect_mqtt(connection_info.m_uri, m_authentication, m_client_id, m_on_connected, m_on_data, m_on_disconnected);
                    }
                    if (std::holds_alternative<BrokerInfo>(m_connection_info))
                    {
                        const auto connection_info = std::get<BrokerInfo>(m_connection_info);
                        return m_hardware_base.connect_mqtt(connection_info.m_host, connection_info.m_port, m_authentication, m_client_id, m_on_connected, m_on_data, m_on_disconnected);
                    }
                    return eFAILED;
                }
                State is_connected() const noexcept { return State::eDISCONNECTED; }
                OmegaStatus subscribe(const char *topic, u8 qos) noexcept
                {
                    m_hardware_base.subscribe_mqtt(topic, qos);
                    return eFAILED;
                }
                void publish(const char *topic, const u8 *data, size_t data_length, u8 qos = 0, bool retain = false) noexcept
                {
                    m_hardware_base.publish_mqtt(topic, data, data_length, qos);
                }
                OmegaStatus disconnect() noexcept { return m_hardware_base.disconnect_mqtt(m_on_disconnected); }

                ~Client() {}
            };
        } // namespace MQTT
    } // namespace WebServices
} // namespace Omega

#if CONFIG_OMEGA_WEB_SERVICES_DEBUG
#undef LOGD
#undef HEX_LOGD
#endif

#if CONFIG_OMEGA_WEB_SERVICES_INFO
#undef LOGI
#undef HEX_LOGI
#endif

#if CONFIG_OMEGA_WEB_SERVICES_WARN
#undef LOGW
#undef HEX_LOGW
#endif

#if CONFIG_OMEGA_WEB_SERVICES_ERROR
#undef LOGE
#undef HEX_LOGE
#endif