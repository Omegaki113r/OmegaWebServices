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
 * Last Modified: Monday, 17th March 2025 2:39:55 pm
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
#include "OmegaWebServices/WebServices.hpp"

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
            handle = nullptr;
            return eSUCCESS;
        }

        OmegaStatus ESP32xx::cleanup(esp_mqtt_client_handle_t handle) noexcept
        {
            if (const auto err = esp_mqtt_client_destroy(handle); ESP_OK != err)
            {
                LOGE("esp_mqtt_client_destroy failed with: %s", esp_err_to_name(err));
                return eFAILED;
            }
            handle = nullptr;
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

        Response ESP32xx::stream_post(const char *url, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> stream_callback)
        {
            struct String
            {
                char *string;
                size_t count;
                size_t capacity;
            };
            struct _Response
            {
                std::function<void(u8 *data, size_t *data_length)> m_callback;
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
            _Response _response{stream_callback};
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
                    cleanup(http_handle);
                    return {eFAILED, {}};
                }
            }
            if (const auto err = esp_http_client_open(http_handle, -1); ESP_OK != err)
            {
                LOGE("esp_http_client_open failed with %s", esp_err_to_name(err));
                cleanup(http_handle);
                return {eFAILED, {}};
            }

            static constexpr auto chunk_size = 1 * 1024;
            static constexpr auto header_size = 100;
            u8 *user_buffer = (u8 *)calloc(chunk_size + 1, sizeof(char));
            size_t user_buffer_length = 0;
            char *formatted_buffer = (char *)std::calloc(header_size + chunk_size, sizeof(char));
            if (nullptr == formatted_buffer)
            {
                LOGE("Memory allocation failed");
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            for (;;)
            {
                stream_callback(user_buffer, &user_buffer_length);
                // user_buffer[user_buffer_length - 1] = '\0';
                if (nullptr == user_buffer || 0 == user_buffer_length)
                    break;
                if (const auto written_size = snprintf(formatted_buffer, header_size + chunk_size, "%x\r\n%s\r\n", user_buffer_length, user_buffer); 0 > written_size)
                {
                    LOGE("snprintf failed");
                    cleanup(http_handle);
                    return {eFAILED, {}};
                }
                const auto buffer_size = std::strlen(formatted_buffer);
                if (const auto written_size = esp_http_client_write(http_handle, formatted_buffer, buffer_size); 0 > written_size)
                {
                    LOGE("esp_http_client_write failed");
                    cleanup(http_handle);
                    return {eFAILED, {}};
                }
            }
            static constexpr char end_buffer[] = "0\r\n\r\n";
            if (const auto written_amount = esp_http_client_write(http_handle, end_buffer, std::strlen(end_buffer)); 0 > written_amount)
            {
                LOGE("esp_http_client_write failed");
                cleanup(http_handle);
                return {eFAILED, {}};
            }

            const auto content_length = esp_http_client_fetch_headers(http_handle);
            String response_string_builder{0};
            response_string_builder.string = (char *)std::calloc(content_length + 1, sizeof(char));
            if (nullptr == response_string_builder.string)
            {
                LOGE("Memory Allocation failed");
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            int read_size = 0;
            if (read_size = esp_http_client_read(http_handle, response_string_builder.string, content_length); 0 > read_size)
            {
                LOGE("esp_http_client_read failed");
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            response_string_builder.count = 0 > read_size ? 0 : read_size;
            if (const auto err = esp_http_client_close(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_close failed with %s", esp_err_to_name(err));
                cleanup(http_handle);
                return {eFAILED, {}};
            }
            const auto status = esp_http_client_get_status_code(http_handle);
            if (const auto err = esp_http_client_cleanup(http_handle); ESP_OK != err)
            {
                LOGE("esp_http_client_cleanup failed with: %s", esp_err_to_name(err));
                return {eFAILED, {}};
            }
            return {eSUCCESS, {static_cast<u16>(status), _response.m_header, {reinterpret_cast<u8 *>(response_string_builder.string), CHeapDeleter()}, static_cast<size_t>(content_length)}};
        }

        Response ESP32xx::perform_request(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
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

        Response ESP32xx::perform_request(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(const u8 *data, size_t data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }

        Response ESP32xx::stream_request(Request::RequsetType type, const char *url, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept
        {
            if (Request::RequsetType::GET == type)
            {
                LOGE("NOT IMPLEMENTED");
                return {eFAILED, {}};
            }
            else if (Request::RequsetType::POST == type)
            {
                return stream_post(url, auth, header, chunked_callback);
            }
            return {eFAILED, {}};
        }

        Response ESP32xx::stream_request(Request::RequsetType type, const char *host, u16 port, const char *path, const Authentication &auth, const Header &header, std::function<void(u8 *data, size_t *data_length)> chunked_callback) noexcept
        {
            LOGE("NOT IMPLEMENTED");
            return {eFAILED, {}};
        }

        struct _Client
        {
            std::function<void(void)> m_on_connected;
            std::function<void(const char *, const u8 *, size_t)> m_on_data;
            std::function<void(void)> m_on_disconnected;
            ::Omega::WebServices::State *m_state;
        };

        State ESP32xx::get_mqtt_connection_state() const noexcept { return m_state; }

        OmegaStatus ESP32xx::connect_mqtt(const char *url, const Authentication &auth, const char *client_id, std::function<void(void)> on_connected, std::function<void(const char *, const u8 *, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept
        {
            const esp_mqtt_client_config_t::broker_t::address_t address{.uri = url};
            const esp_mqtt_client_config_t::broker_t broker{address};
            const esp_mqtt_client_config_t::credentials_t::authentication_t authentication{auth.password};
            const esp_mqtt_client_config_t::credentials_t credential{.username = auth.username, .client_id = client_id, .authentication = authentication};
            const esp_mqtt_client_config_t mqtt_config = {.broker = broker, .credentials = credential};
            m_handle = esp_mqtt_client_init(&mqtt_config);
            if (nullptr == m_handle)
            {
                LOGE("esp_mqtt_client_init failed");
                return eFAILED;
            }
            const auto event_handler = [](void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
            {
                _Client *client = (_Client *)event_handler_arg;
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
                    if (client->m_state)
                        *client->m_state = ::Omega::WebServices::State::eCONNECTED;
                    if (client->m_on_connected)
                        client->m_on_connected();
                    break;
                }
                case MQTT_EVENT_DISCONNECTED:
                {
                    LOGD("MQTT_EVENT_DISCONNECTED");
                    if (client->m_state)
                        *client->m_state = ::Omega::WebServices::State::eCONNECTED;
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
                    const auto topic = reinterpret_cast<char *>(event->topic);
                    const auto data = reinterpret_cast<u8 *>(event->data);
                    const auto topic_length = event->topic_len;
                    const auto data_length = event->data_len;

                    char *topic_buffer = (char *)malloc(topic_length + 1);
                    if (topic_buffer)
                    {
                        UNUSED(std::memcpy(topic_buffer, topic, topic_length));
                        topic_buffer[topic_length] = '\0';
                    }
                    u8 *data_buffer = (u8 *)malloc(data_length + 1);
                    if (data_buffer)
                    {
                        UNUSED(std::memcpy(data_buffer, data, data_length));
                        data_buffer[data_length] = '\0';
                    }
                    if (client->m_on_data)
                        client->m_on_data(topic_buffer, data_buffer, data_length);
                    free(topic_buffer);
                    free(data_buffer);
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
            _Client *client = (_Client *)calloc(1, sizeof(_Client));
            if (nullptr == client)
            {
                cleanup(m_handle);
                return eFAILED;
            }
            client->m_on_connected = on_connected;
            client->m_on_data = on_data;
            client->m_on_disconnected = on_disconnected;
            client->m_state = &m_state;
            if (const auto err = esp_mqtt_client_register_event(m_handle, MQTT_EVENT_ANY, event_handler, client); ESP_OK != err)
            {
                LOGE("esp_mqtt_client_register_event failed");
                cleanup(m_handle);
                return eFAILED;
            }
            if (const auto err = esp_mqtt_client_start(m_handle); ESP_OK != err)
            {
                LOGE("esp_mqtt_client_start failed");
                cleanup(m_handle);
                return eFAILED;
            }
            return eSUCCESS;
        }

        OmegaStatus ESP32xx::connect_mqtt(const char *host, u16 port, const Authentication &auth, const char *client_id, std::function<void(void)> on_connected, std::function<void(const char *, const u8 *, size_t)> on_data, std::function<void(void)> on_disconnected) noexcept
        {
            const esp_mqtt_client_config_t::broker_t::address_t address{.hostname = host, .transport = MQTT_TRANSPORT_OVER_TCP, .port = port};
            const esp_mqtt_client_config_t::broker_t broker{address};
            const esp_mqtt_client_config_t::credentials_t::authentication_t authentication{auth.password};
            const esp_mqtt_client_config_t::credentials_t credential{.username = auth.username, .client_id = client_id, .authentication = authentication};
            const esp_mqtt_client_config_t mqtt_config = {.broker = broker, .credentials = credential};
            m_handle = esp_mqtt_client_init(&mqtt_config);
            if (nullptr == m_handle)
            {
                LOGE("esp_mqtt_client_init failed");
                return eFAILED;
            }
            const auto event_handler = [](void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
            {
                _Client *client = (_Client *)event_handler_arg;
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
                    if (client->m_state)
                        *client->m_state = ::Omega::WebServices::State::eCONNECTED;
                    if (client->m_on_connected)
                        client->m_on_connected();
                    break;
                }
                case MQTT_EVENT_DISCONNECTED:
                {
                    LOGD("MQTT_EVENT_DISCONNECTED");
                    if (client->m_state)
                        *client->m_state = ::Omega::WebServices::State::eCONNECTED;
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
                    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
                    const auto topic = reinterpret_cast<char *>(event->topic);
                    const auto data = reinterpret_cast<u8 *>(event->data);
                    const auto topic_length = event->topic_len;
                    const auto data_length = event->data_len;

                    char *topic_buffer = (char *)malloc(topic_length + 1);
                    if (topic_buffer)
                    {
                        UNUSED(std::memcpy(topic_buffer, topic, topic_length));
                        topic_buffer[topic_length] = '\0';
                    }
                    u8 *data_buffer = (u8 *)malloc(data_length + 1);
                    if (data_buffer)
                    {
                        UNUSED(std::memcpy(data_buffer, data, data_length));
                        data_buffer[data_length] = '\0';
                    }
                    if (client->m_on_data)
                        client->m_on_data(topic_buffer, data_buffer, data_length);
                    free(topic_buffer);
                    free(data_buffer);
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
            _Client *client = (_Client *)calloc(1, sizeof(_Client));
            if (nullptr == client)
            {
                cleanup(m_handle);
                return eFAILED;
            }
            client->m_on_connected = on_connected;
            client->m_on_data = on_data;
            client->m_on_disconnected = on_disconnected;
            client->m_state = &m_state;
            if (const auto err = esp_mqtt_client_register_event(m_handle, MQTT_EVENT_ANY, event_handler, client); ESP_OK != err)
            {
                LOGE("esp_mqtt_client_register_event failed");
                cleanup(m_handle);
                return eFAILED;
            }
            if (const auto err = esp_mqtt_client_start(m_handle); ESP_OK != err)
            {
                LOGE("esp_mqtt_client_start failed");
                cleanup(m_handle);
                return eFAILED;
            }
            return eSUCCESS;
        }

        OmegaStatus ESP32xx::publish_mqtt(const char *topic, const u8 *data, size_t data_length, u8 qos) noexcept
        {
            if (m_state != State::eCONNECTED)
            {
                LOGW("Not connected");
                return eFAILED;
            }
            const auto message_id = esp_mqtt_client_publish(m_handle, topic, reinterpret_cast<const char *>(data), data_length, qos, false);
            // LOGD("MessageID: %d", message_id);
            return eSUCCESS;
        }

        OmegaStatus ESP32xx::subscribe_mqtt(const char *topic, u8 qos) noexcept
        {
            if (m_state != State::eCONNECTED)
            {
                LOGW("Not connected");
                return eFAILED;
            }
            if (const auto message_id = esp_mqtt_client_subscribe(m_handle, topic, qos); 0 < message_id)
            {
                LOGE("esp_mqtt_client_subscribe failed");
                return eFAILED;
            }
            return eSUCCESS;
        }

        OmegaStatus ESP32xx::disconnect_mqtt(std::function<void(void)> on_disconnected) noexcept { return eFAILED; }
    } // namespace WebServices
} // namespace Omega

#pragma GCC diagnostic pop