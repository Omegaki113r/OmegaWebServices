/**
 * @file WebServices.cpp
 * @author Omegaki113r
 * @date Thursday, 2nd January 2025 8:01:20 pm
 * @copyright Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: WebServices.cpp
 * File Created: Thursday, 2nd January 2025 8:01:20 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Friday, 3rd January 2025 8:13:58 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright <<projectCreationYear>> - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <string>
#include <variant>
#include <vector>

#include <esp_http_client.h>

#include "OmegaWebServices/WebServices.hpp"

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
        Response GET(const std::string &in_url) { return GET(in_url.c_str()); }

        Response GET(const char *in_url)
        {
            std::vector<u8> received_data{};
            const auto http_event_handler = [](esp_http_client_event_t *evt)
            {
                switch (evt->event_id)
                {
                case HTTP_EVENT_ERROR:
                {
                    LOGE("HTTP_EVENT_ERROR");
                    break;
                }
                case HTTP_EVENT_ON_CONNECTED:
                {
                    LOGD("HTTP_EVENT_ON_CONNECTED");
                    break;
                }
                case HTTP_EVENT_HEADER_SENT:
                {
                    LOGD("HTTP_EVENT_HEADERS_SENT/HTTP_EVENT_HEADER_SENT");
                    break;
                }
                case HTTP_EVENT_ON_HEADER:
                {
                    LOGD("HTTP_EVENT_ON_HEADER");
                    break;
                }
                case HTTP_EVENT_ON_DATA:
                {
                    LOGD("HTTP_EVENT_ON_DATA");
                    auto *vec = static_cast<std::vector<u8> *>(evt->user_data);
                    uint8_t *data = static_cast<uint8_t *>(evt->data);
                    vec->assign(data, data + evt->data_len);
                    break;
                }
                case HTTP_EVENT_ON_FINISH:
                {
                    LOGD("HTTP_EVENT_ON_FINISH");
                    break;
                }
                case HTTP_EVENT_DISCONNECTED:
                {
                    LOGD("HTTP_EVENT_DISCONNECTED");
                    break;
                }
                case HTTP_EVENT_REDIRECT:
                {
                    LOGD("HTTP_EVENT_REDIRECT");
                    break;
                }
                default:
                {
                    LOGE("Unhandled event: %d", evt->event_id);
                    break;
                }
                }
                return ESP_OK;
            };
            esp_http_client_config_t http_config{
                .url = in_url,
                .event_handler = http_event_handler,
                .user_data = &received_data,
            };
            esp_http_client_handle_t handle = esp_http_client_init(&http_config);
            esp_http_client_perform(handle);
            esp_http_client_cleanup(handle);
            return {eSUCCESS, received_data};
        }
    } // namespace WebServices
} // namespace Omega