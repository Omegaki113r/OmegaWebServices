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
 * Last Modified: Tuesday, 11th March 2025 8:43:57 am
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

        Response ESP32xx::perform_get(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback)
        {
            struct String
            {
                char *string;
                size_t count;
                size_t capacity;
            };
            struct _Response
            {
                std::function<void(const u8 *data, size_t data_length)> m_callback;
                Header m_header;
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

            esp_http_client_config_t config{.url = url, .username = auth.username, .password = auth.password, .method = HTTP_METHOD_GET, .event_handler = http_handler, .user_data = &_response};
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
            if (const auto err = esp_http_client_open(http_handle, 0); ESP_OK != err)
            {
                LOGE("esp_http_client_open failed with %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            const auto content_length = esp_http_client_fetch_headers(http_handle);
            constexpr size_t buffer_length = 1 * 1024;
            String _string_builder{};
            _string_builder.string = (char *)std::calloc(buffer_length, sizeof(char));
            if (nullptr == _string_builder.string)
            {
                LOGE("buffer allocation failed");
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            _string_builder.capacity = buffer_length;
            int read_length = 0;
            do
            {
                read_length = esp_http_client_read(http_handle, _string_builder.string, buffer_length);
                if (0 >= read_length)
                {
                    break;
                }
                _string_builder.count += read_length;
                if (nullptr != chunked_callback)
                {
                    chunked_callback(reinterpret_cast<u8 *>(_string_builder.string), read_length);
                }
                else
                {
                    if (_string_builder.capacity <= read_length + _string_builder.count)
                    {
                        char *temp = static_cast<char *>(std::realloc(_string_builder.string, sizeof(char) * (_string_builder.capacity * 2)));
                        if (nullptr == temp)
                        {
                            LOGE("Allocation failed");
                            _string_builder.count = 0;
                            _string_builder.capacity = 0;
                            std::free(_string_builder.string);
                            _string_builder.string = nullptr;
                            break;
                        }
                        _string_builder.string = temp;
                        _string_builder.capacity *= 2;
                    }
                    _string_builder.count += read_length;
                }
            } while (0 < read_length);
            if (nullptr == chunked_callback)
            {
                std::free(_string_builder.string);
            }
            if (const auto err = esp_http_client_close(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_close failed with %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            const auto status = esp_http_client_get_status_code(http_handle);
            if (const auto err = esp_http_client_cleanup(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            u8 *internal_buffer = nullptr;
            if (nullptr == chunked_callback)
            {
                internal_buffer = (u8 *)calloc(_string_builder.count + 1, sizeof(u8));
                if (nullptr == internal_buffer)
                {
                    LOGE("allocating buffer failed");
                    return {eFAILED, {}};
                }
                UNUSED(std::memcpy(internal_buffer, _string_builder.string, _string_builder.count));
                std::free(_string_builder.string);
            }
            return {eSUCCESS, {static_cast<u16>(status), _response.m_header, {internal_buffer, CHeapDeleter()}, _string_builder.count}};
        }

        Response ESP32xx::perform_post(const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback)
        {
            struct String
            {
                char *string;
                size_t count;
                size_t capacity;
            };
            struct _Response
            {
                std::function<void(const u8 *data, size_t data_length)> m_callback;
                Header m_header;
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
            esp_http_client_config_t config{.url = url, .username = auth.username, .password = auth.password, .method = HTTP_METHOD_POST, .event_handler = http_handler, .user_data = &_response};
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
            const size_t file_size = 4 * 1024 * 1024;
            if (const auto err = esp_http_client_open(http_handle, -1); ESP_OK != err)
            {
                LOGE("esp_http_client_open failed with %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            constexpr size_t buffer_length = 1 * 1024;
            String _string_builder{};
            _string_builder.string = (char *)std::calloc(buffer_length + 1, sizeof(char));
            if (nullptr == _string_builder.string)
            {
                LOGE("buffer allocation failed");
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            _string_builder.capacity = buffer_length;
            int read_length = 0;
            UNUSED(std::memset(_string_builder.string, 'h', buffer_length));

            char *chunk_buffer = (char *)std::calloc(100 + buffer_length + 2, sizeof(char));
            if (nullptr == chunk_buffer)
            {
                LOGE("Buffer allocation failed");
                return {eFAILED, {}};
            }

            for (size_t idx = 0; idx < file_size;)
            {
                char chunk_size_header[100]{0};
                size_t written_size = 0;

                if (written_size = snprintf(chunk_buffer, 100 + buffer_length + 2, "%x\r\n%s\r\n", buffer_length, _string_builder.string); 0 > written_size)
                {
                    LOGE("snprintf failed");
                    return {eFAILED, {}};
                }

                if (const auto written_amount = esp_http_client_write(http_handle, chunk_buffer, std::strlen(chunk_buffer)); 0 > written_amount)
                {
                    LOGE("esp_http_client_write failed. %d %d", written_amount, idx);
                    return {eFAILED, {}};
                }
                idx += buffer_length;
            }
            static constexpr char end_buffer[] = "0\r\n\r\n";
            if (const auto written_amount = esp_http_client_write(http_handle, end_buffer, std::strlen(end_buffer)); 0 > written_amount)
            {
                LOGE("esp_http_client_write failed");
                return {eFAILED, {}};
            }

            const auto content_length = esp_http_client_fetch_headers(http_handle);
            int read_size = 0;
            if (read_size = esp_http_client_read(http_handle, _string_builder.string, content_length); 0 > read_size)
            {
                LOGE("esp_http_client_read failed");
                return {eFAILED, {}};
            }
            _string_builder.count = 0 > read_size ? 0 : read_size;
            if (const auto err = esp_http_client_close(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_close failed with %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            const auto status = esp_http_client_get_status_code(http_handle);
            if (const auto err = esp_http_client_cleanup(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            u8 *internal_buffer = nullptr;
            if (nullptr == chunked_callback)
            {
                internal_buffer = (u8 *)calloc(_string_builder.count + 1, sizeof(u8));
                if (nullptr == internal_buffer)
                {
                    LOGE("allocating buffer failed");
                    return {eFAILED, {}};
                }
                UNUSED(std::memcpy(internal_buffer, _string_builder.string, _string_builder.count));
                std::free(_string_builder.string);
            }
            return {eSUCCESS, {static_cast<u16>(status), _response.m_header, {internal_buffer, CHeapDeleter()}, static_cast<size_t>(content_length)}};
        }

        Response ESP32xx::perform(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            if (Request::RequsetType::GET == type)
            {
                return perform_get(url, auth, header, chunked_callback);
            }
            else if (Request::RequsetType::POST == type)
            {
                return perform_post(url, auth, header, chunked_callback);
            }
            return {eFAILED, {}};
        }

        Response ESP32xx::perform(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }

    } // namespace WebServices
} // namespace Omega

#pragma GCC diagnostic pop