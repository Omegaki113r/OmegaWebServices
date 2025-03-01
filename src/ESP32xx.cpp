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
 * Last Modified: Sunday, 2nd March 2025 12:15:56 am
 * Modified By: Omegaki113r (omegaki113r@gmail.com)
 * -----
 * Copyright 2025 - 2025 0m3g4ki113r, Xtronic
 * -----
 * HISTORY:
 * Date      	By	Comments
 * ----------	---	---------------------------------------------------------
 */

#include <memory>

#include <esp_http_client.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "OmegaUtilityDriver/UtilityDriver.hpp"
#include "OmegaWebServices/ESP32xx.hpp"

#define ARENA_IMPLEMENTATION
#include "OmegaUtilityDriver/arena.h"

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

        OmegaStatus ESP32xx::cleanup(esp_http_client_handle_t handle) noexcept
        {
            if (const auto err = esp_http_client_cleanup(handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return eFAILED;
            }
            return eSUCCESS;
        }

        Response ESP32xx::perform(const char *url, const Authentication &auth, const Header &header) noexcept
        {
            const auto empty_chunk_callback = [](const u8 *data, size_t data_length) {};
            return perform_chunked(url, auth, header, empty_chunk_callback);
        }

        Response ESP32xx::perform(const char *host, u16 port, const char *path, const Authentication &auth, const Header &header) noexcept
        {
            const auto empty_chunk_callback = [](const u8 *data, size_t data_length) {};
            return perform_chunked(host, port, path, auth, header, empty_chunk_callback);
        }

        Response ESP32xx::perform_chunked(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            typedef struct
            {
                char *items;
                size_t count;
                size_t capacity;
            } String_Builder;
            struct _Response
            {
                Header m_header;
                String_Builder m_sb;
                Arena m_buffer_arena;
                std::function<void(const u8 *data, size_t data_length)> m_callback;
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
                    _Response *response = static_cast<_Response *>(evt->user_data);
                    if (nullptr == response)
                        break;
                    response->m_header.add_header(key, value);
                    break;
                }
                case HTTP_EVENT_ON_DATA:
                {
                    LOGD("HTTP_EVENT_ON_DATA");
                    u8 *data = static_cast<u8 *>(evt->data);
                    const size_t data_length = evt->data_len;
                    _Response *response = static_cast<_Response *>(evt->user_data);
                    if (nullptr == response)
                        break;
                    // arena_sb_append_buf(&response->m_buffer_arena, &response->m_sb, data, data_length);
                    response->m_callback(data, data_length);
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
            _Response response{.m_callback = chunked_callback};
            const esp_http_client_config_t config{.url = url, .username = auth.username, .password = auth.password, .event_handler = http_handler, .user_data = &response};
            const esp_http_client_handle_t http_handle = esp_http_client_init(&config);
            if (nullptr == http_handle)
            {
                LOGE("esp_http_client_init failed");
                return {eFAILED, {}};
            }
            for (const auto &[key, value] : header)
            {
                if (const auto err = esp_http_client_set_header(http_handle, key, value); ESP_OK != err)
                {
                    LOGE("esp_http_client_set_header failed with %s", esp_err_to_name(err));
                    return {eFAILED, {}};
                }
            }
            if (const auto err = esp_http_client_perform(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_perform failed with %s", esp_err_to_name(err));
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            const auto status = esp_http_client_get_status_code(http_handle);
            const auto content_size = esp_http_client_get_content_length(http_handle);
            LOGD("Status: %d | Content size: %lld", status, content_size);
            if (const auto err = esp_http_client_cleanup(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            u8 *internal_buffer = (u8 *)calloc(response.m_sb.count + 1, sizeof(u8));
            if (nullptr == internal_buffer)
            {
                LOGE("allocating buffer failed");
                return {eFAILED, {}};
            }
            UNUSED(std::memcpy(internal_buffer, response.m_sb.items, response.m_sb.count));
            arena_free(&response.m_buffer_arena);
            return {eSUCCESS, {static_cast<u16>(status), response.m_header, {internal_buffer, CHeapDeleter()}, response.m_sb.count}};
        }

        Response ESP32xx::perform_chunked(const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            typedef struct
            {
                char *items;
                size_t count;
                size_t capacity;
            } String_Builder;
            struct _Response
            {
                Header m_header;
                String_Builder m_sb;
                Arena m_buffer_arena;
                std::function<void(const u8 *data, size_t data_length)> m_callback;
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
                    _Response *response = static_cast<_Response *>(evt->user_data);
                    if (nullptr == response)
                        break;
                    response->m_header.add_header(key, value);
                    break;
                }
                case HTTP_EVENT_ON_DATA:
                {
                    LOGD("HTTP_EVENT_ON_DATA");
                    u8 *data = static_cast<u8 *>(evt->data);
                    const size_t data_length = evt->data_len;
                    _Response *response = static_cast<_Response *>(evt->user_data);
                    if (nullptr == response)
                        break;
                    // arena_sb_append_buf(&response->m_buffer_arena, &response->m_sb, data, data_length);
                    response->m_callback(data, data_length);
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
            _Response response{.m_callback = chunked_callback};
            // char host_buffer[100]{0};
            // UNUSED(sprintf(host_buffer, "%s:%d", host, port));
            const esp_http_client_config_t config{.host = host, .port = port, .username = auth.username, .password = auth.password, .path = path, .event_handler = http_handler, .user_data = &response};
            const esp_http_client_handle_t http_handle = esp_http_client_init(&config);
            if (nullptr == http_handle)
            {
                LOGE("esp_http_client_init failed for %s/%s", config.host, config.path);
                return {eFAILED, {}};
            }
            for (const auto &[key, value] : header)
            {
                if (const auto err = esp_http_client_set_header(http_handle, key, value); ESP_OK != err)
                {
                    LOGE("esp_http_client_set_header failed with %s", esp_err_to_name(err));
                    return {eFAILED, {}};
                }
            }
            if (const auto err = esp_http_client_perform(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_perform failed with %s", esp_err_to_name(err));
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            const auto status = esp_http_client_get_status_code(http_handle);
            const auto content_size = esp_http_client_get_content_length(http_handle);
            LOGD("Status: %d | Content size: %lld", status, content_size);
            if (const auto err = esp_http_client_cleanup(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            u8 *internal_buffer = (u8 *)calloc(response.m_sb.count + 1, sizeof(u8));
            if (nullptr == internal_buffer)
            {
                LOGE("allocating buffer failed");
                return {eFAILED, {}};
            }
            UNUSED(std::memcpy(internal_buffer, response.m_sb.items, response.m_sb.count));
            arena_free(&response.m_buffer_arena);
            return {eSUCCESS, {static_cast<u16>(status), response.m_header, {internal_buffer, CHeapDeleter()}, response.m_sb.count}};
        }

    } // namespace WebServices
} // namespace Omega