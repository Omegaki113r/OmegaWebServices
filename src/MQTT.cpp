/**
 * @file MQTT.cpp
 * @author Omegaki113r
 * @date Sunday, 9th February 2025 7:08:11 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: MQTT.cpp
 * File Created: Sunday, 9th February 2025 7:08:11 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Sunday, 9th February 2025 8:52:14 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */
#include <cstring>

#include <mqtt_client.h>

#include "OmegaWebServices/MQTT.hpp"

#include <sdkconfig.h>
#if CONFIG_OMEGA_WEB_SERVICES_DEBUG
#define LOGD(format, ...) OMEGA_LOGD(format, ##__VA_ARGS__)
#else
#define LOGD(format, ...)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_INFO
#define LOGI(format, ...) OMEGA_LOGI(format, ##__VA_ARGS__)
#else
#define LOGI(format, ...)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_WARN
#define LOGW(format, ...) OMEGA_LOGW(format, ##__VA_ARGS__)
#else
#define LOGW(format, ...)
#endif

#if CONFIG_OMEGA_WEB_SERVICES_ERROR
#define LOGE(format, ...) OMEGA_LOGE(format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)
#endif

namespace Omega
{
    namespace WebServices
    {
        namespace MQTT
        {
            namespace CLIENT
            {

                esp_mqtt_client_handle_t handle;
                OmegaStatus connect(const char *in_url, u16 in_port, const char *in_username, const char *in_password) noexcept
                {
                    if (nullptr == in_url || 0 == std::strlen(in_url))
                    {
                        LOGE("invalid URL");
                        return eFAILED;
                    }
                    const esp_mqtt_client_config_t::broker_t::address_t address{.hostname = in_url, .transport = MQTT_TRANSPORT_OVER_TCP, .port = in_port};
                    const esp_mqtt_client_config_t::broker_t broker{address};
                    const esp_mqtt_client_config_t::credentials_t::authentication_t authentication{in_password};
                    const esp_mqtt_client_config_t::credentials_t credential{.username = in_username, .authentication = authentication};
                    const esp_mqtt_client_config_t mqtt_config{.broker = broker, .credentials = credential};
                    handle = esp_mqtt_client_init(&mqtt_config);
                    const auto event_handler = [](void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
                    {
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
                            break;
                        }
                        case MQTT_EVENT_PUBLISHED:
                        {
                            LOGD("MQTT_EVENT_PUBLISHED");
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
                    if (const auto err = esp_mqtt_client_register_event(handle, MQTT_EVENT_ANY, event_handler, nullptr); ESP_OK != err)
                    {
                        LOGE("esp_mqtt_client_register_event failed");
                        return eFAILED;
                    }
                    if (const auto err = esp_mqtt_client_start(handle); ESP_OK != err)
                    {
                        LOGE("esp_mqtt_client_start failed");
                        return eFAILED;
                    }
                    return eSUCCESS;
                }

                OmegaStatus connect(const char *in_url, u16 in_port) noexcept
                {
                    if (nullptr == in_url || 0 == std::strlen(in_url))
                    {
                        LOGE("invalid URL");
                        return eFAILED;
                    }
                    const esp_mqtt_client_config_t::broker_t::address_t address{.hostname = in_url, .transport = MQTT_TRANSPORT_OVER_TCP, .port = in_port};
                    const esp_mqtt_client_config_t::broker_t broker{address};
                    const esp_mqtt_client_config_t mqtt_config{.broker = broker};
                    handle = esp_mqtt_client_init(&mqtt_config);
                    if (nullptr == handle)
                    {
                        LOGE("esp_mqtt_client_init failed");
                        return eFAILED;
                    }
                    const auto event_handler = [](void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
                    {
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
                            break;
                        }
                        case MQTT_EVENT_PUBLISHED:
                        {
                            LOGD("MQTT_EVENT_PUBLISHED");
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
                    if (const auto err = esp_mqtt_client_register_event(handle, MQTT_EVENT_ANY, event_handler, nullptr); ESP_OK != err)
                    {
                        LOGE("esp_mqtt_client_register_event failed");
                        return eFAILED;
                    }
                    if (const auto err = esp_mqtt_client_start(handle); ESP_OK != err)
                    {
                        LOGE("esp_mqtt_client_start failed");
                        return eFAILED;
                    }
                    return eSUCCESS;
                }

                void publish(const char *in_topic, const u8 *in_data, size_t in_data_length, u8 in_qos) noexcept
                {
                    publish(in_topic, (const char *)in_data, in_data_length, in_qos);
                }

                void publish(const char *in_topic, const char *in_data, size_t in_data_length, u8 in_qos) noexcept
                {
                    const auto message_id = esp_mqtt_client_publish(handle, in_topic, in_data, in_data_length, in_qos, false);
                    LOGD("MessageID: %d", message_id);
                }
            } // namespace CLIENT
        } // namespace MQTT
    } // namespace WebServices
} // namespace Omega
