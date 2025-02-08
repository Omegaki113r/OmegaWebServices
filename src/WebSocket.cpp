/**
 * @file WebSocket.cpp
 * @author Omegaki113r
 * @date Wednesday, 15th January 2025 2:30:53 am
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: WebSocket.cpp
 * File Created: Wednesday, 15th January 2025 2:30:53 am
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 23rd January 2025 10:53:38 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <cstring>

#include <esp_websocket_client.h>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/WebSocket.hpp"

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
        namespace WebSocket
        {
            namespace Client
            {
                OmegaStatus Init(const char *in_url) { return Init(in_url, nullptr, nullptr); }

                OmegaStatus Init(const char *in_url, const Authentication &in_auth) { return Init(in_url, in_auth.username, in_auth.password); }

                OmegaStatus Init(const char *in_url, const char *in_username, const char *in_password)
                {
                    if (nullptr == in_url || 0 == std::strlen(in_url))
                    {
                        LOGE("Provided URL is invalid");
                        return eFAILED;
                    }
                    const esp_websocket_client_config_t config{.uri = in_url, .username = in_username, .password = in_password};
                    const esp_websocket_client_handle_t handle = esp_websocket_client_init(&config);
                    const auto callback = [](void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
                    {
                        esp_websocket_event_data_t *data = (esp_websocket_event_data_t *)event_data;
                        switch (event_id)
                        {
                        case WEBSOCKET_EVENT_ERROR:
                        {
                            LOGD("WEBSOCKET_EVENT_ERROR");
                            break;
                        }
                        case WEBSOCKET_EVENT_CONNECTED:
                        {
                            LOGD("WEBSOCKET_EVENT_CONNECTED");
                            break;
                        }
                        case WEBSOCKET_EVENT_DISCONNECTED:
                        {
                            LOGD("WEBSOCKET_EVENT_DISCONNECTED");
                            break;
                        }
                        case WEBSOCKET_EVENT_DATA:
                        {
                            LOGD("WEBSOCKET_EVENT_DATA");
                            LOGD("Received Data length: %d", data->data_len);
                            const auto length = data->data_len;
                            if (length > 0)
                                on_data(reinterpret_cast<const u8 *>(data->data_ptr), length);
                            break;
                        }
                        case WEBSOCKET_EVENT_CLOSED:
                        {
                            LOGD("WEBSOCKET_EVENT_CLOSED");
                            break;
                        }
                        case WEBSOCKET_EVENT_BEFORE_CONNECT:
                        {
                            LOGD("WEBSOCKET_EVENT_BEFORE_CONNECT");
                            break;
                        }
                        case WEBSOCKET_EVENT_BEGIN:
                        {
                            LOGD("WEBSOCKET_EVENT_BEGIN");
                            break;
                        }
                        case WEBSOCKET_EVENT_FINISH:
                        {
                            LOGD("WEBSOCKET_EVENT_FINISH");
                            break;
                        }
                        default:
                        {
                            LOGE("Unhandled Event: %ld", event_id);
                            break;
                        }
                        }
                    };
                    if (ESP_OK != esp_websocket_register_events(handle, WEBSOCKET_EVENT_ANY, callback, NULL))
                    {
                        LOGE("esp_websocket_register_events failed");
                        return eFAILED;
                    }
                    if (ESP_OK != esp_websocket_client_start(handle))
                    {
                        LOGE("esp_websocket_client_start failed");
                        return eFAILED;
                    }
                    return eSUCCESS;
                }

                __attribute__((weak)) void on_data(const u8 *data, const size_t data_len) {}
            } // namespace Client

            namespace Server
            {
                __attribute__((weak)) void on_data(const u8 *data, const size_t data_len) {}
            } // namespace Server
        } // namespace WebSocketClient
    } // namespace WebServices
} // namespace Omega