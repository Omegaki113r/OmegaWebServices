/**
 * @file MQTT.hpp
 * @author Omegaki113r
 * @date Sunday, 9th February 2025 7:00:28 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: OmegaWebServices
 * File Name: MQTT.hpp
 * File Created: Sunday, 9th February 2025 7:00:28 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Monday, 10th February 2025 2:12:11 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
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

#include <mqtt_client.h>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/Authentication.hpp"

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

            class Client
            {
            private:
                State m_state = State::eDISCONNECTED;
                std::variant<BrokerURI, BrokerInfo> m_connection_info;
                std::optional<Authentication> m_authentication;
                esp_mqtt_client_handle_t m_handle;
                std::function<void(void)> m_on_connected;
                std::function<void(const u8 *, size_t)> m_on_data;
                std::function<void(void)> m_on_disconnected;

            public:
                constexpr Client(const char *uri) : m_connection_info(BrokerURI{uri}) {}
                constexpr Client(const char *host, u16 port) : m_connection_info(BrokerInfo{host, port}) {}
                constexpr Client &url(const char *in_uri)
                {
                    m_connection_info = BrokerURI{in_uri};
                    return *this;
                }
                constexpr Client &host(const char *in_host, u16 in_port)
                {
                    m_connection_info = BrokerInfo{in_host, in_port};
                    return *this;
                }
                constexpr Client &authentication(const Authentication &in_authentication)
                {
                    m_authentication = in_authentication;
                    return *this;
                }
                constexpr Client &authentication(const char *in_username, const char *in_password)
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

                constexpr Client &on_connected(std::function<void(void)> connected_callback)
                {
                    m_on_connected = connected_callback;
                    return *this;
                }
                constexpr Client &on_data(std::function<void(const u8 *, size_t)> data_callback)
                {
                    m_on_data = data_callback;
                    return *this;
                }
                constexpr Client &on_disconnected(std::function<void(void)> disconnected_callback)
                {
                    m_on_disconnected = disconnected_callback;
                    return *this;
                }

                void add_on_connected_handler(std::function<void(void)> connected_callback) noexcept { m_on_connected = connected_callback; }
                void add_on_data_handler(std::function<void(const u8 *, size_t)> data_callback) noexcept { m_on_data = data_callback; }
                void add_on_disconnected_handler(std::function<void(void)> disconnected_callback) noexcept { m_on_disconnected = disconnected_callback; }
                OmegaStatus connect() noexcept;
                State is_connected() const noexcept;
                void publish(const char *topic, const char *data, size_t data_length, u8 qos = 0, bool retain = false) noexcept;

                ~Client();
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