/**
 * @file ESP32xx.cpp
 * @author Omegaki113r
 * @date Friday, 21st February 225 4:30:23 pm
 * @copyright Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * */
/*
 * Project: src
 * File Name: ESP32xx.cpp
 * File Created: Friday, 21st February 2025 4:30:23 pm
 * Author: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Last Modified: Thursday, 27th February 2025 4:00:30 pm
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <esp_http_client.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/ESP32xx.hpp"

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
        constexpr size_t BUFFER_INCREMENT = 1000;

        Response ESP32xx::perform(const char *url, const Authentication &auth, const Header &header) noexcept
        {
            struct _Response
            {
                Header m_header;
                u8 *m_buffer;
                size_t m_capacity;
                size_t m_size;
            };
            const auto http_handler = [](esp_http_client_event_t *evt)
            {
                const esp_http_client_event_id_t event_id = evt->event_id;
                switch (event_id)
                {
                case HTTP_EVENT_ERROR:
                {
                    LOGD("HTTP_EVENT_ERROR");
                    break;
                }
                case HTTP_EVENT_ON_CONNECTED:
                {
                    LOGD("HTTP_EVENT_ON_CONNECTED");
                    break;
                }
                case HTTP_EVENT_HEADERS_SENT:
                {
                    LOGD("HTTP_EVENT_HEADERS_SENT");
                    break;
                }
                case HTTP_EVENT_ON_HEADER:
                {
                    LOGD("HTTP_EVENT_ON_HEADER");
                    const char *key = evt->header_key;
                    const char *value = evt->header_value;
                    LOGD("[%s]: %s", key, value);
                    _Response *response = static_cast<_Response *>(evt->user_data);
                    response->m_header.add_header({key}, {value});
                    break;
                }
                case HTTP_EVENT_ON_DATA:
                {
                    LOGD("HTTP_EVENT_ON_DATA");
                    u8 *data = static_cast<u8 *>(evt->data);
                    const size_t data_length = evt->data_len;
                    HEX_LOGD(data, data_length);
                    _Response *response = static_cast<_Response *>(evt->user_data);
                    if (nullptr == response)
                        break;
                    if (!esp_http_client_is_chunked_response(evt->client))
                    {
                        LOGD("Not a Chunked response");
                        response->m_capacity = data_length;
                        response->m_buffer = (u8 *)std::calloc(data_length, sizeof(u8));
                        UNUSED(std::memcpy(response->m_buffer, data, data_length));
                        response->m_size = data_length;
                        return ESP_OK;
                    }
                    if (0 == response->m_capacity)
                    {
                        LOGD("Allocating memory");
                        response->m_capacity = BUFFER_INCREMENT;
                        response->m_buffer = (u8 *)std::calloc(response->m_capacity, sizeof(u8));
                        if (nullptr == response->m_buffer)
                        {
                            LOGE("Allocating buffer failed");
                            response->m_capacity = 0;
                            response->m_size = 0;
                            return ESP_FAIL;
                        }
                    }
                    if (response->m_capacity <= response->m_size + data_length)
                    {
                        LOGD("Reallocating memory: %d < %d + %d", response->m_capacity, response->m_size, data_length);
                        response->m_capacity += BUFFER_INCREMENT;
                        u8 *reallocated = (u8 *)std::realloc(response->m_buffer, response->m_capacity * sizeof(u8));
                        if (nullptr == reallocated)
                        {
                            LOGE("Reallocating buffer failed");
                            response->m_buffer = nullptr;
                            response->m_capacity = 0;
                            response->m_size = 0;
                            return ESP_FAIL;
                        }
                        response->m_buffer = reallocated;
                    }
                    UNUSED(std::memcpy(&response->m_buffer[response->m_size], data, data_length));
                    response->m_size += data_length;
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
                    LOGE("Unhandled Event id: %d", event_id);
                    break;
                }
                }
                return ESP_OK;
            };
            _Response response{};
            esp_http_client_config_t config{.url = url, .username = auth.username, .password = auth.password, .event_handler = http_handler, .user_data = &response};
            const esp_http_client_handle_t http_handle = esp_http_client_init(&config);
            if (nullptr == http_handle)
            {
                LOGE("esp_http_client_init failed");
                return {eFAILED, {}};
            }
            for (const auto &[key, value] : header)
            {
                if (const auto err = esp_http_client_set_header(http_handle, key.c_str(), value.c_str()); ESP_OK != err)
                {
                    LOGE("esp_http_client_set_header failed with %s", esp_err_to_name(err));
                    return {eFAILED, {}};
                }
            }
            if (const auto err = esp_http_client_perform(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_perform failed with %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            if (nullptr != response.m_buffer)
                HEX_LOGD(response.m_buffer, response.m_size);
            free(response.m_buffer);
            response.m_buffer = nullptr;
            const auto status = esp_http_client_get_status_code(http_handle);
            const auto content_size = esp_http_client_get_content_length(http_handle);
            LOGD("Status: %d | Content size: %lld", status, content_size);
            if (const auto err = esp_http_client_cleanup(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            return {eSUCCESS, {response.m_header}};
        }
    } // namespace WebServices
} // namespace Omega