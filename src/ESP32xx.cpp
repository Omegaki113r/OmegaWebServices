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
 * Last Modified: Tuesday, 4th March 2025 12:49:24 am
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"

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

        Response ESP32xx::perform(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            typedef struct
            {
                u8 *items;
                size_t count;
                size_t capacity;
            } String;
            struct _Response
            {
                std::function<void(const u8 *data, size_t data_length)> m_callback;
                Header m_header;
                String m_string;
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
                    _Response *_response = static_cast<_Response *>(evt->user_data);
                    if (nullptr == _response)
                        break;
                    _response->m_header.add_header(key, value);
                    break;
                }
                case HTTP_EVENT_ON_DATA:
                {
                    LOGD("HTTP_EVENT_ON_DATA");
                    u8 *data = static_cast<u8 *>(evt->data);
                    const size_t data_length = evt->data_len;
                    _Response *_response = static_cast<_Response *>(evt->user_data);
                    if (nullptr == _response)
                        break;
                    if (_response->m_callback)
                    {
                        _response->m_string.count += data_length;
                        _response->m_callback(data, data_length);
                        break;
                    }
                    if (0 == _response->m_string.capacity)
                    {
                        _response->m_string.count = 0;
                        _response->m_string.capacity = 1000;
                        _response->m_string.items = (u8 *)malloc(sizeof(u8) * _response->m_string.capacity);
                        if (nullptr == _response->m_string.items)
                        {
                            LOGE("Allocation failed");
                            return ESP_ERR_NO_MEM;
                        }
                    }
                    if (_response->m_string.capacity <= data_length + _response->m_string.count)
                    {
                        u8 *temp = static_cast<u8 *>(realloc(_response->m_string.items, sizeof(u8) * (_response->m_string.capacity * 2)));
                        if (temp == NULL)
                        {
                            LOGE("Allocation failed");
                            _response->m_string.count = 0;
                            _response->m_string.capacity = 0;
                            free(_response->m_string.items);
                            return ESP_ERR_NO_MEM;
                        }
                        _response->m_string.items = temp;
                        _response->m_string.capacity *= 2;
                    }
                    _response->m_string.count += data_length;
                    UNUSED(std::memcpy(_response->m_string.items, data, data_length));
                    // arena_sb_append_buf(&_response->m_buffer_arena, &_response->m_sb, data, data_length);
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
            _Response _response{chunked_callback};
            esp_http_client_method_t method = HTTP_METHOD_GET;
            switch (type)
            {
            case Request::RequsetType::GET:
            {
                method = HTTP_METHOD_GET;
                break;
            }
            case Request::RequsetType::POST:
            {
                method = HTTP_METHOD_POST;
                break;
            }
            }
            const esp_http_client_config_t config{.url = url, .username = auth.username, .password = auth.password, .method = method, .event_handler = http_handler, .user_data = &_response};
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

            // if (const auto err = esp_http_client_open(http_handle, 0); ESP_OK != err)
            // {
            //     LOGE("esp_http_client_open failed with %s", esp_err_to_name(err));
            //     return {eFAILED, {}};
            // }
            // const auto content_length = esp_http_client_fetch_headers(http_handle);
            // char *buffer = (char *)std::calloc(2000, sizeof(char));
            // int read_length = 0;
            // do
            // {
            //     read_length = esp_http_client_read(http_handle, buffer, 2000);
            // } while (0 < read_length);
            // std::free(buffer);
            // if (const auto err = esp_http_client_close(http_handle); ESP_OK != err)
            // {
            //     LOGE("esp_http_client_close failed with %s", esp_err_to_name(err));
            //     return {eFAILED, {}};
            // }
            // for (const auto &[key, value] : _response.m_header)
            // {
            //     LOGD("%s : %s", key, value);
            // }
            // // if (nullptr != chunked_callback)
            // // {
            // //     const auto read_length = esp_http_client_read_read();
            // // }

            // // HEX_LOGD(buffer, read_length);
            // return {eFAILED, {}};
            // // if ()
            // // {
            // // }

            if (const auto err = esp_http_client_perform(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_perform failed with %s", esp_err_to_name(err));
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            const auto status = esp_http_client_get_status_code(http_handle);
            [[maybe_unused]] const auto content_size = esp_http_client_get_content_length(http_handle);
            LOGD("Status: %d | Content size: %lld", status, content_size);
            if (const auto err = esp_http_client_cleanup(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            u8 *internal_buffer = nullptr;
            if (nullptr == chunked_callback)
            {
                internal_buffer = (u8 *)calloc(_response.m_string.count + 1, sizeof(u8));
                if (nullptr == internal_buffer)
                {
                    LOGE("allocating buffer failed");
                    return {eFAILED, {}};
                }
                UNUSED(std::memcpy(internal_buffer, _response.m_string.items, _response.m_string.count));
                free(_response.m_string.items);
            }
            return {eSUCCESS, {static_cast<u16>(status), _response.m_header, {internal_buffer, CHeapDeleter()}, _response.m_string.count}};
        }

        Response ESP32xx::perform(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }

    } // namespace WebServices
} // namespace Omega

#pragma GCC diagnostic pop